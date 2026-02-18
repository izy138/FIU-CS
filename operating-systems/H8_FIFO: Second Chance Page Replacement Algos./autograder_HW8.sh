#!/bin/bash

# =============================================================================
# ASSIGNMENT SETTINGS (Edit these for each assignment)
# =============================================================================
ASSIGNMENT_NAME="HOMEWORK-8: FIFO and Second Chance Page Replacement"
TOTAL_POINTS=90

# Test case weights (must sum to exactly 90)
# FIFO: 45 points, Second Chance: 45 points
declare -A TEST_WEIGHTS=(
    ["fifo_simple"]=15                  # FIFO simple test
    ["fifo_moderate"]=15                # FIFO moderate test
    ["fifo_rigorous"]=15                # FIFO rigorous test
    ["second_chance_simple"]=15         # Second Chance simple test
    ["second_chance_moderate"]=15       # Second Chance moderate test
    ["second_chance_rigorous"]=15       # Second Chance rigorous test
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
EXECUTABLE_NAME="page_replacement"

# =============================================================================
# COLORS FOR OUTPUT
# =============================================================================
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

# Disable colors if output is redirected
if [[ ! -t 1 ]]; then
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    CYAN=''
    NC=''
fi

# =============================================================================
# INITIALIZE SCORES
# =============================================================================
TOTAL_TESTS=0
declare -A TEST_SCORES  # Associative array to store individual test scores

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
    local test_name=$1
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
    local test_name=$1
    local expected_file=$2
    local student_output=$3
    local score=$4
    local test_weight=$5
    
    echo -e "${CYAN}📋 Detailed Analysis for $test_name:${NC}"
    
    # Show score earned
    local points_earned=$(echo "scale=1; $score * $test_weight" | bc -l 2>/dev/null || echo "0")
    local percentage=$(echo "scale=0; ($score * 100 + 0.5) / 1" | bc -l 2>/dev/null || echo "0")
    echo -e "${YELLOW}   Points Earned: ${points_earned}/${test_weight} (${percentage}%)${NC}"
    
    # Create detailed diff
    local diff_file="Results/diff_${test_name}.txt"
    diff -u --label "Expected Output" --label "Your Output" "$expected_file" "$student_output" > "$diff_file" 2>/dev/null

    if [[ -s "$diff_file" ]]; then
        echo -e "${CYAN}   Differences (- expected, + your output):${NC}"
        local line_count=0
        while IFS= read -r line; do 
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
        
        if [[ $(wc -l < "$diff_file") -gt 50 ]]; then
            echo -e "${CYAN}   ... (output truncated, see Results/diff_${test_name}.txt for full diff)${NC}"
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
    local total=0
    for weight in "${TEST_WEIGHTS[@]}"; do
        total=$(echo "scale=2; $total + $weight" | bc -l 2>/dev/null || echo "$total")
    done
    
    local expected=90
    local diff=$(echo "scale=2; $total - $expected" | bc -l 2>/dev/null || echo "1")
    local abs_diff=$(echo "scale=2; if ($diff < 0) -$diff else $diff" | bc -l 2>/dev/null || echo "1")
    
    if [[ $(echo "$abs_diff > 0.01" | bc -l 2>/dev/null || echo "1") -eq 1 ]]; then
        print_error "Test weights sum to $total, expected $expected"
        exit 1
    fi
}

validate_test_files() {
    if [[ ! -d "Testing/Testcases" ]]; then
        print_error "Testing/Testcases directory not found!"
        exit 1
    fi
    
    # Check for test case files
    local test_files=("simple.txt" "moderate.txt" "rigorous.txt")
    for test_file in "${test_files[@]}"; do
        if [[ ! -f "Testing/Testcases/$test_file" ]]; then
            print_error "Testing/Testcases/$test_file not found!"
            exit 1
        fi
    done
    
    if [[ ! -d "Testing/Expected_Output" ]]; then
        print_error "Testing/Expected_Output directory not found!"
        exit 1
    fi
}

# =============================================================================
# COMPILATION
# =============================================================================

compile_code() {
    echo "🔨 COMPILATION"
    echo
    
    # Look for Makefile (instructor-provided)
    local makefile_found=$(find_makefile)
    
    if [[ -z "$makefile_found" ]]; then
        print_error "Makefile not found!"
        print_info "Expected: Makefile (provided by instructor)"
        exit 1
    fi
    
    print_info "Using makefile: $makefile_found"
    
    # Clean first
    make clean > /dev/null 2>&1
    
    # Build
    print_info "Compiling with: make"
    local compile_output
    compile_output=$(make 2>&1)
    local compile_status=$?
    
    if [[ $compile_status -ne 0 ]]; then
        print_error "Compilation failed!"
        echo "$compile_output"
        exit 1
    fi
    
    # Check if executable was created
    if [[ ! -f "$EXECUTABLE_NAME" ]]; then
        print_error "Executable '$EXECUTABLE_NAME' not created!"
        echo "$compile_output"
        exit 1
    fi
    
    print_success "Compilation successful!"
    echo
}

# =============================================================================
# TEST EXECUTION
# =============================================================================

run_test_case() {
    local algorithm=$1
    local test_level=$2
    local test_name="${algorithm}_${test_level}"
    local input_file="Testing/Testcases/${test_level}.txt"
    local expected_file="Testing/Expected_Output/${algorithm}_${test_level}_output.txt"
    local student_output="Results/student_${algorithm}_${test_level}.txt"
    
    print_info "Running ${algorithm^^} ${test_level^} Test..."
    timeout $TIMEOUT_SECONDS ./"$EXECUTABLE_NAME" "$input_file" "$algorithm" > "$student_output" 2>&1
    local exit_code=$?
    local crashed=0
    
    if [[ $exit_code -eq 124 ]]; then
        print_error "Test timed out (exceeded ${TIMEOUT_SECONDS}s)"
        TEST_SCORES[$test_name]=0
        crashed=1
    elif [[ $exit_code -ne 0 ]]; then
        print_error "Program crashed (exit code: $exit_code)"
        TEST_SCORES[$test_name]=0
        crashed=1
    fi
    
    if [[ $crashed -eq 0 ]]; then
        local score=$(calculate_proportional_score "$test_name" "$expected_file" "$student_output" $crashed)
        TEST_SCORES[$test_name]=$score
        
        if [[ "$score" == "0" || "$score" == "0.0" ]]; then
            print_error "${algorithm^^} ${test_level^} Test FAILED (0%)"
            show_detailed_diff "$test_name" "$expected_file" "$student_output" "$score" "${TEST_WEIGHTS[$test_name]}"
        elif [[ $(echo "$score == 1.0" | bc -l 2>/dev/null || echo "0") -eq 1 ]]; then
            print_success "${algorithm^^} ${test_level^} Test PASSED (100%)"
        else
            local percentage=$(echo "scale=0; ($score * 100 + 0.5) / 1" | bc -l 2>/dev/null || echo "0")
            local points_earned=$(echo "scale=1; $score * ${TEST_WEIGHTS[$test_name]}" | bc -l 2>/dev/null || echo "0")
            print_partial "${algorithm^^} ${test_level^} Test PARTIAL (${percentage}% = ${points_earned}/${TEST_WEIGHTS[$test_name]} points)"
            show_detailed_diff "$test_name" "$expected_file" "$student_output" "$score" "${TEST_WEIGHTS[$test_name]}"
        fi
    fi
    echo
}

run_functionality_tests() {
    echo "🧪 FUNCTIONALITY & CORRECTNESS TESTS"
    echo
    
    # Test FIFO algorithm
    echo "📝 FIFO Algorithm Tests:"
    echo
    run_test_case "fifo" "simple"
    run_test_case "fifo" "moderate"
    run_test_case "fifo" "rigorous"
    
    # Test Second Chance algorithm
    echo "📝 Second Chance Algorithm Tests:"
    echo
    run_test_case "second_chance" "simple"
    run_test_case "second_chance" "moderate"
    run_test_case "second_chance" "rigorous"
}

calculate_final_grade() {
    local final_score=0
    
    # Calculate weighted score from all tests
    for test_name in "${!TEST_WEIGHTS[@]}"; do
        local test_weight=${TEST_WEIGHTS[$test_name]}
        local test_score=${TEST_SCORES[$test_name]:-0}
        
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
    echo "FIFO Algorithm Tests:"
    
    local fifo_tests=("fifo_simple" "fifo_moderate" "fifo_rigorous")
    for test_name in "${fifo_tests[@]}"; do
        local test_weight=${TEST_WEIGHTS[$test_name]}
        local test_score=${TEST_SCORES[$test_name]:-0}
        local points=$(echo "scale=1; $test_score * $test_weight" | bc -l 2>/dev/null || echo "0")
        local test_percentage=$(echo "scale=1; $test_score * 100" | bc -l 2>/dev/null || echo "0")
        printf "  %-25s: %.1f/%.0f points (%.1f%%)\n" "$test_name" "$points" "$test_weight" "$test_percentage"
    done
    
    echo
    echo "Second Chance Algorithm Tests:"
    
    local sc_tests=("second_chance_simple" "second_chance_moderate" "second_chance_rigorous")
    for test_name in "${sc_tests[@]}"; do
        local test_weight=${TEST_WEIGHTS[$test_name]}
        local test_score=${TEST_SCORES[$test_name]:-0}
        local points=$(echo "scale=1; $test_score * $test_weight" | bc -l 2>/dev/null || echo "0")
        local test_percentage=$(echo "scale=1; $test_score * 100" | bc -l 2>/dev/null || echo "0")
        printf "  %-25s: %.1f/%.0f points (%.1f%%)\n" "$test_name" "$points" "$test_weight" "$test_percentage"
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
}

# =============================================================================
# MAIN EXECUTION
# =============================================================================

main() {
    echo "🎯 Assignment: $ASSIGNMENT_NAME"
    echo "📅 Student: ${STUDENT_ZIP_NAME:-Unknown}"
    echo "⏰ Submission: $(date '+%Y-%m-%d %H:%M:%S')"
    echo
    
    # Create Results directory
    mkdir -p Results
    
    # Validate configuration
    validate_test_weights
    validate_test_files
    
    # Grade the submission
    compile_code
    run_functionality_tests
    generate_results
    export_csv
    
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
