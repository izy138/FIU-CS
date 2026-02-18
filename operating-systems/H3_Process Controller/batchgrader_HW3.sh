#!/bin/bash

# =====================================================================================
# Homework 3 - Batch Autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# Description: This script processes all student submissions and generates grade files
# =====================================================================================

echo "=========================================================================="
echo "     HOMEWORK 3 - Batch Autograder                                        "
echo "=========================================================================="
echo "Date: $(date)"
echo

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

# Function to find student makefile with case-insensitive matching
find_student_makefile() {
    local temp_dir="$1"
    local makefile_path=""
    
    # Try exact matches first (preferred)
    if [[ -n $(find "$temp_dir" -name "Makefile" -type f | head -1) ]]; then
        makefile_path=$(find "$temp_dir" -name "Makefile" -type f | head -1)
    elif [[ -n $(find "$temp_dir" -name "makefile" -type f | head -1) ]]; then
        makefile_path=$(find "$temp_dir" -name "makefile" -type f | head -1)
    else
        # Try case-insensitive search for any makefile variant
        makefile_path=$(find "$temp_dir" -iname "makefile*" -type f | head -1)
    fi
    
    echo "$makefile_path"
}

# Check if autograder script exists
if [ ! -f "autograder_HW3.sh" ]; then
    echo "ERROR: autograder_HW3.sh not found in current directory!"
    echo "Please place the autograder script in the same directory as this batch script."
    exit 1
fi

# Check if required framework directories exist
REQUIRED_DIRS=("Testing/Testcases" "Testing/Expected_Output")
for item in "${REQUIRED_DIRS[@]}"; do
    if [ ! -e "$item" ]; then
        echo "ERROR: Required framework item '$item' not found!"
        echo "Please ensure all framework files/directories are in the current directory."
        exit 1
    fi
done

# Make autograder script executable
chmod +x autograder_HW3.sh

# Create results directory (clean startup)
RESULTS_DIR="GRADING_RESULTS"

# Remove existing results directory if it exists for clean startup
if [ -d "$RESULTS_DIR" ]; then
    echo "Removing existing results directory for clean startup..."
    rm -rf "$RESULTS_DIR"
fi

mkdir -p "$RESULTS_DIR"

# Create summary file
SUMMARY_FILE="$RESULTS_DIR/grading_summary.txt"
echo "COP4610 Assignment 3 - Grading Summary" > "$SUMMARY_FILE"
echo "Generated: $(date)" >> "$SUMMARY_FILE"
echo "=================================================================" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"

# Create detailed log file
LOG_FILE="$RESULTS_DIR/batch_grading.log"
echo "Batch Grading Log - $(date)" > "$LOG_FILE"
echo "=================================" >> "$LOG_FILE"
echo >> "$LOG_FILE"

# Master CSV file
MASTER_CSV="$RESULTS_DIR/ALL_GRADES.csv"
echo "zip_filename,assignment_name,final_score" > "$MASTER_CSV"

# Initialize counters
TOTAL_STUDENTS=0
SUCCESSFUL_GRADES=0
FAILED_GRADES=0
PERFECT_SCORES=0
STUDENTS_WITH_MAKEFILE=0
STUDENTS_WITHOUT_MAKEFILE=0

# Required files for the assignment (adjust as needed)
REQUIRED_C_FILES=("submission_HW3.c")

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
    if [[ "$base_name" =~ ^A[0-9]+_(.+)$ ]]; then
        # Pattern: A3_FirstName_LastName
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
    echo "No ZIP files found in current directory"
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
    if ! unzip -q "$zip_file" -d "$TEMP_DIR" 2>/dev/null; then
        log_message "  ERROR: Failed to extract $zip_file"
        echo "$STUDENT_NAME: EXTRACTION_FAILED - Could not extract ZIP file" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        continue
    fi
    
    # Show required files first
    log_message "  Required files for this assignment:"
    for required_file in "${REQUIRED_C_FILES[@]}"; do
        log_message "    - $required_file"
    done
    log_message "    - Makefile (optional - framework Makefile provided)"
    
    # Search for required files recursively
    FOUND_FILES=()
    MISSING_FILES=()
    
    log_message "  File submission status:"
    
    # Search for C files
    for required_file in "${REQUIRED_C_FILES[@]}"; do
        found_file=$(find "$TEMP_DIR" -name "$required_file" -type f | head -1)
        if [ -n "$found_file" ]; then
            FOUND_FILES+=("$required_file:$found_file")
            log_message "    ✅ Found: $required_file"
        else
            # Look for any .c file that might be the submission
            any_c_file=$(find "$TEMP_DIR" -name "*.c" -type f | head -1)
            if [ -n "$any_c_file" ]; then
                log_message "    ⚠️ Using $(basename "$any_c_file") as $required_file"
                FOUND_FILES+=("$required_file:$any_c_file")
            else
                MISSING_FILES+=("$required_file")
                log_message "    ❌ Missing: $required_file"
            fi
        fi
    done
    
    # Check for student Makefile and report status (similar to HW1)
    STUDENT_MAKEFILE_PATH=$(find_student_makefile "$TEMP_DIR")
    STUDENT_MAKEFILE_NAME=""
    
    if [[ -n "$STUDENT_MAKEFILE_PATH" ]]; then
        STUDENT_MAKEFILE_NAME=$(basename "$STUDENT_MAKEFILE_PATH")
        log_message "    ✅ Found student Makefile: $STUDENT_MAKEFILE_NAME (will use framework Makefile instead)"
        STUDENTS_WITH_MAKEFILE=$((STUDENTS_WITH_MAKEFILE + 1))
        export USE_STUDENT_MAKEFILE="false"  # For HW3, we always use framework Makefile
        export STUDENT_MAKEFILE_NAME="$STUDENT_MAKEFILE_NAME"
    else
        log_message "    ❌ No student Makefile found (using framework Makefile - this is expected)"
        STUDENTS_WITHOUT_MAKEFILE=$((STUDENTS_WITHOUT_MAKEFILE + 1))
        export USE_STUDENT_MAKEFILE="false"
        export STUDENT_MAKEFILE_NAME=""
    fi
    
    # Look for README file
    README_INFO=""
    README_FILE=$(find "$TEMP_DIR" -iname "readme*" -type f | head -1)
    if [ -n "$README_FILE" ]; then
        README_INFO=$(head -20 "$README_FILE" | grep -v "^$")
    else
        log_message "    ❌ No README file found"
    fi
    
    # Check if we can proceed with grading
    SHOULD_GRADE=true
    FAILURE_REASON=""
    
    # Check if all required files are found
    REQUIRED_TOTAL=${#REQUIRED_C_FILES[@]}
    if [ ${#FOUND_FILES[@]} -lt $REQUIRED_TOTAL ]; then
        # Show specific error message for the first missing file (matching HW1 format)
        FIRST_MISSING=${MISSING_FILES[0]}
        log_message "  ⚠️ ERROR: Autograder failed (No submission of \"$FIRST_MISSING\" found)"
        echo "$STUDENT_NAME: MISSING_FILES - Missing required files: ${MISSING_FILES[*]}" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        SHOULD_GRADE=false
        FAILURE_REASON="MISSING_FILES"
    fi
    
    # If we have files to grade, proceed
    if [ "$SHOULD_GRADE" = true ]; then
        # CRITICAL: Clean up any leftover files from previous student
        if command -v make >/dev/null 2>&1; then
            make clean 2>/dev/null || true  # Use Makefile's clean target if available
        fi
        # Fallback manual cleanup
        rm -f run *.o 2>/dev/null
        rm -rf Results/ 2>/dev/null
        
        # Copy student files to current directory
        for file_info in "${FOUND_FILES[@]}"; do
            file_name=$(echo "$file_info" | cut -d':' -f1)
            file_path=$(echo "$file_info" | cut -d':' -f2)
            
            if ! cp "$file_path" "./$file_name"; then
                log_message "  ERROR: Failed to copy $file_name"
                echo "$STUDENT_NAME: COPY_FAILED - Could not copy $file_name" >> "$SUMMARY_FILE"
                FAILED_GRADES=$((FAILED_GRADES + 1))
                SHOULD_GRADE=false
                FAILURE_REASON="COPY_FAILED"
                break
            fi
        done
    fi
    
    # Proceed with grading if all files are ready
    if [ "$SHOULD_GRADE" = true ]; then
        # Create individual grade report
        GRADE_FILE="$RESULTS_DIR/${CLEAN_NAME}_Grade.txt"
        
        # CRITICAL: Set environment variable for autograder
        export STUDENT_ZIP_NAME="$(basename "$zip_file" .zip)"
        
        log_message "  Running autograder for $STUDENT_ZIP_NAME..."
        
        # Generate grade report header
        {
            echo "=========================================================================="
            echo "                   GRADE REPORT FOR: $STUDENT_NAME"
            echo "=========================================================================="
            echo "Submission File: $zip_file"
            echo "Graded on: $(date)"
            echo "Graded by: Assignment Autograder"
            echo
            
            if [ -n "$README_INFO" ]; then
                echo "Student Information from README:"
                echo "--------------------------------"
                echo "$README_INFO"
                echo
            fi
            
            echo "Files Submitted:"
            echo "----------------"
            for file_info in "${FOUND_FILES[@]}"; do
                file_name=$(echo "$file_info" | cut -d':' -f1)
                echo "✅ $file_name"
            done
            
            # Add Makefile information
            echo
            echo "Makefile Information:"
            echo "--------------------"
            if [[ "$USE_STUDENT_MAKEFILE" == "true" ]]; then
                echo "✅ Student submitted Makefile: $STUDENT_MAKEFILE_NAME (using framework Makefile)"
            else
                if [[ -n "$STUDENT_MAKEFILE_NAME" ]]; then
                    echo "✅ Student submitted Makefile: $STUDENT_MAKEFILE_NAME (using framework Makefile instead)"
                else
                    echo "❌ Student did not submit Makefile - using framework Makefile (expected)"
                fi
            fi
            
            if [ ${#MISSING_FILES[@]} -gt 0 ]; then
                echo
                echo "Missing Files:"
                echo "-------------"
                for missing_file in "${MISSING_FILES[@]}"; do
                    echo "❌ $missing_file"
                done
            fi
            
            echo
            echo "Grading Results:"
            echo "================"
            echo
        } > "$GRADE_FILE"
        
        # Run the autograder with timeout protection
        AUTOGRADER_OUTPUT=$(timeout 30 ./autograder_HW3.sh 2>&1)
        AUTOGRADER_EXIT_CODE=$?
        
        # Append autograder output to grade file
        echo "$AUTOGRADER_OUTPUT" >> "$GRADE_FILE"
        
        if [ $AUTOGRADER_EXIT_CODE -eq 0 ]; then
            # Extract final score from autograder output
            FINAL_SCORE=$(echo "$AUTOGRADER_OUTPUT" | grep "FINAL GRADE:" | tail -1 | grep -o '[0-9]\+/[0-9]\+' | head -1 | cut -d'/' -f1)
            TOTAL_POSSIBLE=$(echo "$AUTOGRADER_OUTPUT" | grep "FINAL GRADE:" | tail -1 | grep -o '[0-9]\+/[0-9]\+' | head -1 | cut -d'/' -f2)
            
            if [ -n "$FINAL_SCORE" ] && [ -n "$TOTAL_POSSIBLE" ]; then
                PERCENTAGE=$(( FINAL_SCORE * 100 / TOTAL_POSSIBLE ))
                echo "$STUDENT_NAME: $FINAL_SCORE/$TOTAL_POSSIBLE ($PERCENTAGE%)" >> "$SUMMARY_FILE"
                log_message "  Score: $FINAL_SCORE/$TOTAL_POSSIBLE ($PERCENTAGE%)"
                
                # Check for perfect score
                if [[ "$PERCENTAGE" -eq 100 ]]; then
                    PERFECT_SCORES=$((PERFECT_SCORES + 1))
                fi
                
                # Copy CSV data to master file if it exists
                if [ -f "Results/grade.csv" ]; then
                    tail -n +2 "Results/grade.csv" >> "$MASTER_CSV"
                fi
            else
                echo "$STUDENT_NAME: COMPLETED - Check individual grade file" >> "$SUMMARY_FILE"
                log_message "  Grading completed - check grade file for details"
            fi
            
            SUCCESSFUL_GRADES=$((SUCCESSFUL_GRADES + 1))
            
        elif [ $AUTOGRADER_EXIT_CODE -eq 124 ]; then
            log_message "  ERROR: Autograder timed out (30-second limit)"
            echo >> "$GRADE_FILE"
            echo "ERROR: Autograder timed out after 30 seconds" >> "$GRADE_FILE"
            echo "This usually indicates infinite loops or hanging programs." >> "$GRADE_FILE"
            echo "$STUDENT_NAME: TIMEOUT - Program hung or infinite loop detected" >> "$SUMMARY_FILE"
            FAILED_GRADES=$((FAILED_GRADES + 1))
            
        else
            log_message "  ⚠️ ERROR: Autograder failed (exit code: $AUTOGRADER_EXIT_CODE)"
            echo >> "$GRADE_FILE"
            echo "ERROR: Autograder script failed with exit code: $AUTOGRADER_EXIT_CODE" >> "$GRADE_FILE"
            echo "$STUDENT_NAME: AUTOGRADER_FAILED - Compilation or runtime error" >> "$SUMMARY_FILE"
            FAILED_GRADES=$((FAILED_GRADES + 1))
        fi
        
        # Copy Results directory to student's permanent location
        if [ -d "Results" ]; then
            STUDENT_RESULTS_DIR="$RESULTS_DIR/${CLEAN_NAME}_Results"
            mkdir -p "$STUDENT_RESULTS_DIR"
            cp -r Results/* "$STUDENT_RESULTS_DIR/" 2>/dev/null || true
        fi
    fi
    
    # Clean up temporary files and student implementation files
    chmod -R 755 "$TEMP_DIR" 2>/dev/null || true
    rm -rf "$TEMP_DIR"
    
    # Remove student implementation files
    for file in "${REQUIRED_C_FILES[@]}"; do
        rm -f "$file" 2>/dev/null
    done
    
    # Remove any generated files from autograder
    rm -f submission_HW3 *.o 2>/dev/null
    rm -rf Results/ 2>/dev/null
    
    # Clear environment variables
    unset STUDENT_ZIP_NAME
    unset USE_STUDENT_MAKEFILE
    unset STUDENT_MAKEFILE_NAME
    
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
echo "Makefile Submission Statistics:" >> "$SUMMARY_FILE"
echo "Students who submitted Makefiles: $STUDENTS_WITH_MAKEFILE" >> "$SUMMARY_FILE"
echo "Students who did not submit Makefiles: $STUDENTS_WITHOUT_MAKEFILE" >> "$SUMMARY_FILE"
echo "Note: For HW3, framework Makefile is provided and always used for compilation" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"

if [ $TOTAL_STUDENTS -gt 0 ]; then
    SUCCESS_RATE=$(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))
    MAKEFILE_SUBMISSION_RATE=$(( (STUDENTS_WITH_MAKEFILE * 100) / TOTAL_STUDENTS ))
    echo "Success Rate: ${SUCCESS_RATE}%" >> "$SUMMARY_FILE"
else
    echo "Success Rate: 0%" >> "$SUMMARY_FILE"
fi

echo >> "$SUMMARY_FILE"
echo "Individual grade files are in: $RESULTS_DIR/" >> "$SUMMARY_FILE"
echo "Master CSV file: $MASTER_CSV" >> "$SUMMARY_FILE"
echo "Detailed log file: $LOG_FILE" >> "$SUMMARY_FILE"

# Calculate grade distribution
if [ $SUCCESSFUL_GRADES -gt 0 ]; then
    echo >> "$SUMMARY_FILE"
    echo "Grade Distribution Analysis:" >> "$SUMMARY_FILE"
    echo "============================" >> "$SUMMARY_FILE"
    
    # Count grades in different ranges
    A_GRADES=0
    B_GRADES=0
    C_GRADES=0
    D_GRADES=0
    F_GRADES=0
    
    # Read CSV file and analyze grades
    if [ -f "$MASTER_CSV" ]; then
        while IFS=',' read -r zip_name assignment_name score; do
            # Skip header
            if [[ "$score" == "final_score" ]]; then
                continue
            fi
            
            if [ "$score" -ge 90 ]; then
                A_GRADES=$((A_GRADES + 1))
            elif [ "$score" -ge 80 ]; then
                B_GRADES=$((B_GRADES + 1))
            elif [ "$score" -ge 70 ]; then
                C_GRADES=$((C_GRADES + 1))
            elif [ "$score" -ge 60 ]; then
                D_GRADES=$((D_GRADES + 1))
            else
                F_GRADES=$((F_GRADES + 1))
            fi
        done < "$MASTER_CSV"
        
        echo "A (90-100%): $A_GRADES students" >> "$SUMMARY_FILE"
        echo "B (80-89%):  $B_GRADES students" >> "$SUMMARY_FILE"
        echo "C (70-79%):  $C_GRADES students" >> "$SUMMARY_FILE"
        echo "D (60-69%):  $D_GRADES students" >> "$SUMMARY_FILE"
        echo "F (0-59%):   $F_GRADES students" >> "$SUMMARY_FILE"
    fi
fi

# Display final results
echo "=========================================================================="
echo "                      BATCH GRADING COMPLETE"
echo "=========================================================================="
echo "Total Students Processed: $TOTAL_STUDENTS"
echo "Successfully Graded: $SUCCESSFUL_GRADES"
echo "Failed to Grade: $FAILED_GRADES"
echo "Perfect Scores: $PERFECT_SCORES"
echo "Students with Makefiles: $STUDENTS_WITH_MAKEFILE"
echo "Students without Makefiles: $STUDENTS_WITHOUT_MAKEFILE"

if [ $TOTAL_STUDENTS -gt 0 ]; then
    SUCCESS_RATE=$(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))
    MAKEFILE_SUBMISSION_RATE=$(( (STUDENTS_WITH_MAKEFILE * 100) / TOTAL_STUDENTS ))
    echo "Success Rate: ${SUCCESS_RATE}%"
fi

echo
echo "Results Directory: $RESULTS_DIR/"
echo "Summary File: $SUMMARY_FILE"
echo "Master CSV: $MASTER_CSV"
echo "Detailed Log: $LOG_FILE"
echo "Individual Grade Files:"
echo "   -- Individual grade files are in the Results Directory"
echo "Individual Results Directories:"
echo "   -- Individual result folders are in the Results Directory"
echo 
echo "Grading completed at: $(date)"
echo "=========================================================================="

# Show CSV summary
if [ -f "$MASTER_CSV" ]; then
    echo
    total_entries=$(tail -n +2 "$MASTER_CSV" | wc -l)
    echo "Total entries in CSV: $total_entries"
fi

# Final cleanup
echo "Final cleanup completed."
echo "Ready for grade submission!"
echo

exit 0