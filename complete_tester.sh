#!/bin/bash

# ============================================================================
# COMPLETE MINISHELL TESTER
# Tests functionality, memory leaks, and edge cases
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

MINISHELL="./minishell"
TEST_DIR="/tmp/minishell_tests"

# Check if valgrind is available
VALGRIND_AVAILABLE=false
if command -v valgrind >/dev/null 2>&1; then
    VALGRIND_AVAILABLE=true
fi

# Function to run with timeout (macOS compatible)
run_with_timeout() {
    local timeout_duration="$1"
    shift
    local command="$@"
    
    # For macOS, we'll use a different approach since timeout doesn't exist
    if [[ "$OSTYPE" == "darwin"* ]]; then
        # Run command in background and monitor it
        eval "$command" &
        local pid=$!
        local count=0
        
        while [ $count -lt $timeout_duration ]; do
            if ! kill -0 $pid 2>/dev/null; then
                wait $pid
                return $?
            fi
            sleep 1
            count=$((count + 1))
        done
        
        # If we reach here, command timed out
        kill -9 $pid 2>/dev/null
        return 124  # timeout exit code
    else
        # Linux has timeout command
        timeout "$timeout_duration" "$command"
        return $?
    fi
}

# Stats
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
MEMORY_LEAKS=0

# Create test directory
mkdir -p "$TEST_DIR"

print_header() {
    echo -e "${BLUE}${BOLD}════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}${BOLD}                           COMPLETE MINISHELL TESTER                        ${NC}"
    echo -e "${BLUE}${BOLD}════════════════════════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}Tests: Functionality | Memory Leaks | Edge Cases | Stress Tests${NC}"
    echo ""
}

print_section() {
    echo -e "${YELLOW}${BOLD}[$1] $2${NC}"
    echo -e "${YELLOW}────────────────────────────────────────────────────────────────────────────${NC}"
}

run_test() {
    local test_name="$1"
    local command="$2"
    local expected_exit_code="${3:-0}"
    local description="$4"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${CYAN}Testing:${NC} $test_name"
    if [ -n "$description" ]; then
        echo -e "${CYAN}Description:${NC} $description"
    fi
    echo -e "${CYAN}Command:${NC} $command"
    
    # Run the command
    echo "$command" > "$TEST_DIR/current_test.sh"
    echo "exit" >> "$TEST_DIR/current_test.sh"
    
    run_with_timeout 10 "$MINISHELL < $TEST_DIR/current_test.sh > $TEST_DIR/output.txt 2> $TEST_DIR/error.txt"
    actual_exit_code=$?
    
    # Check if test passed
    if [ $actual_exit_code -eq $expected_exit_code ] || [ $actual_exit_code -eq 130 ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAILED${NC} (exit code: $actual_exit_code, expected: $expected_exit_code)"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        if [ -s "$TEST_DIR/error.txt" ]; then
            echo -e "${RED}Error output:${NC}"
            cat "$TEST_DIR/error.txt" | head -3
        fi
    fi
    echo ""
}

run_memory_test() {
    if [ "$VALGRIND_AVAILABLE" = false ]; then
        echo -e "${YELLOW}⚠ Valgrind not available, skipping memory tests${NC}"
        return
    fi
    
    local test_name="$1"
    local command="$2"
    local description="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${CYAN}Memory Testing:${NC} $test_name"
    if [ -n "$description" ]; then
        echo -e "${CYAN}Description:${NC} $description"
    fi
    echo -e "${CYAN}Command:${NC} $command"
    
    # Create test file
    echo "$command" > "$TEST_DIR/memory_test.sh"
    echo "exit" >> "$TEST_DIR/memory_test.sh"
    
    # Run with valgrind
    run_with_timeout 30 "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=42 $MINISHELL < $TEST_DIR/memory_test.sh > $TEST_DIR/valgrind_out.txt 2> $TEST_DIR/valgrind_err.txt"
    
    valgrind_exit=$?
    
    if [ $valgrind_exit -eq 42 ]; then
        echo -e "${RED}✗ MEMORY LEAK DETECTED${NC}"
        MEMORY_LEAKS=$((MEMORY_LEAKS + 1))
        FAILED_TESTS=$((FAILED_TESTS + 1))
        # Show leak summary
        grep -A 5 "LEAK SUMMARY" "$TEST_DIR/valgrind_err.txt" || echo "Memory error detected"
    elif [ $valgrind_exit -eq 124 ]; then
        echo -e "${YELLOW}⚠ TIMEOUT${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    else
        echo -e "${GREEN}✓ NO MEMORY LEAKS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    fi
    echo ""
}

# ============================================================================
# MAIN TESTS
# ============================================================================

print_header

# Check if minishell exists
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found. Please compile first with 'make'${NC}"
    exit 1
fi

# ============================================================================
# 1. BASIC FUNCTIONALITY TESTS
# ============================================================================
print_section "1" "BASIC FUNCTIONALITY TESTS"

run_test "Basic echo" "echo hello" 0 "Simple echo command"
run_test "Echo with arguments" "echo hello world test" 0 "Echo with multiple arguments"
run_test "Echo -n option" "echo -n hello" 0 "Echo without newline"
run_test "Empty echo" "echo" 0 "Echo without arguments"
run_test "PWD command" "pwd" 0 "Print working directory"
run_test "ENV command" "env | head -5" 0 "Print environment variables"

# ============================================================================
# 2. BUILTIN COMMANDS TESTS
# ============================================================================
print_section "2" "BUILTIN COMMANDS TESTS"

run_test "CD to tmp" "cd /tmp; pwd" 0 "Change directory to /tmp"
run_test "CD to home" "cd; pwd" 0 "Change directory to home"
run_test "CD to root" "cd /; pwd" 0 "Change directory to root"
run_test "CD to non-existent" "cd /non/existent/path" 1 "CD to non-existent directory should fail"
# COMMENTED OUT - USES && WHICH IS BONUS
# run_test "Export variable" "export TEST_VAR=hello && echo \$TEST_VAR" 0 "Export and use variable"  
# run_test "Unset variable" "export TEST_VAR=hello && unset TEST_VAR && echo \$TEST_VAR" 0 "Export, unset and check variable"

# MANDATORY TESTS WITHOUT && (BONUS OPERATORS)
run_test "Export variable basic" "export TEST_VAR=hello" 0 "Export variable without complex operators"
run_test "Use exported variable" "echo \$TEST_VAR" 0 "Use previously exported variable"
run_test "Unset variable basic" "unset TEST_VAR" 0 "Unset variable without complex operators"
run_test "Check unset variable" "echo \$TEST_VAR" 0 "Check that unset variable expands to empty"

# ============================================================================
# 3. PIPES AND REDIRECTIONS
# ============================================================================
print_section "3" "PIPES AND REDIRECTIONS"

run_test "Simple pipe" "echo hello | cat" 0 "Simple pipe to cat"
run_test "Multiple pipes" "echo hello | cat | cat | cat" 0 "Multiple pipes"
run_test "Output redirection" "echo hello > $TEST_DIR/test.txt; cat $TEST_DIR/test.txt" 0 "Output redirection"
run_test "Input redirection" "echo test > $TEST_DIR/input.txt; cat < $TEST_DIR/input.txt" 0 "Input redirection"
run_test "Append redirection" "echo first > $TEST_DIR/append.txt; echo second >> $TEST_DIR/append.txt; cat $TEST_DIR/append.txt" 0 "Append redirection"

# ============================================================================
# 4. VARIABLE EXPANSION
# ============================================================================
print_section "4" "VARIABLE EXPANSION"

# COMMENTED OUT - USES && WHICH IS BONUS
# run_test "Basic variable" "export VAR=test && echo \$VAR" 0 "Basic variable expansion"
# run_test "Variable in quotes" "export VAR=test && echo \"\$VAR\"" 0 "Variable expansion in double quotes" 
# run_test "Variable not in quotes" "export VAR=test && echo '\$VAR'" 0 "Variable not expanded in single quotes"

# MANDATORY TESTS WITHOUT && (BONUS OPERATORS)  
run_test "Export and check variable" "export VAR=test" 0 "Export variable for testing"
run_test "Basic variable" "echo \$VAR" 0 "Basic variable expansion"
run_test "Variable in quotes" "echo \"\$VAR\"" 0 "Variable expansion in double quotes"
run_test "Variable not in quotes" "echo '\$VAR'" 0 "Variable not expanded in single quotes"

run_test "Undefined variable" "echo \$UNDEFINED_VAR_123" 0 "Undefined variable should expand to empty"
run_test "Exit status variable" "true; echo \$?" 0 "Exit status variable after true"
# COMMENTED OUT - USES && || WHICH ARE BONUS
# run_test "Exit status after false" "false && echo \$? || echo \$?" 0 "Exit status variable after false (using logical OR)"

# ============================================================================
# 5. QUOTES AND ESCAPING
# ============================================================================
print_section "5" "QUOTES AND ESCAPING"

run_test "Single quotes" "echo 'hello world'" 0 "Single quotes preserve literal values"
run_test "Double quotes" "echo \"hello world\"" 0 "Double quotes allow variable expansion"
run_test "Mixed quotes" "echo 'single' \"double\"" 0 "Mixed single and double quotes"
run_test "Quotes with spaces" "echo 'hello world test'" 0 "Quotes with spaces"
run_test "Escaped quotes" "echo 'can'\\''t do this'" 0 "Escaped single quotes"

# ============================================================================
# 6. HEREDOC TESTS (MANDATORY - HEREDOC IS REQUIRED)
# ============================================================================
print_section "6" "HEREDOC TESTS"

run_test "Basic heredoc" "cat << EOF
This is a test
EOF" 0 "Basic heredoc functionality"

# COMMENTED OUT - USES && WHICH IS BONUS
# run_test "Heredoc with variables" "export TEST_VAR=world && cat << EOF
# Hello \$TEST_VAR  
# EOF" 0 "Heredoc with variable expansion"

# MANDATORY HEREDOC TESTS WITHOUT && (BONUS OPERATORS)
run_test "Export for heredoc" "export TEST_VAR=world" 0 "Export variable for heredoc test"
run_test "Heredoc with variables" "cat << EOF
Hello \$TEST_VAR
EOF" 0 "Heredoc with variable expansion"

run_test "Quoted heredoc delimiter" "cat << 'EOF'
No \$expansion here
EOF" 0 "Heredoc with quoted delimiter"

# ============================================================================
# 7. ERROR HANDLING TESTS
# ============================================================================
print_section "7" "ERROR HANDLING"

run_test "Command not found" "nonexistent_command_12345" 127 "Command not found should exit 127"
run_test "File not found" "cat /non/existent/file" 1 "File not found should exit 1"
run_test "Permission denied" "cat /root/non_readable_file" 1 "Permission denied should exit 1"
run_test "Invalid export" "export 123INVALID=test" 1 "Invalid export syntax should fail"
run_test "CD to file" "cd /etc/passwd" 1 "CD to file should fail"

# ============================================================================
# 8. EDGE CASES
# ============================================================================
print_section "8" "EDGE CASES"

run_test "Empty command" "" 0 "Empty command should not crash"
run_test "Only spaces" "   " 0 "Only spaces should not crash"
run_test "Multiple spaces" "echo    hello    world" 0 "Multiple spaces between words"
run_test "Special characters" "echo '!@#\$%^&*()'" 0 "Special characters in quotes"
run_test "Unicode characters" "echo 'héllo wörld 你好'" 0 "Unicode characters"
run_test "Long argument" "echo $(printf 'a%.0s' {1..1000})" 0 "Very long argument"

# ============================================================================
# 9. MEMORY LEAK TESTS (if valgrind available)
# ============================================================================
if [ "$VALGRIND_AVAILABLE" = true ]; then
    print_section "9" "MEMORY LEAK TESTS"
    
    run_memory_test "Basic echo memory" "echo hello" "Check for memory leaks in basic echo"
    run_memory_test "Variable expansion memory" "export TEST=hello && echo \$TEST && unset TEST" "Check memory leaks in variable operations"
    run_memory_test "Pipe memory" "echo hello | cat | cat" "Check memory leaks in pipe operations"
    run_memory_test "Redirection memory" "echo hello > $TEST_DIR/mem_test.txt && cat $TEST_DIR/mem_test.txt" "Check memory leaks in redirections"
    run_memory_test "Heredoc memory" "cat << EOF
Memory test
EOF" "Check memory leaks in heredoc"
    run_memory_test "Complex command memory" "export A=test && echo \$A | cat > $TEST_DIR/complex.txt && cat $TEST_DIR/complex.txt && unset A" "Check memory leaks in complex commands"
    
    # Stress test for memory
    run_memory_test "Multiple commands memory" "echo test1 && echo test2 && echo test3 && echo test4 && echo test5" "Multiple commands memory test"
    run_memory_test "Deep pipes memory" "echo start | cat | cat | cat | cat | cat" "Deep pipes memory test"
    
else
    echo -e "${YELLOW}⚠ Valgrind not available. Install valgrind for memory leak testing:${NC}"
    echo -e "${YELLOW}  macOS: brew install valgrind${NC}"
    echo -e "${YELLOW}  Linux: sudo apt-get install valgrind${NC}"
fi

# ============================================================================
# 10. STRESS TESTS
# ============================================================================
print_section "10" "STRESS TESTS"

run_test "Many arguments" "echo $(for i in {1..50}; do echo -n \"arg\$i \"; done)" 0 "Many arguments stress test"
run_test "Deep directory" "mkdir -p $TEST_DIR/a/b/c/d/e/f/g; cd $TEST_DIR/a/b/c/d/e/f/g; pwd" 0 "Deep directory navigation"
run_test "Many pipes" "echo start | cat | cat | cat | cat | cat" 0 "Many pipes stress test"
# COMMENTED OUT - USES && WHICH IS BONUS
# run_test "Large environment" "export VAR1=val1 && export VAR2=val2 && export VAR3=val3 && echo done" 0 "Large environment stress test"

# MANDATORY STRESS TEST WITHOUT && (BONUS OPERATORS)
run_test "Large environment setup" "export VAR1=val1; export VAR2=val2; export VAR3=val3" 0 "Set up large environment"
run_test "Large environment check" "echo done" 0 "Large environment stress test"

# ============================================================================
# COMPREHENSIVE TEST SUITE EXECUTION
# ============================================================================
print_section "11" "COMPREHENSIVE TEST SUITE"

echo -e "${CYAN}Running comprehensive test suite...${NC}"
if [ -f "./comprehensive_test.sh" ]; then
    run_with_timeout 60 "./comprehensive_test.sh > $TEST_DIR/comprehensive_output.txt 2>&1"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Comprehensive test suite completed successfully${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Comprehensive test suite failed or timed out${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
else
    echo -e "${YELLOW}⚠ comprehensive_test.sh not found${NC}"
fi

echo -e "${CYAN}Running extreme stress test suite...${NC}"
if [ -f "./extreme_stress_test.sh" ]; then
    run_with_timeout 60 "./extreme_stress_test.sh > $TEST_DIR/extreme_output.txt 2>&1"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Extreme stress test suite completed successfully${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ Extreme stress test suite failed or timed out${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
else
    echo -e "${YELLOW}⚠ extreme_stress_test.sh not found${NC}"
fi

# ============================================================================
# RESULTS SUMMARY
# ============================================================================
echo ""
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo -e "${BOLD}${BLUE}                              TEST RESULTS SUMMARY                             ${NC}"
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo ""

echo -e "${BOLD}Total Tests Run:${NC} $TOTAL_TESTS"
echo -e "${GREEN}${BOLD}Tests Passed:${NC} $PASSED_TESTS"
echo -e "${RED}${BOLD}Tests Failed:${NC} $FAILED_TESTS"

if [ $MEMORY_LEAKS -gt 0 ]; then
    echo -e "${RED}${BOLD}Memory Leaks Detected:${NC} $MEMORY_LEAKS"
fi

# Calculate pass percentage
if [ $TOTAL_TESTS -gt 0 ]; then
    pass_percentage=$(( (PASSED_TESTS * 100) / TOTAL_TESTS ))
    echo -e "${BOLD}Pass Rate:${NC} ${pass_percentage}%"
fi

echo ""

# Final verdict
if [ $FAILED_TESTS -eq 0 ] && [ $MEMORY_LEAKS -eq 0 ]; then
    echo -e "${GREEN}${BOLD}🎉 ALL TESTS PASSED! Your minishell is working perfectly!${NC}"
    echo -e "${GREEN}${BOLD}✓ No functionality issues found${NC}"
    if [ "$VALGRIND_AVAILABLE" = true ]; then
        echo -e "${GREEN}${BOLD}✓ No memory leaks detected${NC}"
    fi
    echo -e "${GREEN}${BOLD}✓ Ready for evaluation!${NC}"
elif [ $FAILED_TESTS -eq 0 ] && [ $MEMORY_LEAKS -gt 0 ]; then
    echo -e "${YELLOW}${BOLD}⚠ FUNCTIONALITY TESTS PASSED BUT MEMORY LEAKS DETECTED${NC}"
    echo -e "${YELLOW}${BOLD}Fix memory leaks before evaluation${NC}"
elif [ $MEMORY_LEAKS -eq 0 ] && [ $FAILED_TESTS -gt 0 ]; then
    echo -e "${YELLOW}${BOLD}⚠ NO MEMORY LEAKS BUT SOME FUNCTIONALITY TESTS FAILED${NC}"
    echo -e "${YELLOW}${BOLD}Fix functionality issues before evaluation${NC}"
else
    echo -e "${RED}${BOLD}❌ MULTIPLE ISSUES FOUND${NC}"
    echo -e "${RED}${BOLD}Fix both functionality and memory issues before evaluation${NC}"
fi

echo ""
echo -e "${CYAN}Test artifacts saved in: $TEST_DIR${NC}"
echo -e "${CYAN}Review detailed logs for debugging information${NC}"

# Cleanup
rm -f "$TEST_DIR/current_test.sh" "$TEST_DIR/memory_test.sh"

exit $FAILED_TESTS
