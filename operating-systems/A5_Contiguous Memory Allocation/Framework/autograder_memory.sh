#!/bin/bash

# ===============================================================================
# COP4610 BONUS Assignment - Contiguous Memory Allocation Autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# ===============================================================================

echo "=========================================================================="
echo "  COP4610 BONUS Assignment - Contiguous Memory Allocation Autograder"
echo "=========================================================================="
echo "Date: $(date)"
echo

set -euo pipefail

# Configuration
readonly STUDENT_EXEC="memory_allocator"
readonly TESTCASE_FILE="TESTCASES.txt"
readonly EXPECTED_OUTPUT="EXPECTED_OUTPUT.txt"
readonly STUDENT_OUTPUT="STUDENT_OUTPUT.txt"
readonly MAKEFILE="Makefile"
readonly TIMEOUT_SECONDS=10

# Color codes
readonly RED='\033[0;31m'
readonly YELLOW='\033[1;33m'
readonly GREEN='\033[0;32m'
readonly LIGHT_GREEN='\033[1;32m'
readonly BLUE='\033[0;34m'
readonly CYAN='\033[0;36m'
readonly NC='\033[0m'

# Score tracking
FIRST_FIT_SCORE=0
BEST_FIT_SCORE=0
WORST_FIT_SCORE=0
COALESCE_SCORE=0
COMPILATION_PENALTY=0
RUNTIME_PENALTY=0

readonly MAX_FIRST_FIT_POINTS=20
readonly MAX_BEST_FIT_POINTS=20
readonly MAX_WORST_FIT_POINTS=20
readonly MAX_COALESCE_POINTS=30
readonly TOTAL_AUTOGRADER_POINTS=90

#==============================================================================
# Utility Functions
#==============================================================================

log_error() {
    echo -e "${RED}ERROR: $1${NC}"
}

log_warning() {
    echo -e "${YELLOW}WARNING: $1${NC}"
}

log_info() {
    echo "INFO: $1"
}

log_success() {
    echo -e "${LIGHT_GREEN}SUCCESS: $1${NC}"
}

cleanup() {
    rm -f "$STUDENT_OUTPUT" compile_errors.txt runtime_errors.txt
    if [ -f "$MAKEFILE" ]; then
        make clean >/dev/null 2>&1 || true
    fi
}

trap cleanup EXIT

#==============================================================================
# Section Extraction Functions
#==============================================================================

extract_section_data() {
    local section_prefix="$1"
    local content="$2"
    
    if [[ -z "$content" ]]; then
        echo ""
        return
    fi
    
    # Extract lines starting with "STRATEGY: section_prefix" pattern
    # This extracts everything from that strategy section
    echo "$content" | awk -v section="$section_prefix" '
        BEGIN { in_section = 0 }
        /^=+$/ {
            next_line_check = 1
            next
        }
		next_line_check && /STRATEGY:/ {
			next_line_check = 0
			if ($2 == section) {
				in_section = 1
				next
			} else if (in_section) {
				in_section = 0  
			}
			next
		}
        next_line_check {
            next_line_check = 0
            next
        }
        /^=+$/ && in_section {
            next
        }
        in_section {
            print
        }
    ' 2>/dev/null || echo ""
}

compare_section_data() {
    local expected_data="$1"
    local student_data="$2"
    local section_name="$3"
    
    if [[ -z "$expected_data" && -z "$student_data" ]]; then
        echo "100"
        return
    fi
    
    if [[ -z "$expected_data" || -z "$student_data" ]]; then
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
    done <<< "$expected_data"
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            student_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
        fi
    done <<< "$student_data"
    
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
    local percentage=0
    if [[ $total_lines -gt 0 ]]; then
        percentage=$(( (correct_lines * 100) / total_lines ))
    fi
    
    echo "$percentage"
}

# show_section_differences() {
#     local expected_data="$1"
#     local student_data="$2"
#     local section_name="$3"
    
#     echo -e "${CYAN}   📋 Detailed Analysis for $section_name:${NC}"
    
#     # Just show the raw data side by side
#     echo "   Expected Output (first 20 lines):"
#     echo "$expected_data" | head -20 | sed 's/^/      /'
    
#     echo ""
#     echo "   Your Output (first 20 lines):"
#     echo "$student_data" | head -20 | sed 's/^/      /'
#     echo ""
# }

show_section_differences() {
    local expected_data="$1"
    local student_data="$2"
    local section_name="$3"
    
    echo -e "${CYAN}   📋 Detailed Analysis for $section_name:${NC}"
    #echo -e "   ${YELLOW}Line-by-line comparison (first 200 lines shown):${NC}"

    local -a expected_lines
    local -a student_lines

    while IFS= read -r line; do
        expected_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
    done <<< "$expected_data"

    while IFS= read -r line; do
        student_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
    done <<< "$student_data"

    local max_lines=$(( ${#expected_lines[@]} > ${#student_lines[@]} ? ${#expected_lines[@]} : ${#student_lines[@]} ))
    local display_lines=$(( max_lines < 2000 ? max_lines : 2000 ))

    for ((i=0; i<display_lines; i++)); do
        local expected="${expected_lines[i]:-}"
        local actual="${student_lines[i]:-}"

        if [[ "$expected" == "$actual" ]]; then
            echo -e "  ${GREEN}✅ Line $((i+1)): $actual${NC}"
        else
            echo -e "  ${RED}❌ Line $((i+1)):${NC}"
            echo -e "     ${YELLOW}Expected:${NC} $expected"
            echo -e "     ${CYAN}Actual:  ${NC} $actual"
        fi
    done
    echo ""
}


#==============================================================================
# Main Functions
#==============================================================================

check_files() {
    log_info "Checking required files..."
    
    local missing_files=()
    local required_files=("$MAKEFILE" "$TESTCASE_FILE" "$EXPECTED_OUTPUT" 
                         "first_fit.c" "best_fit.c" "worst_fit.c" 
                         "coalesce.c" "memory_allocator.h" "driver.c")
    
    for file in "${required_files[@]}"; do
        [ ! -f "$file" ] && missing_files+=("$file")
    done
    
    if [ ${#missing_files[@]} -ne 0 ]; then
        log_error "Missing files: ${missing_files[*]}"
        exit 1
    fi
    
    echo "All required files found"
}

compile_code() {
    log_info "Compiling student code..."
    
    if make 2>compile_errors.txt; then
        if [ -s compile_errors.txt ]; then
            log_warning "Compilation warnings detected"
            COMPILATION_PENALTY=2
        else
            echo "Compilation successful"
        fi
    else
        log_error "Compilation failed"
        cat compile_errors.txt
        exit 1
    fi
    
    if [ ! -x "$STUDENT_EXEC" ]; then
        log_error "Executable not created"
        exit 1
    fi
}

run_program() {
    log_info "Running student program..."
    
    if timeout "$TIMEOUT_SECONDS" ./"$STUDENT_EXEC" "$TESTCASE_FILE" > "$STUDENT_OUTPUT" 2>runtime_errors.txt; then
        echo "Program executed successfully"
        if [ -s runtime_errors.txt ]; then
            log_warning "Runtime warnings detected"
            RUNTIME_PENALTY=1
        fi
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            log_error "Program timed out"
            RUNTIME_PENALTY=5
        else
            log_error "Program crashed"
            RUNTIME_PENALTY=10
        fi
    fi
}

grade_strategies() {
    log_info "Grading memory allocation strategies..."
    
    # Cache file contents
    local expected_content=""
    local student_content=""
    
    if [[ -f "$EXPECTED_OUTPUT" ]]; then
        expected_content=$(cat "$EXPECTED_OUTPUT")
    fi
    
    if [[ -f "$STUDENT_OUTPUT" ]]; then
        student_content=$(cat "$STUDENT_OUTPUT")
    fi
    
    echo ""
    echo "🔬 Testing Memory Allocation Algorithms..."
    echo ""
    
    # Define strategies with their max points
    local strategies=("FIRST_FIT:$MAX_FIRST_FIT_POINTS" "BEST_FIT:$MAX_BEST_FIT_POINTS" "WORST_FIT:$MAX_WORST_FIT_POINTS")
    
    for strategy_info in "${strategies[@]}"; do
        local strategy=$(echo "$strategy_info" | cut -d':' -f1)
        local max_points=$(echo "$strategy_info" | cut -d':' -f2)
        
        echo "   🧪 Testing $strategy..."
        
        # Extract strategy data
        local expected_data=$(extract_section_data "$strategy" "$expected_content")
        local student_data=$(extract_section_data "$strategy" "$student_content")
        
        # Compare data
        local match_percentage=0
        if [[ -n "$expected_data" && -n "$student_data" ]]; then
            match_percentage=$(compare_section_data "$expected_data" "$student_data" "$strategy")
        elif [[ -z "$expected_data" && -z "$student_data" ]]; then
            match_percentage=100
        fi
        
        # Calculate points
        local points=0
        if [[ "$match_percentage" =~ ^[0-9]+$ ]]; then
            points=$(( (max_points * match_percentage) / 100 ))
        fi
        
        # Apply penalties
        if [[ $COMPILATION_PENALTY -gt 0 && $points -gt $COMPILATION_PENALTY ]]; then
            points=$((points - COMPILATION_PENALTY))
        elif [[ $COMPILATION_PENALTY -gt 0 ]]; then
            points=0
        fi
        
        if [[ $RUNTIME_PENALTY -gt 0 && $points -gt $RUNTIME_PENALTY ]]; then
            points=$((points - RUNTIME_PENALTY))
        elif [[ $RUNTIME_PENALTY -gt 0 ]]; then
            points=0
        fi
        
        # Store scores
        case $strategy in
            "FIRST_FIT") FIRST_FIT_SCORE=$points ;;
            "BEST_FIT") BEST_FIT_SCORE=$points ;;
            "WORST_FIT") WORST_FIT_SCORE=$points ;;
        esac
        
        # Show results
        if [[ $match_percentage -eq 100 ]]; then
            log_success "$strategy: Perfect match ($points/$max_points points)"
        else
            log_warning "$strategy: $match_percentage% match ($points/$max_points points)"
            show_section_differences "$expected_data" "$student_data" "$strategy"
        fi
        
        echo ""
    done
    
    # Coalescing score based on average
    local avg_percentage=$(( (FIRST_FIT_SCORE * 100 / MAX_FIRST_FIT_POINTS + 
                              BEST_FIT_SCORE * 100 / MAX_BEST_FIT_POINTS + 
                              WORST_FIT_SCORE * 100 / MAX_WORST_FIT_POINTS) / 3 ))
    COALESCE_SCORE=$(( (MAX_COALESCE_POINTS * avg_percentage) / 100 ))
    
    echo "   🧪 Memory Coalescing: $COALESCE_SCORE/$MAX_COALESCE_POINTS points"
}

generate_report() {
    local total_score=$((FIRST_FIT_SCORE + BEST_FIT_SCORE + WORST_FIT_SCORE + COALESCE_SCORE))
    local percentage=0
    if [[ $TOTAL_AUTOGRADER_POINTS -gt 0 ]]; then
        percentage=$(( (total_score * 100) / TOTAL_AUTOGRADER_POINTS ))
    fi
    
    echo ""
    echo "=================================================================="
    echo "              AUTOGRADER REPORT"
    echo "=================================================================="
    echo "Date: $(date)"   
    echo ""
    echo "COMPILATION & RUNTIME:"
    echo "Compilation: $([ $COMPILATION_PENALTY -eq 0 ] && echo "SUCCESS" || echo "WARNINGS")"
    echo "Execution: $([ $RUNTIME_PENALTY -eq 0 ] && echo "SUCCESS" || echo "ISSUES")"
    echo ""
    echo "MEMORY ALLOCATION IMPLEMENTATION SCORES:"
    echo "--------------------------------------------------------------"
    printf "First-Fit Algorithm:          %d/%d points (%d%%)\n" $FIRST_FIT_SCORE $MAX_FIRST_FIT_POINTS $((FIRST_FIT_SCORE * 100 / MAX_FIRST_FIT_POINTS))
    printf "Best-Fit Algorithm:           %d/%d points (%d%%)\n" $BEST_FIT_SCORE $MAX_BEST_FIT_POINTS $((BEST_FIT_SCORE * 100 / MAX_BEST_FIT_POINTS))
    printf "Worst-Fit Algorithm:          %d/%d points (%d%%)\n" $WORST_FIT_SCORE $MAX_WORST_FIT_POINTS $((WORST_FIT_SCORE * 100 / MAX_WORST_FIT_POINTS))
    printf "Memory Coalescing:            %d/%d points (%d%%)\n" $COALESCE_SCORE $MAX_COALESCE_POINTS $((COALESCE_SCORE * 100 / MAX_COALESCE_POINTS))
    echo "--------------------------------------------------------------"
    echo "AUTOGRADER TOTAL:             $total_score/$TOTAL_AUTOGRADER_POINTS points ($percentage%)"
    echo ""    
    echo "--------------------------------------------------------------"
    echo "Remaining 10% is manually graded by the instructor based on:"
    echo "• Clear code structure and organization"
    echo "• Meaningful comments and documentation"
    echo "• Well-written README file"
    echo "• Overall code readability and style"    
    echo "--------------------------------------------------------------"    
    
    if [ $percentage -ge 90 ]; then
        echo "🎉 Autograder Performance: Excellent (A range)"
    elif [ $percentage -ge 80 ]; then
        echo "🌟 Autograder Performance: Good (B range)"
    elif [ $percentage -ge 70 ]; then
        echo "👍 Autograder Performance: Acceptable (C range)"
    elif [ $percentage -ge 60 ]; then
        log_warning "Autograder Performance: Below expectations (D range)"
    else
        log_warning "❌ Autograder Performance: Significant issues (F range)"
    fi
    echo "==================================================================" 
    echo ""
}

#==============================================================================
# Main Execution
#==============================================================================

main() {
    echo "Starting Contiguous Memory Allocation Autograder"
    echo "Grading Rubric: First-Fit(20) + Best-Fit(20) + Worst-Fit(20) + Coalescing(30) = 90 points"
    echo "================================================"
    
    check_files
    compile_code
    run_program
    grade_strategies
    generate_report
    
    exit 0
}

# Execute main function
main "$@"
