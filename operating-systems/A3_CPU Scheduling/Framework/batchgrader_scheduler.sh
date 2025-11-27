#!/bin/bash

# =====================================================================================
# COP4610 Assignment - CPU Scheduling Algorithms Batch-autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# Description: This script processes all student submissions and generates grade files
# =====================================================================================


echo "=========================================================================="
echo "     COP4610 Assignment - CPU Scheduling Algorithms Batch-Autograder     "
echo "=========================================================================="
echo "Date: $(date)"
echo

# Check if autograder script exists
if [ ! -f "autograder_scheduler.sh" ]; then
    echo "❌ ERROR: autograder_scheduler.sh not found in current directory!"
    echo "Please place the autograder script in the same directory as this batch script."
    exit 1
fi

# Check if required framework files exist
REQUIRED_FILES=("driver.c" "CPU_scheduler.h" "Makefile")
for file in "${REQUIRED_FILES[@]}"; do
    if [ ! -f "$file" ]; then
        echo "❌ ERROR: Required framework file '$file' not found!"
        echo "Please ensure all framework files are in the current directory."
        exit 1
    fi
done

# Check if Testing directory exists
if [ ! -d "Testing" ]; then
    echo "❌ ERROR: Testing directory not found!"
    echo "Please ensure Testing/Testcases/ and Testing/Expected_output/ directories exist."
    exit 1
fi

if [ ! -f "Testing/Testcases/input1.txt" ]; then
    echo "❌ ERROR: Testing/Testcases/input1.txt not found!"
    exit 1
fi

if [ ! -f "Testing/Expected_output/output1.txt" ]; then
    echo "❌ ERROR: Testing/Expected_output/output1.txt not found!"
    exit 1
fi

# Make autograder script executable
chmod +x autograder_scheduler.sh

# Create results directory with fixed name
RESULTS_DIR="Scheduler_grading_Summary_File"
mkdir -p "$RESULTS_DIR"

# Create summary file
SUMMARY_FILE="$RESULTS_DIR/grading_summary.txt"
echo "COP4610 Assignment - CPU Scheduling Algorithms Grading Summary" > "$SUMMARY_FILE"
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
REQUIRED_C_FILES=("first_come_first_served.c" "round_robin.c" "shortest_job_first.c" "shortest_remaining_time_first.c" "priority_non_preemptive.c" "priority_preemptive_rr.c")

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
    if [[ "$base_name" =~ ^Scheduler_(.+)$ ]]; then
        # Pattern: Scheduler_FirstName_LastName
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
        
        # # Take any file with readme in name
        # if [ -z "$found_readme" ]; then
        #     found_readme=$(echo "$readme_candidates" | head -1)
        # fi
        
        if [ -n "$found_readme" ]; then
            README_FOUND=true
            README_FILE_FOUND="$found_readme"
            log_message "    ✅ Found README: $(basename "$README_FILE_FOUND")"
        fi
    fi
    
    if [ "$README_FOUND" = false ]; then
        log_message "  ❌ ERROR: Missing README file"
        echo "$STUDENT_NAME: MISSING_README - No README file found" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
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
            log_message "    ✅ Found: $required_file"
        else
            MISSING_FILES+=("$required_file")
            log_message "    ❌ Missing: $required_file"
        fi
    done
    
    # Check if at least one required file is found (partial grading)
    if [ ${#FOUND_FILES[@]} -eq 0 ]; then
        log_message "  ❌ ERROR: No required C files found"
        
        # List all C files found for debugging
        log_message "  🔍 All C files found in submission:"
        find "$TEMP_DIR" -name "*.c" -type f | sed 's/^/    /' | tee -a "$LOG_FILE"
        
        echo "$STUDENT_NAME: NO_C_FILES - No required implementation files found" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        continue
    fi
    
    # Log missing files for partial grading
    if [ ${#MISSING_FILES[@]} -gt 0 ]; then
        log_message "  ⚠️  Warning: Missing files (partial grading): ${MISSING_FILES[*]}"
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
        
        log_message "    ✅ Copied: $file_name"
    done
    
    # Create stub files for missing implementations to prevent compilation errors
    for missing_file in "${MISSING_FILES[@]}"; do
        log_message "  🔧 Creating stub file for missing: $missing_file"
        
        case "$missing_file" in
            "first_come_first_served.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void first_come_first_served(SchedulerContext *ctx) {
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "FCFS");
}
EOF
                ;;
            "round_robin.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void round_robin(SchedulerContext *ctx, int time_quantum) {
	(void)time_quantum;
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "RR");
}
EOF
                ;;
            "shortest_job_first.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void shortest_job_first(SchedulerContext *ctx) {
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "SJF");
}
EOF
                ;;
            "shortest_remaining_time_first.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void shortest_remaining_time_first(SchedulerContext *ctx) {
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "SRT");
}
EOF
                ;;
            "priority_non_preemptive.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void priority_non_preemptive(SchedulerContext *ctx) {
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "PRIORITY_NP");
}
EOF
                ;;
            "priority_preemptive_rr.c")
                cat > "$missing_file" << 'EOF'
#include "CPU_scheduler.h"
void priority_preemptive_rr(SchedulerContext *ctx, int time_quantum) {
	(void)time_quantum;
    reset_process_states(ctx);
    // Stub implementation - no functionality
    display_results(ctx, "PRIORITY_PREEMPTIVE_RR");
}
EOF
                ;;
        esac
        log_message "    🔧 Created stub: $missing_file"
    done
    
    # CRITICAL: Clean up any leftover files from previous student
    log_message "  🧹 Pre-cleaning workspace..."
    rm -f STUDENT_OUTPUT.txt scheduler *.o 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null
    rm -f compile_errors.txt runtime_errors.txt 2>/dev/null

    # Ensure we start with a clean build
    if [ -f "Makefile" ]; then
        make clean >/dev/null 2>&1 || true
    fi
    
    # Create individual grade report
    GRADE_FILE="$RESULTS_DIR/${CLEAN_NAME}_Grade.txt"

    # CRITICAL: Clean environment before each autograder run
    log_message "  🔄 Ensuring clean environment..."
    unset FCFS_SCORE RR_SCORE SJF_SCORE SRT_SCORE PRIORITY_NP_SCORE PRIORITY_P_SCORE COMPILATION_PENALTY RUNTIME_PENALTY
    rm -f STUDENT_OUTPUT.txt scheduler *.o compile_errors.txt runtime_errors.txt 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null
    if [ -f "Makefile" ]; then
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
        echo "Graded by: CPU Scheduling Algorithms Autograder"
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
        
        if [ ${#MISSING_FILES[@]} -gt 0 ]; then
            echo
            echo "Missing Files (Partial Grading):"
            echo "--------------------------------"
            for missing_file in "${MISSING_FILES[@]}"; do
                echo "❌ $missing_file (stub created - 0 points)"
            done
        fi
        
        echo
        echo "Grading Results:"
        echo "================"
        echo
    } > "$GRADE_FILE"
    
    # Run the autograder with timeout protection (18 seconds)
    AUTOGRADER_OUTPUT=$(timeout 18 ./autograder_scheduler.sh 2>&1)
    AUTOGRADER_EXIT_CODE=$?
    
    # Append autograder output to grade file
    echo "$AUTOGRADER_OUTPUT" >> "$GRADE_FILE"
    
    if [ $AUTOGRADER_EXIT_CODE -eq 0 ]; then
        log_message "  ✅ Grading completed successfully"
        
        # Extract final score and percentage from autograder output
        FINAL_SCORE=$(echo "$AUTOGRADER_OUTPUT" | grep "AUTOGRADER TOTAL:" | tail -1 | grep -o '[0-9]\+/[0-9]\+' | head -1)
        PERCENTAGE=$(echo "$AUTOGRADER_OUTPUT" | grep "AUTOGRADER TOTAL:" | grep -o '[0-9]\+%' | head -1)
        
        # Add note about missing files if applicable
        GRADE_NOTE=""
        if [ ${#MISSING_FILES[@]} -gt 0 ]; then
            GRADE_NOTE=" (Missing: ${#MISSING_FILES[@]} files)"
        fi
        
        if [ -n "$FINAL_SCORE" ]; then
            if [ -n "$PERCENTAGE" ]; then
                echo "$STUDENT_NAME: $FINAL_SCORE ($PERCENTAGE)$GRADE_NOTE" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE ($PERCENTAGE)$GRADE_NOTE"
                
                # Check for perfect score (only if no missing files)
                if [[ "$PERCENTAGE" == "100%" && ${#MISSING_FILES[@]} -eq 0 ]]; then
                    PERFECT_SCORES=$((PERFECT_SCORES + 1))
                fi
            else
                echo "$STUDENT_NAME: $FINAL_SCORE$GRADE_NOTE" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE$GRADE_NOTE"
            fi
        else
            echo "$STUDENT_NAME: COMPLETED$GRADE_NOTE - Check individual grade file" >> "$SUMMARY_FILE"
            log_message "  📊 Grading completed$GRADE_NOTE - check grade file for details"
        fi
        
        SUCCESSFUL_GRADES=$((SUCCESSFUL_GRADES + 1))
        
    elif [ $AUTOGRADER_EXIT_CODE -eq 124 ]; then
        log_message "  ⏱️  ERROR: Autograder timed out (18-second limit)"
        echo >> "$GRADE_FILE"
        echo "ERROR: Autograder timed out after 18 seconds" >> "$GRADE_FILE"
        echo "This usually indicates infinite loops in scheduling algorithm implementations." >> "$GRADE_FILE"
        echo "Common causes:" >> "$GRADE_FILE"
        echo "- Infinite loops in Round Robin queue management" >> "$GRADE_FILE"
        echo "- Incorrect loop conditions in preemptive algorithms" >> "$GRADE_FILE"
        echo "- Missing process completion checks" >> "$GRADE_FILE"
        echo "- Incorrect time advancement in schedulers" >> "$GRADE_FILE"
        echo "- Priority queue implementation issues" >> "$GRADE_FILE"
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
    rm -f STUDENT_OUTPUT.txt scheduler *.o 2>/dev/null
    rm -f expected_*.txt student_*.txt diff_*.txt 2>/dev/null
    
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
echo "Success Rate: $(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))%" >> "$SUMMARY_FILE"
echo >> "$SUMMARY_FILE"
echo "Common Issues Found:" >> "$SUMMARY_FILE"
echo "- Missing README file (required for grading)" >> "$SUMMARY_FILE"
echo "- Missing implementation files (partial grading applied)" >> "$SUMMARY_FILE"
echo "- Infinite loops in scheduling algorithms" >> "$SUMMARY_FILE"
echo "- Incorrect tie-breaking logic implementation" >> "$SUMMARY_FILE"
echo "- Process completion time calculation errors" >> "$SUMMARY_FILE"
echo "- Compilation errors due to syntax or logic issues" >> "$SUMMARY_FILE"
echo "- Round Robin queue management issues" >> "$SUMMARY_FILE"
echo "- Priority scheduling preemption errors" >> "$SUMMARY_FILE"
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
echo "📈 Success Rate: $(( (SUCCESSFUL_GRADES * 100) / TOTAL_STUDENTS ))%"
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