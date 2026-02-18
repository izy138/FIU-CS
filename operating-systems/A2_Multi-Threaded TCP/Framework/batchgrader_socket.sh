#!/bin/bash

# =====================================================================================
# COP4610 Assignment 2 - Client-Server implementation Batch-autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# Description: This script processes all student submissions and generates grade files
# =====================================================================================

cleanup_environment() {
    echo "🧹 Ensuring completely clean environment..."
    
    # Kill any remaining server/client processes from previous runs
    pkill -f './server' 2>/dev/null || true
    pkill -f './client' 2>/dev/null || true
    pkill -f 'nc.*8888' 2>/dev/null || true
    
    # Remove ALL possible log files that could contaminate results
    rm -f *.log server*.log client*.log error*.log timeout*.log protocol*.log 2>/dev/null || true
    rm -f command_test.log help_test.log threading*.log signal*.log 2>/dev/null || true
    rm -f server_compile.log client_compile.log server_thread.log 2>/dev/null || true
    rm -f client1.log client2.log client3.log server_signal.log 2>/dev/null || true
    rm -f server_timeout.log server_error.log server_restart_test.log 2>/dev/null || true
    
    # Remove executables and compilation artifacts
    rm -f server client *.o *.out a.out core 2>/dev/null || true
    
    # Clean temp files that might persist from crashed servers
    rm -f /tmp/shell_output_*.txt 2>/dev/null || true
    
    # Wait for system cleanup and port release
    sleep 5
    
    echo "✅ Clean environment ready"
}

echo "=========================================================================="
echo "     COP4610 Assignment 2 - Client-Server Socket Batch-Autograder        "
echo "=========================================================================="
echo "Date: $(date)"
echo

# Check if test script exists
if [ ! -f "autograder_socket.sh" ]; then
    echo "❌ ERROR: autograder_socket.sh not found in current directory!"
    echo "Please place the test script in the same directory as this batch script."
    exit 1
fi

# Make test script executable
chmod +x autograder_socket.sh

# BULLETPROOF: Ensure clean starting environment
cleanup_environment

# Create results directory
RESULTS_DIR="GRADING_RESULTS"
mkdir -p "$RESULTS_DIR"

# Create summary file
SUMMARY_FILE="$RESULTS_DIR/grading_summary.txt"
echo "COP4610 Assignment 2 - Client-Server Socket Grading Summary" > "$SUMMARY_FILE"
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
    if [[ "$base_name" =~ ^A2_(.+)$ ]]; then
        # Pattern: A2_FirstName_LastName
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
    echo "Supported format: Any .zip file containing server.c and client.c files"
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
    
    # Create temporary directory for this student (with PID for uniqueness)
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
    
    # Look for server.c and client.c files anywhere in the extracted content
    SERVER_FILE=""
    CLIENT_FILE=""
    
    # Search for files recursively
    SERVER_FILE=$(find "$TEMP_DIR" -name "server.c" -type f | head -1)
    CLIENT_FILE=$(find "$TEMP_DIR" -name "client.c" -type f | head -1)
    
    # Check if required files exist
    if [ -z "$SERVER_FILE" ] || [ -z "$CLIENT_FILE" ]; then
        log_message "  ❌ ERROR: Required files not found"
        log_message "  Looking for: server.c and client.c (searched recursively)"
        
        # Show what files were found for debugging
        log_message "  📁 Files found in submission:"
        find "$TEMP_DIR" -name "*.c" -type f | sed 's/^/    /' | tee -a "$LOG_FILE"
        
        echo "$STUDENT_NAME: MISSING_FILES - server.c or client.c not found" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        continue
    fi
    
    # Copy student files to current directory for testing
    log_message "  📋 Copying student files..."
    log_message "    Found server.c at: $SERVER_FILE"
    log_message "    Found client.c at: $CLIENT_FILE"
    
    cp "$SERVER_FILE" ./server.c
    cp "$CLIENT_FILE" ./client.c
    
    # Check for README file recursively (consistent with C files search pattern)
    README_FOUND=false
    README_FILE_FOUND=""

    # Search recursively for README files with priority: PDF > TXT > MD
    found_readme=$(find "$TEMP_DIR" -name "README.pdf" -type f | head -1)
    if [ -z "$found_readme" ]; then
        found_readme=$(find "$TEMP_DIR" -name "README.txt" -type f | head -1)
    fi
    if [ -z "$found_readme" ]; then
        found_readme=$(find "$TEMP_DIR" -name "README.md" -type f | head -1)
    fi

    if [ -n "$found_readme" ]; then
        README_FOUND=true
        README_FILE_FOUND="$found_readme"
        log_message "    ✅ Found README: $(basename "$README_FILE_FOUND")"
    else
        log_message "  ❌ ERROR: Missing README file (required: README.pdf, README.txt, or README.md)"
        echo "$STUDENT_NAME: MISSING_README - Required README file not found" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
        rm -rf "$TEMP_DIR"
        continue
    fi

    # CRITICAL: Clean environment before each autograder run
    log_message "  🔄 Ensuring clean environment..."
    rm -f *.log server*.log client*.log error*.log timeout*.log protocol*.log 2>/dev/null
    rm -f command_test.log help_test.log threading*.log signal*.log 2>/dev/null
    rm -f server_compile.log client_compile.log server_thread.log 2>/dev/null
    rm -f client1.log client2.log client3.log server_signal.log 2>/dev/null
    rm -f server_timeout.log server_error.log server_restart_test.log 2>/dev/null
    rm -f server client *.o *.out a.out core 2>/dev/null
    sync
    sleep 1

    # Run the grading script and capture output
    log_message "  🎯 Running grading script..."
    GRADE_FILE="$RESULTS_DIR/${CLEAN_NAME}_Grade.txt"
    
    {
        echo "=========================================================================="
        echo "                     GRADE REPORT FOR: $STUDENT_NAME"
        echo "=========================================================================="
        echo "Submission File: $zip_file"
        echo "Graded on: $(date)"
        echo "Graded by: Client-Server Socket Autograder"
        echo
        
        echo "Files Submitted:"
        echo "----------------"
        echo "✅ README: $(basename "$README_FILE_FOUND")"
        echo "✅ server.c"
        echo "✅ client.c"
        echo
        
        echo "Grading Results:"
        echo "================"
        echo
    } > "$GRADE_FILE"
    
    # Run the test script and capture both stdout and stderr
    if timeout 180 ./autograder_socket.sh >> "$GRADE_FILE" 2>&1; then
        log_message "  ✅ Grading completed successfully"
        
        # Extract final score from grade file
        FINAL_SCORE=$(grep "TOTAL SCORE:" "$GRADE_FILE" | tail -1 | grep -o '[0-9]\+/[0-9]\+' | head -1)
        PERCENTAGE=$(grep "TOTAL SCORE:" "$GRADE_FILE" | grep -o '[0-9]\+%' | head -1)
        
        if [ -n "$FINAL_SCORE" ]; then
            if [ -n "$PERCENTAGE" ]; then
                echo "$STUDENT_NAME: $FINAL_SCORE ($PERCENTAGE)" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE ($PERCENTAGE)"
                
                # Check for perfect score
                if [[ "$PERCENTAGE" == "100%" ]]; then
                    PERFECT_SCORES=$((PERFECT_SCORES + 1))
                fi
            else
                echo "$STUDENT_NAME: $FINAL_SCORE points" >> "$SUMMARY_FILE"
                log_message "  📊 Score: $FINAL_SCORE points"
            fi
        else
            echo "$STUDENT_NAME: COMPLETED - Check individual grade file" >> "$SUMMARY_FILE"
            log_message "  📊 Grading completed - check grade file for details"
        fi
        
        SUCCESSFUL_GRADES=$((SUCCESSFUL_GRADES + 1))
    else
        log_message "  ❌ ERROR: Grading script failed or timed out"
        echo "End of grading script output" >> "$GRADE_FILE"
        echo >> "$GRADE_FILE"
        echo "ERROR: Grading script failed or timed out (3-minute limit)" >> "$GRADE_FILE"
        echo "$STUDENT_NAME: GRADING_FAILED - Script timeout or error" >> "$SUMMARY_FILE"
        FAILED_GRADES=$((FAILED_GRADES + 1))
    fi
    
    # Clean up temporary files
    # BULLETPROOF: Complete cleanup between students
    log_message "  🧹 Performing thorough cleanup for next student..."
    
    # Remove student temp directory
    chmod -R 755 "$TEMP_DIR" 2>/dev/null || true
    rm -rf "$TEMP_DIR"
    
    # Kill any processes that might be hanging
    pkill -f './server' 2>/dev/null || true
    pkill -f './client' 2>/dev/null || true
    
    # Remove student source files
    rm -f server.c client.c
    
    # Remove executables
    rm -f server client *.o *.out a.out core
    
    # Remove ALL log files (comprehensive patterns)
    rm -f *.log server*.log client*.log error*.log timeout*.log protocol*.log
    rm -f command_test.log help_test.log threading*.log signal*.log
    rm -f server_compile.log client_compile.log server_thread.log
    rm -f client1.log client2.log client3.log server_signal.log
    rm -f server_timeout.log server_error.log server_restart_test.log
    
    # Clean temp files from /tmp
    rm -f /tmp/shell_output_*.txt 2>/dev/null || true
    
    # Wait for complete cleanup
    sleep 1
    
    log_message "  ✅ Environment cleaned for next student"
    
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
echo "- Missing server.c or client.c files" >> "$SUMMARY_FILE"
echo "- Socket binding errors (port already in use)" >> "$SUMMARY_FILE"
echo "- Threading/concurrency issues (race conditions, deadlocks)" >> "$SUMMARY_FILE"
echo "- Signal handling errors (SIGINT, SIGTERM)" >> "$SUMMARY_FILE"
echo "- Protocol implementation errors (incorrect message format)" >> "$SUMMARY_FILE"
echo "- Memory leaks or segmentation faults" >> "$SUMMARY_FILE"
echo "- Timeout issues (infinite loops or blocking operations)" >> "$SUMMARY_FILE"
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