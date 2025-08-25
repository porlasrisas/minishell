#!/bin/bash

# ============================================================================
# RUN ALL MINISHELL TESTS
# Convenient script to run all test suites including memory leaks
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

echo -e "${BLUE}${BOLD}════════════════════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}${BOLD}                         MINISHELL COMPLETE TEST RUNNER                     ${NC}"
echo -e "${BLUE}${BOLD}════════════════════════════════════════════════════════════════════════════${NC}"
echo ""

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${RED}Error: ./minishell not found. Compiling...${NC}"
    make
    if [ $? -ne 0 ]; then
        echo -e "${RED}Compilation failed. Exiting.${NC}"
        exit 1
    fi
    echo -e "${GREEN}Compilation successful!${NC}"
    echo ""
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

# Function to run a test suite
run_suite() {
    local suite_name="$1"
    local script_path="$2"
    local description="$3"
    
    echo -e "${YELLOW}${BOLD}Running $suite_name...${NC}"
    echo -e "${YELLOW}$description${NC}"
    echo ""
    
    if [ -f "$script_path" ]; then
        chmod +x "$script_path"
        if run_with_timeout 120 "$script_path"; then
            echo -e "${GREEN}✓ $suite_name completed successfully${NC}"
            return 0
        else
            echo -e "${RED}✗ $suite_name failed or timed out${NC}"
            return 1
        fi
    else
        echo -e "${RED}✗ $script_path not found${NC}"
        return 1
    fi
}

# Track results
TOTAL_SUITES=0
PASSED_SUITES=0

echo -e "${CYAN}Available test suites:${NC}"
echo -e "${CYAN}1. Complete Tester - Functionality, memory leaks, edge cases${NC}"
echo -e "${CYAN}2. Comprehensive Tests - All normal to complex cases${NC}"
echo -e "${CYAN}3. Extreme Stress Tests - Parser breaking edge cases${NC}"
echo ""

read -p "Run all suites? (y/N): " answer

case $answer in
    [Yy]* )
        echo -e "${GREEN}Running all test suites...${NC}"
        echo ""
        
        # Run Complete Tester (most comprehensive)
        if run_suite "COMPLETE TESTER" "./complete_tester.sh" "Comprehensive functionality, memory leak, and edge case testing"; then
            PASSED_SUITES=$((PASSED_SUITES + 1))
        fi
        TOTAL_SUITES=$((TOTAL_SUITES + 1))
        echo ""
        
        # Run Comprehensive Test Suite
        if run_suite "COMPREHENSIVE TEST SUITE" "./comprehensive_test.sh" "40 sections covering all normal to complex functionality"; then
            PASSED_SUITES=$((PASSED_SUITES + 1))
        fi
        TOTAL_SUITES=$((TOTAL_SUITES + 1))
        echo ""
        
        # Run Extreme Stress Test Suite
        if run_suite "EXTREME STRESS TEST SUITE" "./extreme_stress_test.sh" "30 sections of parser-breaking extreme edge cases"; then
            PASSED_SUITES=$((PASSED_SUITES + 1))
        fi
        TOTAL_SUITES=$((TOTAL_SUITES + 1))
        echo ""
        ;;
    * )
        echo -e "${YELLOW}Choose which suite to run:${NC}"
        echo "1) Complete Tester (recommended - includes memory leaks)"
        echo "2) Comprehensive Tests"
        echo "3) Extreme Stress Tests"
        echo "4) Exit"
        
        read -p "Enter choice (1-4): " choice
        case $choice in
            1)
                if run_suite "COMPLETE TESTER" "./complete_tester.sh" "Comprehensive functionality, memory leak, and edge case testing"; then
                    PASSED_SUITES=$((PASSED_SUITES + 1))
                fi
                TOTAL_SUITES=$((TOTAL_SUITES + 1))
                ;;
            2)
                if run_suite "COMPREHENSIVE TEST SUITE" "./comprehensive_test.sh" "40 sections covering all normal to complex functionality"; then
                    PASSED_SUITES=$((PASSED_SUITES + 1))
                fi
                TOTAL_SUITES=$((TOTAL_SUITES + 1))
                ;;
            3)
                if run_suite "EXTREME STRESS TEST SUITE" "./extreme_stress_test.sh" "30 sections of parser-breaking extreme edge cases"; then
                    PASSED_SUITES=$((PASSED_SUITES + 1))
                fi
                TOTAL_SUITES=$((TOTAL_SUITES + 1))
                ;;
            4|*)
                echo -e "${YELLOW}Exiting...${NC}"
                exit 0
                ;;
        esac
        ;;
esac

# Final summary
echo ""
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo -e "${BOLD}${BLUE}                              FINAL SUMMARY                                  ${NC}"
echo -e "${BOLD}${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo ""

if [ $TOTAL_SUITES -gt 0 ]; then
    echo -e "${BOLD}Test Suites Run:${NC} $TOTAL_SUITES"
    echo -e "${GREEN}${BOLD}Suites Passed:${NC} $PASSED_SUITES"
    echo -e "${RED}${BOLD}Suites Failed:${NC} $((TOTAL_SUITES - PASSED_SUITES))"
    
    if [ $PASSED_SUITES -eq $TOTAL_SUITES ]; then
        echo ""
        echo -e "${GREEN}${BOLD}🎉 ALL TEST SUITES PASSED!${NC}"
        echo -e "${GREEN}${BOLD}Your minishell is ready for evaluation!${NC}"
    else
        echo ""
        echo -e "${YELLOW}${BOLD}⚠ Some test suites failed. Review the output above.${NC}"
    fi
else
    echo -e "${YELLOW}No test suites were run.${NC}"
fi

echo ""
echo -e "${CYAN}For detailed analysis, check the test artifacts in /tmp/minishell_tests/${NC}"

exit $((TOTAL_SUITES - PASSED_SUITES))
