#!/bin/bash

# ============================================================================
# MEMORY LEAK DETECTION AND PERFORMANCE TEST
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"

echo -e "${BLUE}============================================================================${NC}"
echo -e "${BLUE}                    MEMORY LEAK AND PERFORMANCE TEST                       ${NC}"
echo -e "${BLUE}============================================================================${NC}"

# Check if valgrind is available
if command -v valgrind &> /dev/null; then
    echo -e "${GREEN}✓ Valgrind detected - will run memory leak tests${NC}"
    USE_VALGRIND=1
else
    echo -e "${YELLOW}⚠ Valgrind not found - skipping memory leak tests${NC}"
    USE_VALGRIND=0
fi

# Function to test with valgrind
test_memory() {
    local test_name="$1"
    local command="$2"
    
    echo -e "${YELLOW}Testing memory: $test_name${NC}"
    
    if [ $USE_VALGRIND -eq 1 ]; then
        # Run with valgrind
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
            --error-exitcode=1 --log-file=/tmp/valgrind_output.log \
            bash -c "echo '$command' | $MINISHELL" &>/dev/null
        
        local exit_code=$?
        
        if [ $exit_code -eq 0 ]; then
            echo -e "${GREEN}✓ No memory leaks detected${NC}"
        else
            echo -e "${RED}✗ Memory issues detected${NC}"
            echo "Valgrind output:"
            cat /tmp/valgrind_output.log | grep -A 5 -B 5 "ERROR\|LEAK"
        fi
    else
        # Just run the command
        echo "$command" | $MINISHELL &>/dev/null
        echo -e "${YELLOW}→ Executed (no memory check)${NC}"
    fi
    echo "---"
}

# Function to test performance
test_performance() {
    local test_name="$1"
    local command="$2"
    local iterations=${3:-100}
    
    echo -e "${YELLOW}Testing performance: $test_name${NC}"
    
    local start_time=$(date +%s%N)
    
    for ((i=1; i<=iterations; i++)); do
        echo "$command" | $MINISHELL &>/dev/null
        if [ $? -ne 0 ]; then
            echo -e "${RED}✗ Command failed at iteration $i${NC}"
            return 1
        fi
    done
    
    local end_time=$(date +%s%N)
    local duration=$(( (end_time - start_time) / 1000000 )) # Convert to milliseconds
    local avg_time=$(( duration / iterations ))
    
    echo -e "${GREEN}✓ Completed $iterations iterations in ${duration}ms (avg: ${avg_time}ms)${NC}"
    
    if [ $avg_time -lt 10 ]; then
        echo -e "${GREEN}  → Excellent performance${NC}"
    elif [ $avg_time -lt 50 ]; then
        echo -e "${YELLOW}  → Good performance${NC}"
    else
        echo -e "${RED}  → Slow performance${NC}"
    fi
    echo "---"
}

# ============================================================================
# BASIC MEMORY LEAK TESTS
# ============================================================================
echo -e "${BLUE}[1] BASIC MEMORY LEAK TESTS${NC}"

test_memory "Simple echo" "echo hello"
test_memory "Variable expansion" "export VAR=test; echo \$VAR"
test_memory "Command with pipes" "echo hello | cat"
test_memory "File redirection" "echo hello > /tmp/test.txt"
test_memory "Heredoc" "cat << EOF"$'\n'"hello"$'\n'"EOF"
test_memory "Exit command" "exit"

# ============================================================================
# COMPLEX MEMORY TESTS
# ============================================================================
echo -e "${BLUE}[2] COMPLEX MEMORY TESTS${NC}"

test_memory "Multiple commands" "echo hello; echo world; echo test"
test_memory "Multiple pipes" "echo hello | cat | cat | cat"
test_memory "Multiple redirections" "echo hello > /tmp/test1 && cat /tmp/test1 > /tmp/test2"
test_memory "Long command line" "echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10"
test_memory "Many variables" "export A=1 B=2 C=3 D=4 E=5; echo \$A \$B \$C \$D \$E"

# ============================================================================
# ERROR HANDLING MEMORY TESTS
# ============================================================================
echo -e "${BLUE}[3] ERROR HANDLING MEMORY TESTS${NC}"

test_memory "Command not found" "nonexistentcommand123456789"
test_memory "Invalid redirection" "echo hello >"
test_memory "Invalid pipe" "echo hello |"
test_memory "Syntax error" "echo hello &&"
test_memory "Permission denied" "cat /root/.secret_file"

# ============================================================================
# STRESS MEMORY TESTS
# ============================================================================
echo -e "${BLUE}[4] STRESS MEMORY TESTS${NC}"

# Large argument
large_arg=$(printf 'a%.0s' {1..1000})
test_memory "Large argument" "echo $large_arg"

# Many small arguments
many_args="echo"
for i in {1..100}; do
    many_args="$many_args arg$i"
done
test_memory "Many arguments" "$many_args"

# Deep pipe chain
pipe_chain="echo start"
for i in {1..20}; do
    pipe_chain="$pipe_chain | cat"
done
test_memory "Deep pipe chain" "$pipe_chain"

# ============================================================================
# PERFORMANCE TESTS
# ============================================================================
echo -e "${BLUE}[5] PERFORMANCE TESTS${NC}"

test_performance "Simple echo" "echo hello" 1000
test_performance "Variable expansion" "export VAR=test; echo \$VAR" 500
test_performance "Pipe command" "echo hello | cat" 200
test_performance "File operations" "echo hello > /tmp/perf_test.txt && cat /tmp/perf_test.txt" 100
test_performance "Builtin commands" "pwd" 500

# ============================================================================
# LONG-RUNNING TESTS
# ============================================================================
echo -e "${BLUE}[6] LONG-RUNNING TESTS${NC}"

echo -e "${YELLOW}Testing long-running session...${NC}"
{
    echo "export COUNTER=0"
    for i in {1..100}; do
        echo "export COUNTER=\$((COUNTER + 1))"
        echo "echo \"Iteration: \$COUNTER\""
    done
    echo "exit"
} > /tmp/long_session.txt

if [ $USE_VALGRIND -eq 1 ]; then
    valgrind --leak-check=full --show-leak-kinds=all \
        --error-exitcode=1 --log-file=/tmp/valgrind_long.log \
        $MINISHELL < /tmp/long_session.txt &>/dev/null
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Long session completed without memory leaks${NC}"
    else
        echo -e "${RED}✗ Memory leaks detected in long session${NC}"
        tail -20 /tmp/valgrind_long.log
    fi
else
    $MINISHELL < /tmp/long_session.txt &>/dev/null
    echo -e "${YELLOW}→ Long session executed (no memory check)${NC}"
fi

# ============================================================================
# CONCURRENT EXECUTION TESTS
# ============================================================================
echo -e "${BLUE}[7] CONCURRENT EXECUTION TESTS${NC}"

echo -e "${YELLOW}Testing concurrent minishell instances...${NC}"

# Start multiple minishell instances
for i in {1..5}; do
    {
        echo "export INSTANCE=$i"
        echo "echo \"Instance \$INSTANCE running\""
        echo "sleep 1"
        echo "exit"
    } | $MINISHELL &
done

wait
echo -e "${GREEN}✓ All concurrent instances completed${NC}"

# ============================================================================
# RESOURCE USAGE MONITORING
# ============================================================================
echo -e "${BLUE}[8] RESOURCE USAGE MONITORING${NC}"

if command -v ps &> /dev/null; then
    echo -e "${YELLOW}Monitoring resource usage during execution...${NC}"
    
    # Start a background process to monitor resources
    {
        while true; do
            ps aux | grep minishell | grep -v grep >> /tmp/resource_usage.log
            sleep 0.1
        done
    } &
    monitor_pid=$!
    
    # Run some commands
    {
        echo "echo 'Starting resource test'"
        for i in {1..50}; do
            echo "echo 'Iteration $i'"
            echo "export VAR$i=value$i"
        done
        echo "env | wc -l"
        echo "exit"
    } | $MINISHELL &>/dev/null
    
    # Stop monitoring
    kill $monitor_pid 2>/dev/null
    
    if [ -f /tmp/resource_usage.log ]; then
        max_memory=$(awk '{print $6}' /tmp/resource_usage.log | sort -n | tail -1)
        echo -e "${GREEN}✓ Maximum memory usage: ${max_memory}KB${NC}"
        
        if [ "$max_memory" -lt 10000 ]; then
            echo -e "${GREEN}  → Excellent memory efficiency${NC}"
        elif [ "$max_memory" -lt 50000 ]; then
            echo -e "${YELLOW}  → Good memory efficiency${NC}"
        else
            echo -e "${RED}  → High memory usage${NC}"
        fi
    fi
fi

# ============================================================================
# CLEANUP AND FINAL REPORT
# ============================================================================
echo -e "${BLUE}[CLEANUP]${NC}"

rm -f /tmp/test.txt /tmp/test1.txt /tmp/test2.txt /tmp/perf_test.txt
rm -f /tmp/long_session.txt /tmp/resource_usage.log
rm -f /tmp/valgrind_output.log /tmp/valgrind_long.log

echo ""
echo -e "${BLUE}============================================================================${NC}"
echo -e "${BLUE}                        MEMORY AND PERFORMANCE REPORT                      ${NC}"
echo -e "${BLUE}============================================================================${NC}"
echo ""

if [ $USE_VALGRIND -eq 1 ]; then
    echo -e "${GREEN}✓ Memory leak testing completed with Valgrind${NC}"
    echo -e "If no memory errors were reported above, your minishell is leak-free!"
else
    echo -e "${YELLOW}⚠ Memory leak testing skipped (Valgrind not available)${NC}"
    echo -e "To run full memory tests, install Valgrind: brew install valgrind"
fi

echo ""
echo -e "${GREEN}Performance testing completed!${NC}"
echo -e "Review the performance results above to ensure your minishell is efficient."
echo ""
echo -e "${BLUE}Recommendations:${NC}"
echo -e "• Run this test regularly during development"
echo -e "• Fix any memory leaks immediately"
echo -e "• Monitor performance with large inputs"
echo -e "• Test on different systems and architectures"
echo ""
