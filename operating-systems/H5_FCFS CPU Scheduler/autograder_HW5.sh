#!/bin/bash

# =============================================================================
# ASSIGNMENT SETTINGS (Edit these for each assignment)
# =============================================================================
ASSIGNMENT_NAME="HOMEWORK-5: FCFS Scheduler"
TOTAL_POINTS=100

# Test case weights (must sum to exactly 100)
declare -A TEST_WEIGHTS=(
    ["1"]=25    # Test 1 worth 25 points 
    ["2"]=25    # Test 2 worth 25 points 
    ["3"]=25    # Test 3 worth 25 points 
    ["4"]=25    # Test 4 worth 25 points     
)

TIMEOUT_SECONDS=10

# Function to find makefile with case-insensitive matching
find_makefile() {
    local makefile_path=""
    
    # Try exact matches first (preferred)
    if [[ -f "Makefile" ]]; then
        makefile_path="Makefile"
    elif [[ -f "makefile" ]]; then
        makefile_path="makefile"
    else
        # Try case-insensitive search for any makefile variant
        makefile_path=$(find . -maxdepth 1 -iname "makefile*" -type f | head -1)
    fi
    
    echo "$makefile_path"
}

# =============================================================================
# FILE SETTINGS
# =============================================================================
STUDENT_FILE="submission_HW5.c"
EXECUTABLE_NAME="fcfs"

# =============================================================================
# COLORS FOR OUTPUT
# =============================================================================
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# =============================================================================
# INITIALIZE SCORES
# =============================================================================
TOTAL_TESTS=0
declare -a TEST_SCORES  # Array to store individual test scores

# =============================================================================
# UTILITY FUNCTIONS
# =============================================================================
print_success() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ️ $1${NC}"
}

print_partial() {
    echo -e "${YELLOW}⚡ $1${NC}"
}

# Function to calculate proportional score for a test case
calculate_proportional_score() {
    local test_num=$1
    local expected_file=$2
    local student_output=$3
    local crashed=$4
    
    # If program crashed, return 0
    if [[ $crashed -eq 1 ]]; then
        echo "0"
        return
    fi
    
    # If files don't exist, return 0
    if [[ ! -f "$expected_file" || ! -f "$student_output" ]]; then
        echo "0"
        return
    fi
    
    # Check if outputs are identical (full credit)
    if diff -w -B "$expected_file" "$student_output" > /dev/null 2>&1; then
        echo "1.0"
        return
    fi
    
    # Calculate proportional score based on line matching
    local expected_content=""
    local student_content=""
    
    if [[ -f "$expected_file" ]]; then
        expected_content=$(cat "$expected_file")
    fi
    
    if [[ -f "$student_output" ]]; then
        student_content=$(cat "$student_output")
    fi
    
    # If student produced no output, return 0
    if [[ -z "$student_content" ]]; then
        echo "0"
        return
    fi
    
    # Convert to arrays for line-by-line comparison
    local -a expected_lines
    local -a student_lines
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            expected_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
        fi
    done <<< "$expected_content"
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            student_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
        fi
    done <<< "$student_content"
    
    # Line-by-line comparison
    local total_lines=${#expected_lines[@]}
    local correct_lines=0
    
    if [[ $total_lines -eq 0 ]]; then
        echo "0"
        return
    fi
    
    for ((i=0; i<total_lines; i++)); do
        if [[ i -lt ${#student_lines[@]} && "${expected_lines[i]}" == "${student_lines[i]}" ]]; then
            ((correct_lines++))
        fi
    done
    
    # Penalize extra lines in student output
    if [[ ${#student_lines[@]} -gt $total_lines ]]; then
        local penalty=$(( ${#student_lines[@]} - total_lines ))
        correct_lines=$(( correct_lines > penalty ? correct_lines - penalty : 0 ))
    fi
    
    # Calculate percentage
    local score=0
    if [[ $total_lines -gt 0 ]]; then
        score=$(echo "scale=3; $correct_lines / $total_lines" | bc -l 2>/dev/null || echo "0")
    fi
    
    # Cap at 1.0
    local max_check=$(echo "$score > 1.0" | bc -l 2>/dev/null || echo "0")
    if [[ $max_check -eq 1 ]]; then
        score="1.0"
    fi
    
    echo "$score"
}

# Function to display detailed differences
show_detailed_diff() {
    local test_num=$1
    local expected_file=$2
    local student_output=$3
    local score=$4
    local test_weight=$5
    
    echo -e "${CYAN}📋 Detailed Analysis for Test $test_num:${NC}"
    
    # Show score earned
    local points_earned=$(echo "scale=1; $score * $test_weight" | bc -l 2>/dev/null || echo "0")
    local percentage=$(echo "scale=0; ($score * 100 + 0.5) / 1" | bc -l 2>/dev/null || echo "0")
    echo -e "${YELLOW}   Points Earned: ${points_earned}/${test_weight} (${percentage}%)${NC}"
    
    # Create detailed diff
    local diff_file="Results/diff_${test_num}.txt"
    diff -u --label "Expected Output" --label "Your Output" "$expected_file" "$student_output" > "$diff_file" 2>/dev/null

    if [[ -s "$diff_file" ]]; then
        echo -e "${CYAN}    Differences (- expected, + your output):${NC}"
        local line_count=0
        while IFS= read -r line && [[ $line_count -lt 10 ]]; do
            case "$line" in
                ---*|+++*|@@*)
                    continue ;;
                -*)
                    echo -e "${RED}   $line${NC}"
                    ((line_count++)) ;;
                +*)
                    echo -e "${GREEN}   $line${NC}"
                    ((line_count++)) ;;
                *)
                    if [[ $line_count -lt 8 ]]; then
                        echo "   $line"
                        ((line_count++))
                    fi ;;
            esac
        done < "$diff_file"
        
        if [[ $(wc -l < "$diff_file") -gt 500 ]]; then
            echo -e "${CYAN}   ... (output truncated, see Results/diff_${test_num}.txt for full diff)${NC}"
        fi
    fi
    
    # Show statistics
    local expected_lines=$(wc -l < "$expected_file" 2>/dev/null || echo "0")
    local student_lines=$(wc -l < "$student_output" 2>/dev/null || echo "0")
    
    echo -e "${CYAN}   Statistics: Expected=$expected_lines lines, Yours=$student_lines lines${NC}"
    echo
}

# =============================================================================
# VALIDATION FUNCTIONS
# =============================================================================
validate_test_weights() {
    local total_weight=0
    
    # Calculate sum of all weights
    for weight in "${TEST_WEIGHTS[@]}"; do
        total_weight=$((total_weight + weight))
    done
    
    if [[ $total_weight -ne $TOTAL_POINTS ]]; then
        print_error "Test weights sum to $total_weight, must equal $TOTAL_POINTS"
        echo "Current weights:"
        for test_num in "${!TEST_WEIGHTS[@]}"; do
            echo "  Test $test_num: ${TEST_WEIGHTS[$test_num]} points"
        done
        exit 1
    fi
    
    print_success "Test weights validation passed (total: $total_weight points)"
}

validate_test_files() {
    print_info "Validating test files..."
    
    # Check that test files exist for all defined weights
    for test_num in "${!TEST_WEIGHTS[@]}"; do
        local input_file="Testing/Testcases/input${test_num}.txt"
        local expected_file="Testing/Expected_Output/output${test_num}.txt"
        
        if [[ ! -f "$input_file" ]]; then
            print_error "Test case $test_num defined with weight ${TEST_WEIGHTS[$test_num]}, but $input_file not found"
            exit 1
        fi
        
        if [[ ! -f "$expected_file" ]]; then
            print_error "Expected output file $expected_file not found for test case $test_num"
            exit 1
        fi
    done
    
    print_success "All test files validated"
}

# =============================================================================
# MAIN GRADING FUNCTIONS
# =============================================================================

check_files() {
    print_info "Checking required files..."
    
    if [[ ! -f "$STUDENT_FILE" ]]; then
        print_error "$STUDENT_FILE not found!"
        echo "Make sure your code is in a file named exactly: $STUDENT_FILE"
        exit 1
    fi
    
    print_success "Found $STUDENT_FILE"
}

compile_code() {
    print_info "Compiling your code..."
    
    local makefile_to_use=""
    
    # Check if we should use student's Makefile
    if [[ "$USE_STUDENT_MAKEFILE" == "true" && -f "student_makefile_temp" ]]; then
        makefile_to_use="student_makefile_temp"
        print_info "Found student's Makefile ($STUDENT_MAKEFILE_NAME), using it to compile..."
    else
        # Use framework Makefile
        makefile_to_use=$(find_makefile)
        if [[ -n "$makefile_to_use" ]]; then
            print_success "Found $makefile_to_use to compile"
        fi
    fi
    
    if [[ -n "$makefile_to_use" ]]; then
        if make -f "$makefile_to_use" clean > /dev/null 2>&1 && make -f "$makefile_to_use" 2> Results/compile_errors.txt; then
            print_success "Code compiled successfully!"
        else
            print_error "Code failed to compile"
            echo
            echo "Compilation errors:"
            cat Results/compile_errors.txt | head -20
            echo
            echo "📊 FINAL GRADE: 0/$TOTAL_POINTS (Code must compile to earn points)"
            exit 1
        fi
    else
        print_error "No Makefile found!"
        echo "Please ensure a Makefile is present in the current directory."
        #echo "Supported names: Makefile, makefile, or any makefile* variant"
        echo "Supported name: Makefile"
        exit 1
    fi
    
    if [[ ! -x "$EXECUTABLE_NAME" ]]; then
        print_error "Executable $EXECUTABLE_NAME not created"
        exit 1
    fi
}

run_tests() {
    echo
    echo "🧪 Running test cases with proportional scoring..."
    
    # Get sorted list of test numbers from weights
    local test_numbers=($(printf '%s\n' "${!TEST_WEIGHTS[@]}" | sort -n))
    TOTAL_TESTS=${#test_numbers[@]}
    
    echo "Found $TOTAL_TESTS test case(s)"
    echo
    
    # Initialize test scores array
    for ((i=0; i<TOTAL_TESTS; i++)); do
        TEST_SCORES[i]=0
    done
    
    # Run each test
    local test_index=0
    for test_num in "${test_numbers[@]}"; do
        local input_file="Testing/Testcases/input${test_num}.txt"
        local expected_file="Testing/Expected_Output/output${test_num}.txt"
        local test_weight=${TEST_WEIGHTS[$test_num]}
        
        echo "🔬 Test $test_num (${test_weight} points):"
        
        # Run student's program
        local student_output="Results/student_output_${test_num}.txt"
        timeout "$TIMEOUT_SECONDS" ./"$EXECUTABLE_NAME" < "$input_file" > "$student_output" 2>/dev/null
        local exit_code=$?
        
        # Determine if program crashed
        local crashed=0
        if [[ $exit_code -eq 124 ]]; then
            print_error "Test $test_num TIMEOUT"
            crashed=1
        elif [[ $exit_code -gt 128 ]]; then
            print_error "Test $test_num CRASHED"
            crashed=1
        fi
        
        # Calculate proportional score for this test
        local score=$(calculate_proportional_score "$test_num" "$expected_file" "$student_output" "$crashed")
        TEST_SCORES[$test_index]=$score
        
        # Display result
        if [[ $crashed -eq 1 ]]; then
            echo "   ⚠️ Program crashed or timed out"
        elif [[ $(echo "$score == 1.0" | bc -l 2>/dev/null || echo "0") -eq 1 ]]; then
            print_success "Test $test_num PASSED (100%)"
        else
            local percentage=$(echo "scale=0; ($score * 100 + 0.5) / 1" | bc -l 2>/dev/null || echo "0")
            local points_earned=$(echo "scale=1; $score * $test_weight" | bc -l 2>/dev/null || echo "0")
            print_partial "Test $test_num PARTIAL (${percentage}% = ${points_earned}/${test_weight} points)"
            show_detailed_diff "$test_num" "$expected_file" "$student_output" "$score" "$test_weight"
        fi
        
        ((test_index++))
        echo
    done
}

calculate_final_grade() {
    local final_score=0
    local test_numbers=($(printf '%s\n' "${!TEST_WEIGHTS[@]}" | sort -n))
    
    # Calculate weighted score from all tests
    for ((i=0; i<${#test_numbers[@]}; i++)); do
        local test_num="${test_numbers[i]}"
        local test_weight=${TEST_WEIGHTS[$test_num]}
        local test_score=${TEST_SCORES[i]}
        
        if [[ -n "$test_score" && "$test_score" != "0" ]]; then
            local points=$(echo "scale=2; $test_score * $test_weight" | bc -l 2>/dev/null || echo "0")
            final_score=$(echo "scale=2; $final_score + $points" | bc -l 2>/dev/null || echo "$final_score")
        fi
    done
    
    # Round to nearest integer
    FINAL_SCORE=$(echo "scale=0; ($final_score + 0.5) / 1" | bc -l 2>/dev/null || echo "0")
}

generate_results() {
    calculate_final_grade
    
    local percentage=$((FINAL_SCORE * 100 / TOTAL_POINTS))
    
    echo "📊 GRADE SUMMARY"
    echo "=========================="
    echo "Tests:"
    
    local test_numbers=($(printf '%s\n' "${!TEST_WEIGHTS[@]}" | sort -n))
    for ((i=0; i<${#test_numbers[@]}; i++)); do
        local test_num="${test_numbers[i]}"
        local test_weight=${TEST_WEIGHTS[$test_num]}
        local test_score=${TEST_SCORES[i]}
        local points=$(echo "scale=1; $test_score * $test_weight" | bc -l 2>/dev/null || echo "0")
        local percentage=$(echo "scale=1; $test_score * 100" | bc -l 2>/dev/null || echo "0")
        printf "  Test %s: %.1f/%.0f points (%.1f%%)\n" "$test_num" "$points" "$test_weight" "$percentage"
    done
    
    echo "=========================="
    echo "FINAL GRADE: $FINAL_SCORE/$TOTAL_POINTS ($percentage%)"
    
    # Motivational messages
    local percentage_int=$(echo "scale=0; ($percentage + 0.5) / 1" | bc -l 2>/dev/null || echo "0")

    if [[ $percentage_int -ge 90 ]]; then
        echo -e "\n🎉 Excellent work!"
    elif [[ $percentage_int -ge 80 ]]; then
        echo -e "\n👍 Great job!"
    elif [[ $percentage_int -ge 70 ]]; then
        echo -e "\n✅ Good work!"
    elif [[ $percentage_int -ge 60 ]]; then
        echo -e "\n📚 Getting there - keep practicing!"
    else
        echo -e "\n💪 Don't give up - review the feedback and try again!"
    fi
}

export_csv() {
    local csv_file="Results/grade.csv"
    local zip_name="${STUDENT_ZIP_NAME:-Unknown}"
    
    # Create CSV file
    {
        echo "zip_filename,assignment_name,final_score"
        echo "${zip_name},${ASSIGNMENT_NAME},${FINAL_SCORE}"
    } > "$csv_file"
    
    #print_success "Grade exported to: $csv_file"
}

# =============================================================================
# MAIN EXECUTION
# =============================================================================

main() {
    echo "🎯 Assignment: $ASSIGNMENT_NAME"
    echo "📅 Student: ${STUDENT_ZIP_NAME:-Unknown}"
    echo "⏰ Submission: $(date '+%Y-%m-%d %H:%M:%S')"
    echo
    
    # # Show which Makefile is being used
    # if [[ "$USE_STUDENT_MAKEFILE" == "true" ]]; then
    #     echo "🔧 Using student's Makefile: $STUDENT_MAKEFILE_NAME"
    # else
    #     echo "🔧 Using framework Makefile (student did not submit Makefile)"
    # fi
    # echo
    
    # Create Results directory
    mkdir -p Results
    
    # Validate configuration
    validate_test_weights
    validate_test_files
    
    # Grade the submission
    check_files
    compile_code
    run_tests
    generate_results
    export_csv
    
    #echo "💾 Results saved to Results/ directory"
    echo
    
    exit 0
}

# Check if bc is available for calculations
if ! command -v bc &> /dev/null; then
    print_error "bc (calculator) not found - please install bc package"
    exit 1
fi

# Execute main function
main "$@"