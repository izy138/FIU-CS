#!/bin/bash

# ===============================================================================
# COP4610 Assignment 3 - CPU Scheduling Algorithms Autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# ===============================================================================

echo "=========================================================================="
echo "     COP4610 Assignment 3 - CPU Scheduling Algorithms Autograder           "
echo "=========================================================================="
echo "Date: $(date)"
echo

set -euo pipefail

# Configuration
readonly STUDENT_EXEC="scheduler"
readonly TESTCASE_FILE="Testing/Testcases/input1.txt"
readonly EXPECTED_OUTPUT="Testing/Expected_output/output1.txt"
readonly STUDENT_OUTPUT="STUDENT_OUTPUT.txt"
readonly MAKEFILE="Makefile"
readonly TIMEOUT_SECONDS=10

# Color codes
readonly RED='\033[0;31m'
readonly YELLOW='\033[1;33m'
readonly GREEN='\033[0;32m'
readonly LIGHT_GREEN='\033[1;32m'
readonly BLUE='\033[0;34m'
readonly NC='\033[0m'

# Score tracking - Initialize properly
FCFS_SCORE=0
RR_SCORE=0
SJF_SCORE=0
SRT_SCORE=0
PRIORITY_NP_SCORE=0
PRIORITY_P_SCORE=0
COMPILATION_PENALTY=0
RUNTIME_PENALTY=0

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
    rm -f expected_*.txt student_*.txt diff_*.txt
    if [ -f "$MAKEFILE" ]; then
        make clean >/dev/null 2>&1 || true
    fi
}

trap cleanup EXIT

#==============================================================================
# Section Extraction Functions
#==============================================================================

extract_algorithm_output() {
    local algorithm_name="$1"
    local content="$2"
    
    if [[ -z "$content" ]]; then
        echo ""
        return
    fi
    
    # Extract output for specific algorithm between separator lines
    echo "$content" | awk -v algo="$algorithm_name" '
        /^============================================$/ { 
            if (found) exit;
            getline;
            if ($0 == algo) {
                found=1;
                print $0;
                next;
            }
        }
        found { print }
    ' 2>/dev/null || echo ""
}

compare_algorithm_output() {
    local expected_data="$1"
    local student_data="$2"
    local algorithm_name="$3"
    
    if [[ -z "$expected_data" && -z "$student_data" ]]; then
        echo "100"
        return
    fi
    
    if [[ -z "$expected_data" || -z "$student_data" ]]; then
        echo "0"
        return
    fi
    
    # Normalize whitespace and compare line by line
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

show_algorithm_differences() {
    local expected_data="$1"
    local student_data="$2"
    local algorithm_name="$3"
    
    echo "   Expected Output for $algorithm_name:"
    if [[ -n "$expected_data" ]]; then
        echo "$expected_data" | sed 's/^/      /'
    else
        echo "      (No expected output)"
    fi
    
    echo "   Your Output for $algorithm_name:"
    if [[ -n "$student_data" ]]; then
        # Create arrays for line-by-line comparison
        local -a expected_lines
        local -a student_lines
        
        # Read expected lines into array
        while IFS= read -r line; do
            if [[ -n "$line" ]]; then
                expected_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
            fi
        done <<< "$expected_data"
        
        # Read student lines into array
        while IFS= read -r line; do
            if [[ -n "$line" ]]; then
                student_lines+=("$(echo "$line" | tr -s ' ' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')")
            fi
        done <<< "$student_data"
        
        # Compare line by line by position
        for ((i=0; i<${#student_lines[@]}; i++)); do
            local student_line="${student_lines[i]}"
            local mark="❌"
            
            # Check if this line matches the expected line at same position
            if [[ i -lt ${#expected_lines[@]} && "$student_line" == "${expected_lines[i]}" ]]; then
                mark="✓"
            fi
            
            echo "      $mark $student_line"
        done
        
        # Show if student output is missing lines
        if [[ ${#student_lines[@]} -lt ${#expected_lines[@]} ]]; then
            echo "      ❌ (Missing lines...)"
        fi
    else
        echo "      (No output generated)"
    fi
    echo ""
}

#==============================================================================
# Main Functions
#==============================================================================

check_files() {
    log_info "Checking required files..."
    
    local missing_files=()
    local required_files=("$TESTCASE_FILE" "$EXPECTED_OUTPUT" "$MAKEFILE" 
                         "CPU_scheduler.h" "driver.c")
    
    for file in "${required_files[@]}"; do
        [ ! -f "$file" ] && missing_files+=("$file")
    done
    
    if [ ${#missing_files[@]} -ne 0 ]; then
        log_error "Missing framework files: ${missing_files[*]}"
        exit 1
    fi
    
    echo "All required framework files found"
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
    log_info "Running student program with test cases..."
    
    if timeout "$TIMEOUT_SECONDS" ./"$STUDENT_EXEC" < "$TESTCASE_FILE" > "$STUDENT_OUTPUT" 2>runtime_errors.txt; then
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
        # Continue with partial grading
    fi
}

grade_scheduling_algorithms() {
    log_info "Grading individual scheduling algorithms..."
    
    # Cache file contents once instead of reading repeatedly
    local expected_content=""
    local student_content=""
    
    if [[ -f "$EXPECTED_OUTPUT" ]]; then
        expected_content=$(cat "$EXPECTED_OUTPUT")
    fi
    
    if [[ -f "$STUDENT_OUTPUT" ]]; then
        student_content=$(cat "$STUDENT_OUTPUT")
    fi
    
    echo ""
    echo "📊 Testing Scheduling Algorithms..."
    echo ""
    
    # Define algorithms with their max points
    local algorithms=("First-Come-First-Served (FCFS):15" "Round-Robin (RR):15" "Shortest-Job-First (SJF):15" "Shortest-Remaining_Time-First (SRTF):15" "PRIORITY_NON_PREEMPTIVE:15" "PRIORITY_PREEMPTIVE_WITH_RR:15")
    
    for algo_info in "${algorithms[@]}"; do
        local algo=$(echo "$algo_info" | cut -d':' -f1)
        local max_points=$(echo "$algo_info" | cut -d':' -f2)
        
        echo "   Testing $algo..."
        
        # Extract algorithm output
        local expected_data=$(extract_algorithm_output "$algo" "$expected_content")
        local student_data=$(extract_algorithm_output "$algo" "$student_content")
        
        # Count lines for debugging
        local expected_count=$(echo "$expected_data" | grep -c "." 2>/dev/null || echo "0")
        local student_count=$(echo "$student_data" | grep -c "." 2>/dev/null || echo "0")
        
        echo "   Lines found: Expected=$expected_count, Student=$student_count"
        
        # Compare data
        local match_percentage=0
        if [[ -n "$expected_data" && -n "$student_data" ]]; then
            match_percentage=$(compare_algorithm_output "$expected_data" "$student_data" "$algo")
        elif [[ -z "$expected_data" && -z "$student_data" ]]; then
            match_percentage=100
        fi
        
        # Calculate points (ensure it's a valid number)
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
        
        # To this:
		case $algo in
			"First-Come-First-Served (FCFS)") FCFS_SCORE=$points ;;
			"Round-Robin (RR)") RR_SCORE=$points ;;
			"Shortest-Job-First (SJF)") SJF_SCORE=$points ;;
			"Shortest-Remaining_Time-First (SRTF)") SRT_SCORE=$points ;;
			"PRIORITY_NON_PREEMPTIVE") PRIORITY_NP_SCORE=$points ;;
			"PRIORITY_PREEMPTIVE_WITH_RR") PRIORITY_P_SCORE=$points ;;
		esac
        
        # Show results
        if [[ $match_percentage -eq 100 ]]; then
            log_success "$algo: Perfect match ($points/$max_points points)"
        else
            log_warning "$algo: $match_percentage% match ($points/$max_points points)"
            show_algorithm_differences "$expected_data" "$student_data" "$algo"
        fi
    done
}

generate_report() {
    local total_score=$((FCFS_SCORE + RR_SCORE + SJF_SCORE + SRT_SCORE + PRIORITY_NP_SCORE + PRIORITY_P_SCORE))
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
    echo "SCHEDULING ALGORITHM IMPLEMENTATION SCORES:"
    echo "--------------------------------------------------------------"
    echo "First-Come First-Served:      $FCFS_SCORE/15 points ($((FCFS_SCORE * 100 / 15))%)"
    echo "Round Robin:                  $RR_SCORE/15 points ($((RR_SCORE * 100 / 15))%)"
    echo "Shortest Job First:           $SJF_SCORE/15 points ($((SJF_SCORE * 100 / 15))%)"
    echo "Shortest Remaining Time:      $SRT_SCORE/15 points ($((SRT_SCORE * 100 / 15))%)"
    echo "Priority Non-Preemptive:      $PRIORITY_NP_SCORE/15 points ($((PRIORITY_NP_SCORE * 100 / 15))%)"
    echo "Priority Preemptive with RR:  $PRIORITY_P_SCORE/15 points ($((PRIORITY_P_SCORE * 100 / 15))%)"
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
    # Reset all scores for this run
    unset FCFS_SCORE RR_SCORE SJF_SCORE SRT_SCORE PRIORITY_NP_SCORE PRIORITY_P_SCORE COMPILATION_PENALTY RUNTIME_PENALTY
    FCFS_SCORE=0
    RR_SCORE=0  
    SJF_SCORE=0
    SRT_SCORE=0
    PRIORITY_NP_SCORE=0
    PRIORITY_P_SCORE=0
    COMPILATION_PENALTY=0
    RUNTIME_PENALTY=0
    
    echo "Starting CPU Scheduling Algorithms Autograder"
    echo "Grading Rubric: FCFS(15) + RR(15) + SJF(15) + SRT(15) + Priority_NP(15) + Priority_P(15) = 90 points"
    echo "================================================"
    
    check_files
    compile_code
    run_program
    grade_scheduling_algorithms
    generate_report
    
    # Return appropriate exit code
    local total_score=$((FCFS_SCORE + RR_SCORE + SJF_SCORE + SRT_SCORE + PRIORITY_NP_SCORE + PRIORITY_P_SCORE))
    local percentage=$(( (total_score * 100) / TOTAL_AUTOGRADER_POINTS ))
    
    exit 0
}

# Execute main function
main "$@"