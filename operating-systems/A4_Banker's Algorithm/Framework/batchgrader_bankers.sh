#!/bin/bash

# =====================================================================================
# COP4610 Assignment 4 - Banker's Algorithm Batch-autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# Description: This script processes all student submissions and generates grade files
# =====================================================================================

echo "=========================================================================="
echo "           COP4610 Assignment 4 - Banker's Algorithm Batch Grading"
echo "=========================================================================="
echo "Date: $(date)"
echo

# Check if autograder script exists
if [ ! -f "autograder_bankers.sh" ]; then
    echo "❌ ERROR: autograder_bankers.sh not found in current directory!"
    echo "Please place the autograder script in the same directory as this batch script."
    exit 1
fi

# Check if required framework files exist
REQUIRED_FILES=("SYSTEM_STATE.txt" "EXPECTED_OUTPUT.txt" "INPUT.txt" "makefile" "banker_header.h" "banker_driver.c")
for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$file" ]; then
        echo "❌ ERROR: Required framework file '$file' not found!"
        echo "Please ensure all framework files are in the current directory."
        exit 1
    fi
done

# Make autograder script executable
chmod +x autograder_bankers.sh

# Create results directory with fixed name
RESULTS_DIR="A4_grading_Summary_File"
mkdir -p "$RESULTS_DIR"

# Create summary file
SUMMARY_FILE="$RESULTS_DIR/grading_summary.txt"
echo "COP4610 Assignment 4 - Banker's Algorithm Grading Summary" > "$SUMMARY_FILE"
echo "Generated: $(date)" >> "$SUMMARY_FILE"
echo "=================================================================" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"

# Create detailed log file
LOG_FILE="$RESULTS_DIR/batch_grading.log"
echo "Batch Grading Log - $(date)" > "$LOG_FILE"
echo "=================================" >> "$LOG_FILE"
echo >> "$LOG_FILE"

# Initialize counters
TOTAL_STUDENTS=0
SUCCESSFUL_GRADES=0
FAILED_GRADES=0
PERFECT_SCORES=0

# Required C files for the assignment
REQUIRED_C_FILES=("need.c" "available.c" "safety.c" "request.c")

# Function to log messages
log_message() {
    echo "$1" | tee -a "$LOG_FILE"
}

# Function to extract student name from filename
extract_student_name() {
    local filename="$1"
    # Remove .zip suffix and try to extract meaningful name
    local base_name=$(basename "$filename" .zip)
    
    # Try different patterns to extract student name
    if [[ "$base_name" =~ ^Banker_(.+)$ ]]; then
        # Pattern: Banker_FirstName_LastName
        echo "${BASH_REMATCH[1]}" | tr '_' ' '
    elif [[ "$base_name" =~ ^([A-Za-z]+[_\s]+[A-Za-z]+) ]]; then
        # Pattern: FirstName_LastName or FirstName LastName
        echo "${BASH_REMATCH[1]}" | tr '_' ' '
    elif [[ "$base_name" =~ ^([A-Za-z]+_[A-Za-z]+) ]]; then
        # Pattern: FirstName_LastName
        echo "${BASH_REMATCH[1]}" | tr '_' ' '
    else
        # Fallback: use the entire filename (cleaned up)
        echo "$base_name" | tr '_' ' ' | sed 's/[^A-Za-z0-9 ]//g'
    fi
}

# Function to create clean filename
create_clean_name() {
    local name="$1"
    echo "$name" | tr ' ' '_' | tr -cd '[:alnum:]_-'
}

# Process all ZIP files in the directory
shopt -s nullglob
zip_files=(*.zip)

if [ ${#zip_files[@]} -eq 0 ]; then
    echo "❌ No ZIP files found in current directory"
    echo "Please ensure student submission ZIP files are in the current directory."
    echo "Supported format: Any .zip file containing the required C implementation files"
    exit 1
fi

log_message "Found ${#zip_files[@]} student submission(s) to process"
log_message ""

# Process each ZIP file
for zip_file in "${zip_files[@]}"; do
    TOTAL_STUDENTS=$((TOTAL_STUDENTS + 1))
    
    # Extract student information
    STUDENT_NAME=$(extract_student_name "$zip_file")
    CLEAN_NAME=$(create_clean_name "$STUDENT_NAME")
    
    # If no meaningful name extracted, use the zip filename
    if [[ -z "$STUDENT_NAME" || "$STUDENT_NAME" =~ ^[[:space:]]*$ ]]; then
        STUDENT_NAME=$(basename "$zip_file" .zip)
        CLEAN_NAME=$(create_clean_name "$STUDENT_NAME")
    fi
    
    log_message "Processing: $STUDENT_NAME ($zip_file)"
    
    # Create temporary directory for extraction
    TEMP_DIR="temp_${CLEAN_NAME}_$$"
    mkdir -p "$TEMP_DIR"
    
    # Extract ZIP file
    log_message "  📦 Extracting submission..."
    if ! unzip -q "$zip_file" -d "$TEMP_DIR" 2>/dev/null; then
        log_message "  ❌ ERROR: Failed to extract $zip_file"
        echo "$STUDENT_NAME: EXTRACTION_FAILED - Could not extract ZIP file" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        continue
    fi
    
    # Check for README file with flexible case-insensitive matching
    README_FOUND=false
    README_FILE_FOUND=""
    README_INFO=""

    # Find any file with "readme" in name (case-insensitive)
    readme_candidates=$(find "$TEMP_DIR" -type f -iname "*readme*" 2>/dev/null)
    
    if [ -n "$readme_candidates" ]; then
        # Priority: PDF > DOCX > TXT > MD > No extension > Any other
        
        # Try PDF first
        found_readme=$(echo "$readme_candidates" | grep -i '\.pdf' | head -1)
        
        # Try DOCX
        if [ -z "$found_readme" ]; then
            found_readme=$(echo "$readme_candidates" | grep -i '\.docx' | head -1)
        fi
        
        # Try TXT
        if [ -z "$found_readme" ]; then
            found_readme=$(echo "$readme_candidates" | grep -i '\.txt' | head -1)
        fi
        
        # Try MD
        if [ -z "$found_readme" ]; then
            found_readme=$(echo "$readme_candidates" | grep -i '\.md' | head -1)
        fi
        
        # Try no extension (README, readme)
        if [ -z "$found_readme" ]; then
            found_readme=$(echo "$readme_candidates" | grep -iE '/readme$' | head -1)
        fi
        
        if [ -n "$found_readme" ]; then
            README_FOUND=true
            README_FILE_FOUND="$found_readme"
            log_message "  ✅ Found README: $(basename "$README_FILE_FOUND")"
            
            # Try to extract info from README (only for text-based formats)
            if [[ "$README_FILE_FOUND" =~ \.(txt|md)$ ]] || [[ ! "$README_FILE_FOUND" =~ \. ]]; then
                README_INFO=$(head -20 "$README_FILE_FOUND" 2>/dev/null | grep -v "^$")
            fi
        fi
    fi
    
    if [ "$README_FOUND" = false ]; then
        log_message "  ❌ ERROR: Missing README file"
        echo "$STUDENT_NAME: MISSING_README - No README file found" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        echo ""
        continue
    fi
    
    # Search for required C files recursively
    log_message "  🔍 Searching for required C files..."
    FOUND_FILES=()
    MISSING_FILES=()
    
    for required_file in "${REQUIRED_C_FILES[@]}"; do
        found_file=$(find "$TEMP_DIR" -name "$required_file" -type f | head -1)
        if [ -n "$found_file" ]; then
            FOUND_FILES+=("$required_file:$found_file")
        else
            MISSING_FILES+=("$required_file")
            #log_message "  ❌ Missing: $required_file"
        fi
    done
    
    # Check if all required files are found - NO PARTIAL GRADING
    if [ ${#MISSING_FILES[@]} -gt 0 ]; then
        log_message "  ❌ ERROR: Missing required files: ${MISSING_FILES[*]}"
        
        # List all C files found for debugging
        log_message "  🔍 C files found in submission:"
        find "$TEMP_DIR" -name "*.c" -type f | sed 's/^/    /' | tee -a "$LOG_FILE"
        
        echo "$STUDENT_NAME: MISSING_FILES - Missing: ${MISSING_FILES[*]}" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        echo ""
        continue
    fi
    
    # Copy student files to current directory
    log_message "  📋 Copying student implementation files..."
    for file_info in "${FOUND_FILES[@]}"; do
        file_name=$(echo "$file_info" | cut -d':' -f1)
        file_path=$(echo "$file_info" | cut -d':' -f2)
        
        if ! cp "$file_path" "./$file_name"; then
            log_message "  ❌ ERROR: Failed to copy $file_name"
            echo "$STUDENT_NAME: COPY_FAILED - Could not copy $file_name" >> "$SUMMARY_FILE"
            FAILED_GRADES=$((FAILED_GRADES + 1))
            rm -rf "$TEMP_DIR"
            continue 2
        fi
    done
    
    # Clean up any previous build artifacts
    log_message "  🧹 Cleaning previous build artifacts..."
    rm -f STUDENT_OUTPUT.txt banker *.o compile_errors.txt runtime_errors.txt 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null

    # Ensure we start with a clean build
    if [ -f "makefile" ]; then
        make clean >/dev/null 2>&1 || true
    fi
    
    # Create individual grade report
    GRADE_FILE="$RESULTS_DIR/${CLEAN_NAME}_Grade.txt"

    # CRITICAL: Clean environment before each autograder run
    log_message "  🔄 Ensuring clean environment..."
    unset COMPILATION_SCORE SAFETY_CHECK_SCORE VALID_REQUESTS_SCORE INVALID_REQUESTS_SCORE EDGE_CASES_SCORE
    rm -f STUDENT_OUTPUT.txt banker *.o compile_errors.txt runtime_errors.txt 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null
    if [ -f "makefile" ]; then
        make clean >/dev/null 2>&1 || true
    fi
    sync
    sleep 1
   
    log_message "  🎯 Running autograder..."
    
    # Generate grade report header
    {
        echo "=========================================================================="
        echo "                   GRADE REPORT FOR: $STUDENT_NAME"
        echo "=========================================================================="
        echo "Submission File: $zip_file"
        echo "Graded on: $(date)"
        echo "Graded by: Banker's Algorithm Autograder"
        echo
        
        if [ -n "$README_INFO" ]; then
            echo "Student Information from README:"
            echo "--------------------------------"
            echo "$README_INFO"
            echo
        fi
        
        echo "Files Submitted:"
        echo "----------------"
        echo "✅ README: $(basename "$README_FILE_FOUND")"
        for file_info in "${FOUND_FILES[@]}"; do
            file_name=$(echo "$file_info" | cut -d':' -f1)
            echo "✅ $file_name"
        done
        
        echo
        echo "Grading Results:"
        echo "================"
        echo
    } > "$GRADE_FILE"
    
    # Run the autograder with timeout protection (15 seconds)
    AUTOGRADER_OUTPUT=$(timeout 15 ./autograder_bankers.sh 2>&1)
    AUTOGRADER_EXIT_CODE=$?
    
    # Append autograder output to grade file
    echo "$AUTOGRADER_OUTPUT" >> "$GRADE_FILE"
    
    if [ $AUTOGRADER_EXIT_CODE -eq 0 ]; then
        log_message "  ✅ Grading completed successfully"
        
        # Extract final score and percentage from autograder output
        FINAL_SCORE=$(echo "$AUTOGRADER_OUTPUT" | grep "TOTAL AUTOGRADER SCORE:" | tail -1 | grep -o '[0-9]\+/[0-9]\+' | head -1)
        PERCENTAGE=$(echo "$AUTOGRADER_OUTPUT" | grep "TOTAL AUTOGRADER SCORE:" | grep -o '[0-9]\+%' | head -1)
        
        if [ -n "$FINAL_SCORE" ]; then
            if [ -n "$PERCENTAGE" ]; then
                echo "$STUDENT_NAME: $FINAL_SCORE ($PERCENTAGE)" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE ($PERCENTAGE)"
                
                # Check for perfect score
                if [[ "$PERCENTAGE" == "100%" ]]; then
                    PERFECT_SCORES=$((PERFECT_SCORES + 1))
                fi
            else
                echo "$STUDENT_NAME: $FINAL_SCORE" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE"
            fi
        else
            echo "$STUDENT_NAME: COMPLETED - Check individual grade file" >> "$SUMMARY_FILE"
            log_message "  📊 Grading completed - check grade file for details"
        fi
        
        SUCCESSFUL_GRADES=$((SUCCESSFUL_GRADES + 1))
        
    elif [ $AUTOGRADER_EXIT_CODE -eq 124 ]; then
        log_message "  ⏱️  ERROR: Autograder timed out (15-second limit)"
        echo >> "$GRADE_FILE"
        echo "ERROR: Autograder timed out after 15 seconds" >> "$GRADE_FILE"
        echo "This usually indicates infinite loops in Banker's algorithm implementation." >> "$GRADE_FILE"
        echo "Common causes:" >> "$GRADE_FILE"
        echo "- Infinite loops in safety check algorithm" >> "$GRADE_FILE"
        echo "- Incorrect loop conditions in request processing" >> "$GRADE_FILE"
        echo "- Missing termination conditions in need calculation" >> "$GRADE_FILE"
        echo "- Wrong array indexing or bounds checking" >> "$GRADE_FILE"
        echo "- Available resources calculation errors" >> "$GRADE_FILE"
        echo "$STUDENT_NAME: TIMEOUT - Infinite loop or runtime error detected" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        
    else
        log_message "  ❌ ERROR: Autograder failed (exit code: $AUTOGRADER_EXIT_CODE)"
        echo >> "$GRADE_FILE"
        echo "ERROR: Autograder script failed with exit code: $AUTOGRADER_EXIT_CODE" >> "$GRADE_FILE"
        echo "This could indicate:" >> "$GRADE_FILE"
        echo "- Compilation errors in student code" >> "$GRADE_FILE"
        echo "- Missing function implementations" >> "$GRADE_FILE"
        echo "- Syntax errors or invalid C code" >> "$GRADE_FILE"
        echo "- Incompatible function signatures" >> "$GRADE_FILE"
        echo "- Logic errors causing segmentation faults" >> "$GRADE_FILE"
        echo "$STUDENT_NAME: AUTOGRADER_FAILED - Compilation or runtime error" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
    fi
    
    # Clean up temporary files and student implementation files
    log_message "  🧹 Cleaning up temporary files..."
    chmod -R 755 "$TEMP_DIR" 2>/dev/null || true
    rm -rf "$TEMP_DIR"
    
    # Remove student implementation files
    for file in "${REQUIRED_C_FILES[@]}"; do
        rm -f "$file" 2>/dev/null
    done
    
    # Remove any generated files from autograder
    rm -f STUDENT_OUTPUT.txt banker *.o 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null
    rm -f compile_errors.txt runtime_errors.txt 2>/dev/null
    
    # Force make clean to remove any leftover build artifacts
    if [ -f "makefile" ]; then
        make clean >/dev/null 2>&1 || true
    fi
    
    # Remove any additional temporary files that might be created
    rm -f temp_* *.tmp *.temp core 2>/dev/null
    
    log_message "  💾 Grade saved to: $GRADE_FILE"
    log_message ""
done

# Generate final summary statistics
echo >> "$SUMMARY_FILE"
echo "=================================================================" >> "$SUMMARY_FILE"
echo "                        GRADING STATISTICS" >> "$SUMMARY_FILE"
echo "=================================================================" >> "$SUMMARY_FILE"
echo "Total Students Processed: $TOTAL_STUDENTS" >> "$SUMMARY_FILE"
echo "Successfully Graded: $SUCCESSFUL_GRADES" >> "$SUMMARY_FILE"
echo "Failed to Grade: $FAILED_GRADES" >> "$SUMMARY_FILE"
echo "Perfect Scores (100%): $PERFECT_SCORES" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"

if [ $TOTAL_STUDENTS -gt 0 ]; then
    echo "Success Rate: $(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))%" >> "$SUMMARY_FILE"
else
    echo "Success Rate: 0%" >> "$SUMMARY_FILE"
fi

echo >> "$SUMMARY_FILE"
echo "Common Issues Found:" >> "$SUMMARY_FILE"
echo "- Missing README file (required for grading)" >> "$SUMMARY_FILE"
echo "- Missing implementation files (all 4 C files required)" >> "$SUMMARY_FILE"
echo "- Infinite loops in safety check algorithm" >> "$SUMMARY_FILE"
echo "- Incorrect need matrix calculation" >> "$SUMMARY_FILE"
echo "- Available resources calculation errors" >> "$SUMMARY_FILE"
echo "- Compilation errors due to syntax or logic issues" >> "$SUMMARY_FILE"
echo "- Request validation logic errors" >> "$SUMMARY_FILE"
echo "- Deadlock detection implementation issues" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"
echo "Individual grade files are in: $RESULTS_DIR/" >> "$SUMMARY_FILE"
echo "Detailed log file: $LOG_FILE" >> "$SUMMARY_FILE"

# Display final results
echo "=========================================================================="
echo "                      BATCH GRADING COMPLETE"
echo "=========================================================================="
echo "📊 Total Students Processed: $TOTAL_STUDENTS"
echo "✅ Successfully Graded: $SUCCESSFUL_GRADES"
echo "❌ Failed to Grade: $FAILED_GRADES"
echo "🌟 Perfect Scores: $PERFECT_SCORES"

if [ $TOTAL_STUDENTS -gt 0 ]; then
    echo "📈 Success Rate: $(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))%"
else
    echo "📈 Success Rate: 0%"
fi

echo
echo "📁 Results Directory: $RESULTS_DIR/"
echo "📋 Summary File: $SUMMARY_FILE"
echo "📝 Detailed Log: $LOG_FILE"
echo
echo "📄 Individual Grade Files:"
find "$RESULTS_DIR" -name "*_Grade.txt" -type f | sed 's/^/  /' | sort
echo
echo "⏰ Grading completed at: $(date)"
echo "=========================================================================="

# Final cleanup
echo "🧹 Final cleanup completed."
echo
echo "🎁 Ready for grade submission!"

exit 0