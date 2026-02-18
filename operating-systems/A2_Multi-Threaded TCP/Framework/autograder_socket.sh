#!/bin/bash

# ===============================================================================
# COP4610 Assignment 2 - TCP Client-Server Assignment Autograder
# Author: Dr. Bhargav Bhatkalkar, KFSCIS, Florida International University  
# ===============================================================================

echo "=========================================================================="
echo "     COP4610 Assignment 1 - Shell Server Assignment Autograder            "
echo "=========================================================================="
echo "Date: $(date)"
echo "⚠️⚠️..This should take about 30-45 seconds (we are being patient!)..⚠️⚠️"
echo

# Kill any existing processes that could interfere
pkill -f './server' 2>/dev/null || true
pkill -f './client' 2>/dev/null || true  
pkill -f 'nc.*8888' 2>/dev/null || true

# Remove ALL possible log files from previous runs
rm -f *.log server*.log client*.log error*.log timeout*.log protocol*.log 2>/dev/null || true
rm -f command_test.log help_test.log threading*.log signal*.log 2>/dev/null || true
rm -f server_compile.log client_compile.log server_thread.log 2>/dev/null || true
rm -f client1.log client2.log client3.log server_signal.log 2>/dev/null || true
rm -f server_timeout.log server_error.log server_restart_test.log 2>/dev/null || true

# Remove any executables from previous runs
rm -f server client *.o *.out a.out core 2>/dev/null || true

# Clean temp files
rm -f /tmp/shell_output_*.txt 2>/dev/null || true

# Wait for complete system cleanup
sleep 2

echo "✅ Fresh environment ready - no cross-contamination possible"
echo

# Check if source files exist
if [ ! -f "server.c" ] || [ ! -f "client.c" ]; then
    echo "❌ Source files not found. Make sure server.c and client.c are in this directory."
    echo "Tip: Rename your skeleton files to server.c and client.c for testing!"
    exit 1
fi

# Main testing script begins

# Clean up any previous test files
rm -f *.log server client 2>/dev/null

# Initialize score tracking
TOTAL_SCORE=0
MAX_SCORE=90
    
echo "=========================== TEST 1: COMPILATION (10 points) ==============================="
echo "Testing client-server compilation..."
echo

# Test both compilations with more time and better feedback
gcc server.c -o server -lpthread -Wall -Wextra -Werror -std=c17 2>server_compile_$$.log &
SERVER_COMPILE_PID=$!
gcc client.c -o client -lpthread -Wall -Wextra -Werror -std=c17 2>client_compile.log &
CLIENT_COMPILE_PID=$!

# Wait for both compilations
wait $SERVER_COMPILE_PID
SERVER_COMPILE_RESULT=$?
wait $CLIENT_COMPILE_PID  
CLIENT_COMPILE_RESULT=$?

if [ $SERVER_COMPILE_RESULT -eq 0 ]; then
    echo "✅ Server compilation: 5/5 points"
    SERVER_COMPILE=5
else
    echo "❌ Server compilation: 0/5 points"
    echo "Compilation errors in server.c:"
    cat server_compile_$$.log | head -3
    SERVER_COMPILE=0
fi

if [ $CLIENT_COMPILE_RESULT -eq 0 ]; then
    echo "✅ Client compilation: 5/5 points"
    CLIENT_COMPILE=5
else
    echo "❌ Client compilation: 0/5 points"
    echo "Compilation errors in client.c:"
    cat client_compile.log | head -3
    CLIENT_COMPILE=0
fi

COMPILATION_SCORE=$((SERVER_COMPILE + CLIENT_COMPILE))
TOTAL_SCORE=$((TOTAL_SCORE + COMPILATION_SCORE))

echo
echo "📊 Compilation Score: $COMPILATION_SCORE/10"
[ $COMPILATION_SCORE -eq 10 ] && echo "🎉 Perfect compilation! You are off to a great start!"
[ $COMPILATION_SCORE -eq 5 ] && echo "Half way there! Fix the remaining compilation errors."
[ $COMPILATION_SCORE -eq 0 ] && echo "Do not worry - compilation errors are part of learning!"
echo

# Only continue if at least one file compiles
if [ $COMPILATION_SCORE -eq 0 ]; then
    echo "Cannot run tests - let us fix compilation first!"
    echo "Check the TODO comments in your skeleton files for guidance."
    echo "⭐ Current Progress: $TOTAL_SCORE/$MAX_SCORE points"
    exit 1
fi
     
# Check if port 8888 is available
if lsof -i:8888 >/dev/null 2>&1; then
    echo "⚠️ Port 8888 in use, cleaning up..."
    pkill -f ':8888' 2>/dev/null || true
    sleep 2
fi

echo "======================== TEST 2: BASIC FUNCTIONALITY (25 points) =========================="
echo "Testing core client-server communication..."
echo

# Test server startup with more patience
timeout 20 ./server > server.log 2>&1 &
SERVER_PID=$!
echo "Starting server (being patient)..."
sleep 3  # More generous startup time

BASIC_SCORE=0

if kill -0 $SERVER_PID 2>/dev/null; then
    echo "✅ Server startup: 5/5 points - Server is running!"
    BASIC_SCORE=5
else
    echo "❌ Server startup: 0/5 points"
    echo "Server failed to start. Common issues:"
    echo "   • Check if port 8888 is already in use"
    echo "   • Verify socket creation and binding code"
    echo "   • Look at server.log for error messages"
    head -3 server.log 2>/dev/null
    BASIC_SCORE=0
fi

# Test basic client connection with patience
if [ $BASIC_SCORE -eq 5 ]; then
    echo "Testing client connection..."
    echo 'EXIT' | timeout 15 ./client > client.log 2>&1
    CLIENT_RESULT=$?
    
    if [ $CLIENT_RESULT -eq 0 ]; then
        echo "✅ Client connection: 8/8 points - Perfect connection!"
        BASIC_SCORE=$((BASIC_SCORE + 8))
    elif grep -q -i 'connect' client.log; then
        echo "Partial: Client connection: 4/8 points - Partial connection detected!"
        echo "Tip: Check client TODO 1 (connect_to_server function)"
        BASIC_SCORE=$((BASIC_SCORE + 4))
    else
        echo "❌ Client connection: 0/8 points"
        echo "Client failed to connect. Check:"
        echo "   • Server must be running first"
        echo "   • Client connect_to_server function"
        echo "   • IP address and port configuration"
    fi
fi

# Test command execution with generous timeout
if [ $BASIC_SCORE -ge 9 ]; then
    echo "Testing command execution..."
    echo -e 'echo "Hello World"\nEXIT' | timeout 10 ./client > command_test.log 2>&1
    
    if grep -q 'Hello World' command_test.log; then
        echo "✅ Command execution: 7/7 points - Commands working perfectly!"
        BASIC_SCORE=$((BASIC_SCORE + 7))
    elif grep -q -i 'hello\|world' command_test.log; then
        echo "Partial: Command execution: 4/7 points - Partial command output detected!"
        echo "Tip: Check server TODO 3 (execute_shell_command)"
        BASIC_SCORE=$((BASIC_SCORE + 4))
    elif [ -s command_test.log ]; then
        echo "Partial: Command execution: 2/7 points - Server responding but command issues"
        echo "Tip: Verify fork() and execlp() implementation"
        BASIC_SCORE=$((BASIC_SCORE + 2))
    else
        echo "❌ Command execution: 0/7 points"
        echo "No command output detected. Check TODO 3 and 4 in server."
    fi
fi

# Test HELP command gently
if [ $BASIC_SCORE -ge 10 ]; then
    echo "Testing HELP command..."
    echo -e 'HELP\nEXIT' | timeout 8 ./client > help_test.log 2>&1
    
    if grep -q -i 'help\|command\|available' help_test.log; then
        echo "✅ HELP command: 2/2 points - Great user experience!"
        BASIC_SCORE=$((BASIC_SCORE + 2))
    else
        echo "Partial: HELP command: 1/2 points - Basic functionality works!"
        echo "Tip: Enhance HELP response in handle_user_command"
        BASIC_SCORE=$((BASIC_SCORE + 1))
    fi
fi

# Test protocol compliance for response end markers
if [ $BASIC_SCORE -ge 12 ]; then
    echo "Testing protocol compliance (response end markers)..."
    PROTOCOL_POINTS=0
    
    # Use netcat to test raw server response (reuse existing server)
    if command -v nc >/dev/null 2>&1 && kill -0 $SERVER_PID 2>/dev/null; then
        echo -e "HELP\nEXIT" | timeout 5 nc 127.0.0.1 8888 > protocol_raw_test.log 2>&1
        if grep -q '<END_OF_RESPONSE>' protocol_raw_test.log; then
            echo "✅ Protocol compliance: 3/3 points - Response end markers present"
            PROTOCOL_POINTS=3
        else
            echo "❌ Protocol compliance: 0/3 points - Missing <END_OF_RESPONSE> markers"
            echo "CRITICAL: All server responses must end with <END_OF_RESPONSE>"
        fi
        rm -f protocol_raw_test.log
    else
        # Fallback: Since client removes markers, assume compliance if responses work
        if [ -s command_test.log ] && [ -s help_test.log ]; then
            echo "✅ Protocol compliance: 3/3 points - Client successfully processes server responses"
            echo "Note: Client removes <END_OF_RESPONSE> markers after receiving them (good design)"
            PROTOCOL_POINTS=3
        else
            echo "❌ Protocol compliance: 0/3 points - Protocol compliance unclear"
        fi
    fi
    
    # Adjust basic functionality score based on protocol compliance
    BASIC_SCORE=$((BASIC_SCORE + PROTOCOL_POINTS))
    if [ $PROTOCOL_POINTS -eq 0 ]; then
        echo "Points deducted from Basic Functionality for protocol violation"
    fi
fi

# Clean up server gently
kill $SERVER_PID 2>/dev/null
sleep 2
kill -9 $SERVER_PID 2>/dev/null  # Force kill if needed
wait $SERVER_PID 2>/dev/null

TOTAL_SCORE=$((TOTAL_SCORE + BASIC_SCORE))
echo
echo "📊 Basic Functionality Score: $BASIC_SCORE/25"
[ $BASIC_SCORE -ge 20 ] && echo "🎉 Excellent basic functionality!"
[ $BASIC_SCORE -ge 10 ] && [ $BASIC_SCORE -lt 20 ] && echo "Good progress on basic features!"
[ $BASIC_SCORE -lt 10 ] && echo "Keep working on the core functionality - you are learning!"
echo
  
echo "=========================== TEST 3: THREADING (25 points) ================================="
echo "Testing if your server can handle multiple clients..."
echo

# Test concurrent clients with more patience
timeout 20 ./server > server_thread.log 2>&1 &
SERVER_PID=$!
sleep 5  # More generous startup time

THREADING_SCORE=0

if kill -0 $SERVER_PID 2>/dev/null; then
    echo "Testing 3 clients simultaneously (this takes a moment)..."
    
    # Start 3 clients with generous timeouts and better spacing
    echo 'ls' | timeout 15 ./client > client1.log 2>&1 &
    CLIENT1_PID=$!
    sleep 2  # Increased delay
    echo 'pwd' | timeout 12 ./client > client2.log 2>&1 &
    CLIENT2_PID=$!
    sleep 2  # Increased delay
    echo 'date' | timeout 12 ./client > client3.log 2>&1 &
    CLIENT3_PID=$!
    
    # Wait for all clients patiently
    wait $CLIENT1_PID
    wait $CLIENT2_PID  
    wait $CLIENT3_PID
    
    # Check success with more lenient criteria
    SUCCESS_COUNT=0
    if [ -s client1.log ] && ! grep -q -i 'connection.*refused\|connection.*failed\|cannot.*connect' client1.log; then
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    fi
    if [ -s client2.log ] && ! grep -q -i 'connection.*refused\|connection.*failed\|cannot.*connect' client2.log; then
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    fi
    if [ -s client3.log ] && ! grep -q -i 'connection.*refused\|connection.*failed\|cannot.*connect' client3.log; then
        SUCCESS_COUNT=$((SUCCESS_COUNT + 1))
    fi
    
    # Analyze server logs for threading evidence
    echo "Analyzing server logs for threading evidence..."
    THREAD_EVIDENCE=0
    
    # Check for thread creation messages
    if grep -q -i "Created thread\|Thread.*Client connected\|Thread.*Command from" server_thread.log; then
        THREAD_EVIDENCE=1
        echo "Threading evidence found in server logs!"
    else
        echo "No clear threading evidence in server logs"
        echo "Tip: Add printf statements showing thread IDs in your server"
    fi
    
    # Scoring based on client success and threading evidence
    if [ $SUCCESS_COUNT -eq 3 ]; then
        echo "✅ Multiple clients: 25/25 points - Perfect threading!"
        THREADING_SCORE=25
    elif [ $SUCCESS_COUNT -eq 2 ]; then
        echo "🎉 Multiple clients: 23/25 points - Great! 2/3 clients worked!"
        THREADING_SCORE=23
    elif [ $SUCCESS_COUNT -eq 1 ]; then
        echo "Partial: Multiple clients: 18/25 points - Good start! 1/3 clients worked!"
        THREADING_SCORE=18
    else
        # Check if server at least accepts connections
        if grep -q -i 'connected\|accept' server_thread.log; then
            echo "Partial: Multiple clients: 12/25 points - Server accepts connections!"
            THREADING_SCORE=12
        else
            echo "❌ Multiple clients: 5/25 points - Server running but connection issues"
            THREADING_SCORE=5
        fi
    fi
else
    echo "❌ Multiple clients: 0/25 points (server not running)"
    echo "Server crashed. Check basic functionality first."
    THREADING_SCORE=0
fi

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

TOTAL_SCORE=$((TOTAL_SCORE + THREADING_SCORE))
echo
echo "📊 Threading Score: $THREADING_SCORE/25"
[ $THREADING_SCORE -ge 20 ] && echo "🏆 Excellent threading implementation!"
[ $THREADING_SCORE -ge 10 ] && [ $THREADING_SCORE -lt 20 ] && echo "Good threading progress!"
[ $THREADING_SCORE -lt 10 ] && echo "Threading is tricky - keep practicing!"
echo
    
echo "======================== TEST 4: TIMEOUT HANDLING (10 points) ============================="
echo "Testing if commands timeout after 3 seconds..."
echo

# Test command timeout with generous margins
timeout 15 ./server > server_timeout.log 2>&1 &
SERVER_PID=$!
sleep 3

TIMEOUT_SCORE=0

if kill -0 $SERVER_PID 2>/dev/null; then
    echo "Testing sleep command (should timeout)..."
    
    # Use higher precision timing when available
    if command -v python3 >/dev/null 2>&1; then
        # Use Python for high precision timing
        start_time=$(python3 -c "import time; print(time.time())")
        echo 'sleep 5' | timeout 12 ./client > timeout_test.log 2>&1
        end_time=$(python3 -c "import time; print(time.time())")
        duration=$(python3 -c "print(round($end_time - $start_time, 2))")
        duration_int=$(python3 -c "print(int($end_time - $start_time))")
    else
        # Fallback to date
        start_time=$(date +%s)
        echo 'sleep 5' | timeout 12 ./client > timeout_test.log 2>&1
        end_time=$(date +%s)
        duration=$((end_time - start_time))
        duration_int=$duration
    fi
    
    echo "Measured duration: $duration seconds"
    
    # Check for timeout evidence in multiple ways
    timeout_detected=0
    timeout_message=""
    if grep -q -i 'timeout\|exceeded.*second\|killed\|timed.*out' timeout_test.log; then
        timeout_detected=1
        timeout_message=$(grep -i 'timeout\|exceeded.*second\|killed\|timed.*out' timeout_test.log | head -1)
        echo "✅ Timeout message detected in client output"
        echo "   Message: \"$timeout_message\""
    else
        echo "⚠️ No timeout keywords detected in client output"
        if [ -s timeout_test.log ]; then
            echo "   Client output received:"
            echo "   $(head -3 timeout_test.log | sed 's/^/   > /')"
            echo "   Expected keywords: 'timeout', 'exceeded', 'killed', 'timed out'"
        else
            echo "   No client output captured (possible connection issue)"
        fi
    fi
    
    # Fixed timeout scoring - prioritize timing performance, then message quality
    if [ "$duration_int" -le 5 ]; then
        if [ $timeout_detected -eq 1 ]; then
            echo "✅ Command timeout: 10/10 points - Perfect timeout handling!"
            echo "Note: Excellent timing with proper timeout message"
            TIMEOUT_SCORE=10
        else
            echo "✅ Command timeout: 9/10 points - Excellent timing!"
            echo "Note: Great performance, consider adding clearer timeout message"
            TIMEOUT_SCORE=9
        fi
    elif [ "$duration_int" -le 6 ]; then
        if [ $timeout_detected -eq 1 ]; then
            echo "✅ Command timeout: 10/10 points - Perfect timeout handling!"
            echo "Note: Implementation correctly enforces 3-second timeout"
            TIMEOUT_SCORE=10
        else
            echo "🎉 Command timeout: 8/10 points - Very good timing!"
            echo "Tip: Add timeout message with keywords like 'timeout' or 'exceeded'"
            TIMEOUT_SCORE=8
        fi
    elif [ "$duration_int" -le 7 ]; then
        if [ $timeout_detected -eq 1 ]; then
            echo "✅ Command timeout: 9/10 points - Excellent timeout implementation!"
            echo "Note: Minor timing variation is acceptable in system programming"
            TIMEOUT_SCORE=9
        else
            echo "🎉 Command timeout: 7/10 points - Good timing, improve message!"
            echo "Tip: Add clear timeout message and optimize response speed"
            TIMEOUT_SCORE=7
        fi
    elif [ "$duration_int" -le 8 ]; then
        if [ $timeout_detected -eq 1 ]; then
            echo "🎉 Command timeout: 8/10 points - Very good timeout implementation!"
            echo "Tip: Optimize post-timeout response sending for faster results"
            TIMEOUT_SCORE=8
        else
            echo "Partial: Command timeout: 6/10 points - Timeout works but needs optimization!"
            echo "Tip: Improve both timing and timeout message format"
            TIMEOUT_SCORE=6
        fi
    elif [ "$duration_int" -le 10 ]; then
        if [ $timeout_detected -eq 1 ]; then
            echo "Partial: Command timeout: 6/10 points - Timeout detected but slow response!"
            echo "Tip: Minimize delays after timeout detection and killing process"
            TIMEOUT_SCORE=6
        else
            echo "Partial: Command timeout: 4/10 points - Some timeout mechanism present"
            echo "Tip: Check timeout implementation and message format"
            TIMEOUT_SCORE=4
        fi
    else
        echo "❌ Command timeout: 2/10 points - Commands run too long without timeout"
        echo "Tip: Implement 3-second timeout using sleep() and kill() in server"
        TIMEOUT_SCORE=2
    fi
else
    echo "❌ Command timeout: 0/10 points (server not running)"
    TIMEOUT_SCORE=0
fi

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

TOTAL_SCORE=$((TOTAL_SCORE + TIMEOUT_SCORE))
echo
echo "📊 Timeout Score: $TIMEOUT_SCORE/10"
[ $TIMEOUT_SCORE -ge 8 ] && echo "🏆 Great timeout implementation!"
[ $TIMEOUT_SCORE -ge 5 ] && [ $TIMEOUT_SCORE -lt 8 ] && echo "Timeout partially working!"
[ $TIMEOUT_SCORE -lt 5 ] && echo "Timeouts need work - check TODO 3!"
echo

echo "=========================== TEST 5: SIGNAL HANDLING (10 points) ==========================="
echo "Testing Ctrl+C (SIGINT) graceful shutdown..."
echo

SIGNAL_SCORE=0

# Test server graceful shutdown with very generous timing
timeout 20 ./server > server_signal.log 2>&1 &
SERVER_PID=$!
sleep 3  # Extra startup time

if kill -0 $SERVER_PID 2>/dev/null; then
    echo "Testing graceful shutdown with SIGINT..."
    
    # Send SIGINT to server (Ctrl+C simulation)
    kill -INT $SERVER_PID 2>/dev/null
    
    # Give server generous time to clean up
    CLEANUP_TIMEOUT=5
    CLEANUP_COUNT=0
    SERVER_SHUTDOWN=0
    
    while [ $CLEANUP_COUNT -lt $CLEANUP_TIMEOUT ]; do
        if ! kill -0 $SERVER_PID 2>/dev/null; then
            SERVER_SHUTDOWN=1
            break
        fi
        sleep 1
        CLEANUP_COUNT=$((CLEANUP_COUNT + 1))
    done
    
    if [ $SERVER_SHUTDOWN -eq 1 ]; then
        echo "✅ Graceful shutdown: 6/6 points - Server responds to Ctrl+C!"
        SIGNAL_SCORE=6
        
        # Check for cleanup messages (generous criteria)
        if grep -q -i 'shutdown\|cleanup\|graceful\|signal\|shutting' server_signal.log; then
            echo "✅ Cleanup messages: 3/3 points - Great user feedback!"
            SIGNAL_SCORE=$((SIGNAL_SCORE + 3))
        else
            echo "Partial: Cleanup messages: 2/3 points - Shutdown works!"
            echo "Tip: Add informative shutdown messages"
            SIGNAL_SCORE=$((SIGNAL_SCORE + 2))
        fi
        
        # Test server restart capability (generous)
        echo "Testing if server can restart cleanly..."
        timeout 8 ./server > server_restart_test.log 2>&1 &
        RESTART_PID=$!
        sleep 3
        
        if kill -0 $RESTART_PID 2>/dev/null; then
            echo "✅ Resource cleanup: 1/1 points - Perfect cleanup!"
            SIGNAL_SCORE=$((SIGNAL_SCORE + 1))
            kill $RESTART_PID 2>/dev/null
            wait $RESTART_PID 2>/dev/null
        else
            if grep -q -i 'address.*already.*in.*use' server_restart_test.log; then
                echo "Partial: Resource cleanup: 0/1 points - Socket not fully cleaned"
                echo "Tip: Use SO_REUSEADDR and close() in cleanup"
            else
                echo "✅ Resource cleanup: 1/1 points - Restart issue unrelated to cleanup"
                SIGNAL_SCORE=$((SIGNAL_SCORE + 1))
            fi
        fi
    else
        echo "Partial: Graceful shutdown: 3/6 points - Server running but no SIGINT response"
        echo "Tip: Implement signal handler in TODO 6 (cleanup_and_exit)"
        SIGNAL_SCORE=3
        # Force kill the server
        kill -KILL $SERVER_PID 2>/dev/null
        wait $SERVER_PID 2>/dev/null
    fi
else
    echo "❌ Signal handling: 0/10 points (server not running)"
fi

TOTAL_SCORE=$((TOTAL_SCORE + SIGNAL_SCORE))
echo
echo "📊 Signal Handling Score: $SIGNAL_SCORE/10"
[ $SIGNAL_SCORE -ge 8 ] && echo "🏆 Excellent signal handling!"
[ $SIGNAL_SCORE -ge 5 ] && [ $SIGNAL_SCORE -lt 8 ] && echo "Good signal handling progress!"
[ $SIGNAL_SCORE -lt 5 ] && echo "Signal handling needs work - check TODO 6!"
echo

echo "========================== TEST 6: ERROR HANDLING (10 points) ============================="
echo "Testing how well your server handles errors and edge cases..."
echo

ERROR_SCORE=0

# Test invalid command handling gently
timeout 12 ./server > server_error.log 2>&1 &
SERVER_PID=$!
sleep 3

if kill -0 $SERVER_PID 2>/dev/null; then
    echo "Testing invalid command handling..."
    echo -e 'invalidcommand123\nEXIT' | timeout 8 ./client > error_test.log 2>&1
    
    if grep -q -i 'command not found\|not found\|error' error_test.log; then
        echo "✅ Invalid command handling: 10/10 points - Perfect error handling!"
        ERROR_SCORE=10
    elif [ -s error_test.log ]; then
        echo "Partial: Invalid command handling: 7/10 points - Server responds to invalid commands!"
        ERROR_SCORE=7
    else
        echo "Partial: Invalid command handling: 3/10 points - Server running but response issues"
        ERROR_SCORE=3
    fi
else
    echo "❌ Invalid command handling: 0/10 points (server not running)"
fi

kill $SERVER_PID 2>/dev/null
wait $SERVER_PID 2>/dev/null

TOTAL_SCORE=$((TOTAL_SCORE + ERROR_SCORE))
echo
echo "📊 Error Handling Score: $ERROR_SCORE/10"
[ $ERROR_SCORE -ge 8 ] && echo "🏆 Excellent error handling!"
[ $ERROR_SCORE -ge 5 ] && [ $ERROR_SCORE -lt 8 ] && echo "Good error handling!"
[ $ERROR_SCORE -lt 5 ] && echo "Error handling needs improvement!"
echo

# Final results  
echo ""
echo "=================================================================="
echo "              AUTOGRADER REPORT"
echo "=================================================================="
echo "Date: $(date)"   
echo ""
echo "🏆 TOTAL SCORE: $TOTAL_SCORE/$MAX_SCORE points"
echo "📊 MANUAL GRADING: Code Quality (10 points) - graded by instructor"
echo "📊 TOTAL POSSIBLE: 100 points"

percentage=$((TOTAL_SCORE * 100 / MAX_SCORE))
echo "📊 AUTOMATED PERCENTAGE: $percentage%"
echo

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

echo "📈 Score Breakdown:"
echo "   📚 Compilation: $COMPILATION_SCORE/10"
echo "   🏗️ Basic Functionality: $BASIC_SCORE/25"
echo "   🧵 Threading: $THREADING_SCORE/25"
echo "   ⏰ Timeout Handling: $TIMEOUT_SCORE/10"
echo "   🛡️ Signal Handling: $SIGNAL_SCORE/10"
echo "   🛠️ Error Handling: $ERROR_SCORE/10"
echo "   💻 Code Quality: 10 points (manual grading by instructor)"
echo

if [ $TOTAL_SCORE -lt $MAX_SCORE ]; then
    echo
    echo "🚀 Next Steps to Improve:"
    [ $COMPILATION_SCORE -lt 10 ] && echo "   1️⃣ Fix compilation errors first"
    [ $BASIC_SCORE -lt 25 ] && echo "   2️⃣ Focus on basic client-server communication"
    [ $THREADING_SCORE -lt 25 ] && echo "   3️⃣ Work on threading implementation"
    [ $TIMEOUT_SCORE -lt 10 ] && echo "   4️⃣ Implement command timeout handling"
    [ $SIGNAL_SCORE -lt 10 ] && echo "   5️⃣ Add graceful shutdown with signal handling"
    [ $ERROR_SCORE -lt 10 ] && echo "   6️⃣ Improve error handling"
fi

# Testing completed
echo

# BULLETPROOF: Final cleanup to prevent contamination of next run
echo "🧹 Final cleanup to ensure no contamination..."

# Kill any remaining processes
pkill -f './server' 2>/dev/null || true
pkill -f './client' 2>/dev/null || true

# Remove all generated files
rm -f *.log server*.log client*.log error*.log timeout*.log protocol*.log 2>/dev/null || true
rm -f command_test.log help_test.log threading*.log signal*.log 2>/dev/null || true
rm -f server_compile.log client_compile.log server_thread.log 2>/dev/null || true
rm -f client1.log client2.log client3.log server_signal.log 2>/dev/null || true
rm -f server_timeout.log server_error.log server_restart_test.log 2>/dev/null || true

# Remove executables
rm -f server client *.o *.out a.out core 2>/dev/null || true

# Clean temp files
rm -f /tmp/shell_output_*.txt 2>/dev/null || true

echo "✅ Complete cleanup finished"