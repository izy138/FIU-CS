#!/bin/bash

# ===============================================================================
# COP4610 Assignment 4 - Banker's Algorithm Implementation Autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# ===============================================================================

# REMOVED: set -euo pipefail (this was causing termination issues)
# Use more lenient error handling for autograder

# Configuration
readonly STUDENT_EXEC="banker"
readonly TESTCASE_FILE="INPUT.txt"
readonly EXPECTED_OUTPUT="EXPECTED_OUTPUT.txt"
readonly STUDENT_OUTPUT="STUDENT_OUTPUT.txt"
readonly MAKEFILE="makefile"
readonly TIMEOUT_SECONDS=10          # Handling students infinite code 
readonly COMPILATION_TIMEOUT=12      # Compilation timeout
readonly MAX_OUTPUT_SIZE=2097152     # 2MB max output size

# Required student files (4 separate implementations)
readonly REQUIRED_STUDENT_FILES=("need.c" "available.c" "safety.c" "request.c")

# Color codes
if [[ -t 1 ]] && command -v tput >/dev/null 2>&1 && tput colors >/dev/null 2>&1; then
    readonly RED='\033[0;31m'
    readonly YELLOW='\033[1;33m'
    readonly GREEN='\033[0;32m'
    readonly LIGHT_GREEN='\033[1;32m'
    readonly BLUE='\033[0;34m'
    readonly NC='\033[0m'
else
    # No colors if terminal doesn't support them
    readonly RED=''
    readonly YELLOW=''
    readonly GREEN=''
    readonly LIGHT_GREEN=''
    readonly BLUE=''
    readonly NC=''
fi

# Score tracking - Updated point distribution
COMPILATION_SCORE=0
SAFETY_CHECK_SCORE=0
VALID_REQUESTS_SCORE=0
INVALID_REQUESTS_SCORE=0
EDGE_CASES_SCORE=0

readonly MAX_COMPILATION_POINTS=0
readonly MAX_SAFETY_POINTS=30
readonly MAX_VALID_REQUEST_POINTS=20
readonly MAX_INVALID_REQUEST_POINTS=20
readonly MAX_EDGE_CASE_POINTS=20
readonly TOTAL_POINTS=90

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
    echo -e "${LIGHT_GREEN}         SUCCESS: $1${NC}"
}

# FIXED: More careful cleanup that won't terminate the script
cleanup() {
    # Clean up files but be more careful about process killing
    rm -f "$STUDENT_OUTPUT" compile_errors.txt runtime_errors.txt 2>/dev/null || true
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null || true
    
    # Only kill banker processes if they actually exist and are running
    if [ -f "$STUDENT_EXEC" ] && pgrep -x "$STUDENT_EXEC" >/dev/null 2>&1; then
        pkill -x "$STUDENT_EXEC" 2>/dev/null || true
        sleep 0.5
    fi
    
    # Ensure we start with a clean build
    if [ -f "Makefile" ]; then
        make clean >/dev/null 2>&1 || true
    fi  
}

# FIXED: Only trap on specific signals, not EXIT
trap cleanup INT TERM

#==============================================================================
# File Checking Functions
#==============================================================================

check_required_files() {
    local missing_files=()
    local required_infrastructure=("SYSTEM_STATE.txt" "$EXPECTED_OUTPUT" "$MAKEFILE" 
                                 "banker_header.h" "banker_driver.c")
    
    # Check infrastructure files
    for file in "${required_infrastructure[@]}"; do
        [ ! -f "$file" ] && missing_files+=("$file")
    done
    
    if [ ${#missing_files[@]} -ne 0 ]; then
        log_error "Missing infrastructure files: ${missing_files[*]}"
        exit 1
    fi
}

check_student_files() {
    local missing_files=()
    local empty_files=()
    local found_files=()
    
    for file in "${REQUIRED_STUDENT_FILES[@]}"; do
        if [ ! -f "$file" ]; then
            missing_files+=("$file")
        elif [ ! -s "$file" ]; then
            empty_files+=("$file")
        else
            found_files+=("$file")
        fi
    done
    
    # Report findings
    if [ ${#missing_files[@]} -ne 0 ]; then
        log_error "Missing student files: ${missing_files[*]}"
        echo "Found files: ${found_files[*]}"
    else
        echo "All required student files found: ${found_files[*]}"
    fi
    
    if [ ${#empty_files[@]} -ne 0 ]; then
        log_warning "Empty files detected: ${empty_files[*]}"
    fi
    
    # Continue compilation even with missing files (for partial credit)
    return 0
}

#==============================================================================
# Section Extraction Functions
#==============================================================================

extract_section_lines() {
    local pattern="$1"
    local file="$2"
    
    if [[ ! -f "$file" ]]; then
        echo ""
        return
    fi
    
    grep "$pattern" "$file" 2>/dev/null || echo ""
}

compare_safety_checks() {
    local expected_file="$1"
    local student_file="$2"
    
    local expected_safety=$(extract_section_lines "SAFETY_CHECK:" "$expected_file")
    local student_safety=$(extract_section_lines "SAFETY_CHECK:" "$student_file")
    
    if [[ -z "$expected_safety" && -z "$student_safety" ]]; then
        echo "100"
        return
    fi
    
    if [[ -z "$expected_safety" || -z "$student_safety" ]]; then
        echo "0"
        return
    fi
    
    # Convert to arrays for line-by-line comparison
    local -a expected_lines
    local -a student_lines
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            expected_lines+=("$(echo "$line" | tr -s ' ')")
        fi
    done <<< "$expected_safety"
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            student_lines+=("$(echo "$line" | tr -s ' ')")
        fi
    done <<< "$student_safety"
    
    # Line-by-line positional comparison
    local total_lines=${#expected_lines[@]}
    local correct_count=0
    
    for ((i=0; i<total_lines; i++)); do
        if [[ i -lt ${#student_lines[@]} && "${expected_lines[i]}" == "${student_lines[i]}" ]]; then
            ((correct_count++))
        fi
    done
    
    # Penalize extra lines in student output
    if [[ ${#student_lines[@]} -gt $total_lines ]]; then
        local penalty=$(( ${#student_lines[@]} - total_lines ))
        correct_count=$(( correct_count > penalty ? correct_count - penalty : 0 ))
    fi
    
    # Calculate percentage
    if [[ $total_lines -eq 0 ]]; then
        echo "0"
    else
        echo $(( (correct_count * 100) / total_lines ))
    fi
}

compare_request_results() {
    local pattern="$1"
    local expected_file="$2"
    local student_file="$3"
    
    local expected_results=$(extract_section_lines "$pattern" "$expected_file")
    local student_results=$(extract_section_lines "$pattern" "$student_file")
    
    if [[ -z "$expected_results" && -z "$student_results" ]]; then
        echo "100"
        return
    fi
    
    if [[ -z "$expected_results" || -z "$student_results" ]]; then
        echo "0"
        return
    fi
    
    # Convert to arrays for line-by-line comparison
    local -a expected_lines
    local -a student_lines
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            expected_lines+=("$(echo "$line" | tr -s ' ')")
        fi
    done <<< "$expected_results"
    
    while IFS= read -r line; do
        if [[ -n "$line" ]]; then
            student_lines+=("$(echo "$line" | tr -s ' ')")
        fi
    done <<< "$student_results"
    
    # Line-by-line positional comparison
    local total_lines=${#expected_lines[@]}
    local correct_count=0
    
    for ((i=0; i<total_lines; i++)); do
        if [[ i -lt ${#student_lines[@]} && "${expected_lines[i]}" == "${student_lines[i]}" ]]; then
            ((correct_count++))
        fi
    done
    
    # Penalize extra lines in student output
    if [[ ${#student_lines[@]} -gt $total_lines ]]; then
        local penalty=$(( ${#student_lines[@]} - total_lines ))
        correct_count=$(( correct_count > penalty ? correct_count - penalty : 0 ))
    fi
    
    # Calculate percentage
    if [[ $total_lines -eq 0 ]]; then
        echo "0"
    else
        echo $(( (correct_count * 100) / total_lines ))
    fi
}

compare_system_state() {
    local expected_file="$1"
    local student_file="$2"
    
    local expected_state=$(extract_section_lines "SYSTEM_STATE:" "$expected_file")
    local student_state=$(extract_section_lines "SYSTEM_STATE:" "$student_file")
    
    if [[ -z "$expected_state" && -z "$student_state" ]]; then
        echo "100"
        return
    fi
    
    if [[ -z "$expected_state" || -z "$student_state" ]]; then
        echo "0"
        return
    fi
    
    if [[ "$expected_state" == "$student_state" ]]; then
        echo "100"
    else
        echo "50"
    fi
}

#==============================================================================
# Main Functions
#==============================================================================

compile_code() {
    echo "INFO: Compiling student program..."  
    if timeout "$COMPILATION_TIMEOUT" make > /dev/null 2>compile_errors.txt; then
        if [ -s compile_errors.txt ]; then
            log_warning "Compilation warnings detected"
            cat compile_errors.txt
        else
            echo "   👍 Compilation successful"
        fi
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            log_error "⚠️ Compilation timed out (${COMPILATION_TIMEOUT}s limit)"
        else
            log_error "⚠️ Compilation failed"
        fi
        if [ -f compile_errors.txt ]; then
            cat compile_errors.txt
        fi
        exit 1
    fi
    
    if [ ! -x "$STUDENT_EXEC" ]; then
        log_error "⚠️ Executable not created"
        exit 1
    fi
}

run_program() {
    echo "INFO: Executing student program..."
    if timeout "$TIMEOUT_SECONDS" ./"$STUDENT_EXEC" > "$STUDENT_OUTPUT" 2>runtime_errors.txt; then
        echo "   👍 Execution successful"
        if [ -s runtime_errors.txt ]; then
            log_warning "⚠️ Runtime warnings detected"
        fi
    else
        local exit_code=$?
        if [ $exit_code -eq 124 ]; then
            log_error "⚠️ Program timed out"
        else
            log_error "⚠️ Program crashed"
        fi
    fi

    check_output_size
}

check_output_size() {
    if [ -f "$STUDENT_OUTPUT" ]; then
        local size=$(stat -f%z "$STUDENT_OUTPUT" 2>/dev/null || stat -c%s "$STUDENT_OUTPUT" 2>/dev/null || echo 0)
        if [ "$size" -gt "$MAX_OUTPUT_SIZE" ]; then
            log_warning "Output file too large (${size} bytes), truncating for analysis..."
            head -c "$MAX_OUTPUT_SIZE" "$STUDENT_OUTPUT" > "${STUDENT_OUTPUT}.tmp"
            mv "${STUDENT_OUTPUT}.tmp" "$STUDENT_OUTPUT"
            echo "" >> "$STUDENT_OUTPUT"
            echo "--- OUTPUT TRUNCATED DUE TO SIZE LIMIT ---" >> "$STUDENT_OUTPUT"
        fi
    fi
}

grade_algorithm() {
    log_info "Grading Banker's Algorithm implementation..."    
    # Grade Safety Checks
    echo "   🧪 Testing Safety Check Algorithm (safety.c)..."
    local safety_percentage=$(compare_safety_checks "$EXPECTED_OUTPUT" "$STUDENT_OUTPUT")
    SAFETY_CHECK_SCORE=$(( (MAX_SAFETY_POINTS * safety_percentage) / 100 ))
    
    if [[ $safety_percentage -eq 100 ]]; then
        log_success "Safety Check: Perfect ($SAFETY_CHECK_SCORE/$MAX_SAFETY_POINTS points)"
    else
        log_warning "Safety Check: $safety_percentage% correct ($SAFETY_CHECK_SCORE/$MAX_SAFETY_POINTS points)"
    fi
    
    # Grade Valid Requests
    echo "   🧪 Testing Valid Request Handling (request.c)..."
    local valid_percentage=$(compare_request_results "REQUEST_RESULT: GRANTED" "$EXPECTED_OUTPUT" "$STUDENT_OUTPUT")
    VALID_REQUESTS_SCORE=$(( (MAX_VALID_REQUEST_POINTS * valid_percentage) / 100 ))
    
    if [[ $valid_percentage -eq 100 ]]; then
        log_success "Valid Requests: Perfect ($VALID_REQUESTS_SCORE/$MAX_VALID_REQUEST_POINTS points)"
    else
        log_warning "Valid Requests: $valid_percentage% correct ($VALID_REQUESTS_SCORE/$MAX_VALID_REQUEST_POINTS points)"
    fi
    
    # Grade Invalid Requests
    echo "   🧪 Testing Invalid Request Handling (request.c)..."
    local invalid_percentage=$(compare_request_results "REQUEST_RESULT: DENIED" "$EXPECTED_OUTPUT" "$STUDENT_OUTPUT")
    INVALID_REQUESTS_SCORE=$(( (MAX_INVALID_REQUEST_POINTS * invalid_percentage) / 100 ))
    
    if [[ $invalid_percentage -eq 100 ]]; then
        log_success "Invalid Requests: Perfect ($INVALID_REQUESTS_SCORE/$MAX_INVALID_REQUEST_POINTS points)"
    else
        log_warning "Invalid Requests: $invalid_percentage% correct ($INVALID_REQUESTS_SCORE/$MAX_INVALID_REQUEST_POINTS points)"
    fi
    
    # Grade Edge Cases
    echo "   🧪 Testing Edge Cases..."
    local edge_percentage=$(compare_request_results "REQUEST_RESULT: ERROR" "$EXPECTED_OUTPUT" "$STUDENT_OUTPUT")
    local zero_req_percentage=$(compare_request_results "REQUEST: T.*\[0,0,0\]" "$EXPECTED_OUTPUT" "$STUDENT_OUTPUT")
    edge_percentage=$(( (edge_percentage + zero_req_percentage) / 2 ))
    
    EDGE_CASES_SCORE=$(( (MAX_EDGE_CASE_POINTS * edge_percentage) / 100 ))
    
    if [[ $edge_percentage -eq 100 ]]; then
        log_success "Edge Cases: Perfect ($EDGE_CASES_SCORE/$MAX_EDGE_CASE_POINTS points)"
    else
        log_warning "Edge Cases: $edge_percentage% correct ($EDGE_CASES_SCORE/$MAX_EDGE_CASE_POINTS points)"
    fi
}

generate_report() {
    local total_score=$((COMPILATION_SCORE + SAFETY_CHECK_SCORE + VALID_REQUESTS_SCORE + INVALID_REQUESTS_SCORE + EDGE_CASES_SCORE))
    local percentage=$(( (total_score * 100) / TOTAL_POINTS ))
    
    echo ""
    echo "=================================================================="
    echo "📋 BANKER'S ALGORITHM AUTOGRADER REPORT"
    echo "=================================================================="   
    echo "Date: $(date)"
    echo "Student: $(whoami)"  
    
    # Show individual file status
    for file in "${REQUIRED_STUDENT_FILES[@]}"; do
        if [ ! -f "$file" ]; then
            echo "❌ $file: Missing implementation"
        fi
    done
   
    echo "------------------------------------------------------------------"
    echo "Safety Algorithm:          $SAFETY_CHECK_SCORE/$MAX_SAFETY_POINTS points"
    echo "Valid Request Processing:  $VALID_REQUESTS_SCORE/$MAX_VALID_REQUEST_POINTS points"
    echo "Invalid Request Processing: $INVALID_REQUESTS_SCORE/$MAX_INVALID_REQUEST_POINTS points"
    echo "Handling Edge Test Cases:  $EDGE_CASES_SCORE/$MAX_EDGE_CASE_POINTS points"
    echo "------------------------------------------------------------------"
    echo -e "🏆 TOTAL AUTOGRADER SCORE: ${YELLOW}$total_score/$TOTAL_POINTS${NC} points (${YELLOW}$percentage%${NC})"
    echo -e "🔍 Code Quality: ${YELLOW}10 points${NC} (manual grading by instructor)"       
    
    if [ $percentage -ge 90 ]; then
        echo -e "🎉 Performance: Excellent (${YELLOW}A range${NC})"
    elif [ $percentage -ge 80 ]; then
        echo -e "🌟 Performance: Good (${YELLOW}B range${NC})"
    elif [ $percentage -ge 70 ]; then
        echo -e "👍 Performance: Acceptable (${YELLOW}C range${NC})"
    elif [ $percentage -ge 60 ]; then
        echo -e "🚨 Performance: Below expectations (${YELLOW}D range${NC})"
    else        
        echo -e "⚠️ Performance: Significant issues (${YELLOW}F range${NC})"
    fi
    echo "------------------------------------------------------------------"

    echo ""
    echo "IMPLEMENTATION CHECKLIST:"
    echo "==================================================================" 
    echo "  - calculate_need():           $([ $SAFETY_CHECK_SCORE -gt 0 ] && echo "✓ Working" || echo "❌ Issues")"
    echo "  - calculate_available():      $([ $SAFETY_CHECK_SCORE -gt 0 ] && echo "✓ Working" || echo "❌ Issues")"
    echo "  - is_safe_state():            $([ $SAFETY_CHECK_SCORE -gt 5 ] && echo  "✓ Working" || echo "❌ Issues")"
    echo "  - thread_resource_request():  $([ $((VALID_REQUESTS_SCORE + INVALID_REQUESTS_SCORE)) -gt 10 ] && echo  "✓ Working" || echo "❌ Issues")"     
    echo "=================================================================="
    echo ""
}

#==============================================================================
# Main Execution
#==============================================================================

main() {
    echo " "
    echo "=================================================================="
    echo "Starting Banker's Algorithm Autograder"
    echo "Required Files: ${REQUIRED_STUDENT_FILES[*]}"
    echo "Grading Rubric: 90 points total"
    echo "=================================================================="
    
    check_required_files
    check_student_files
    compile_code
    run_program
    grade_algorithm
    generate_report
    
    # FIXED: Manual cleanup instead of relying on trap
    cleanup
    
    exit 0
}

# Execute main function
main "$@"