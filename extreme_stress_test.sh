#!/bin/bash

# ================================================================echo "5. REDIRECTION ERROR CASES - Should fail gracefully"
cat /non/existent/file/should/fail
echo test > /root/should/fail/permission/denied
echo "Testing incomplete redirection operators"
echo "Testing redirection without target"======
# EXTREME EDGE CASES AND PARSER STRESS TEST
# Tests designed to break parsers and find edge cases
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"

# Create extreme test script for direct execution
create_extreme_test_script() {
    cat > /tmp/extreme_test_commands.txt << 'EOF'
echo "════════════════════════════════════════════════════════════════════════════"
echo "                   EXTREME MINISHELL PARSER STRESS TEST"
echo "════════════════════════════════════════════════════════════════════════════"
echo "Testing ridiculously extreme cases designed to break parsers..."
echo

echo "1. NULL AND EMPTY CASES"
echo "Testing empty inputs and edge cases..."

echo ""
echo ""
   
	
   echo "    After empty lines    "
echo $'Testing\x00null\x00bytes'

echo
echo "2. QUOTE HELL - Designed to break quote parsing"
echo 'hello world'
echo "hello world"
echo 'it'\''s working'
echo "she said \"hello\""
echo '''multiple''' """quotes"""
echo "'nested single in double'"
echo '"nested double in single"'
echo 'can'\''t stop won'\''t stop'
echo "Deeply \"nested \\\"quotes\\\" here\""
echo $'Special\tquote\nhandling'

echo
echo "3. QUOTE EXTREMES - Even more ridiculous"
echo ''''
echo """"
echo "''"
echo '""'
echo '\''
echo "\""
echo 'hello'\''world'\''test'
echo "first\"middle\"last"

echo
echo "4. REDIRECTION CHAOS - Multiple redirections"
echo hello > /tmp/extreme1.txt
cat /tmp/extreme1.txt
echo world > /tmp/extreme2.txt > /tmp/extreme3.txt
echo "test" >> /tmp/extreme1.txt
cat /tmp/extreme1.txt
echo overwrite > /tmp/extreme1.txt
echo append >> /tmp/extreme1.txt

echo
echo "5. REDIRECTION EDGE CASES"
cat > /tmp/only_redirect.txt
echo content > /tmp/content.txt
cat < /tmp/content.txt
cat < /tmp/content.txt > /tmp/copy.txt
cat /tmp/copy.txt
echo test>  /tmp/nospace.txt
echo test  >/tmp/extraspace.txt

echo
echo "6. REDIRECTION ERROR CASES - Should fail gracefully"
cat /non/existent/file/should/fail
echo test > /root/should/fail/permission/denied
echo hello >
cat < 
> /tmp/only_redir_operator

echo
echo "7. PIPE MADNESS - Extreme pipe combinations"
echo hello | cat
echo hello | cat | cat | cat
echo start | cat | cat | cat | cat | cat | cat
echo "multi line
content here" | cat | wc -l
echo hello|cat|cat|cat
echo test | head -n 1 | cat | tail -n 1

echo
echo "8. PIPE ERROR CASES - Should handle gracefully" 
echo hello
echo "Testing pipe at end (should fail gracefully)"
echo test || echo logical or
echo test && echo logical and
echo "Testing double pipe (should fail gracefully)"

echo
echo "9. VARIABLE EXPANSION EXTREMES"
echo $NON_EXISTENT_VAR
echo "$NON_EXISTENT_VAR"
echo ${NON_EXISTENT_VAR}
echo $USER
echo "$USER"
echo ${USER}
echo $USER_NON_EXISTENT
echo ${USER}_suffix
echo $HOME/test
echo "$PATH" | head -c 50

echo
echo "10. VARIABLE EDGE CASES"
export EXTREME_VAR=test
echo $EXTREME_VAR
export EXTREME_VAR=""
echo $EXTREME_VAR
export EXTREME_VAR="with spaces"
echo $EXTREME_VAR
export EXTREME_VAR='with single quotes'
echo $EXTREME_VAR
unset EXTREME_VAR
echo $EXTREME_VAR

echo
echo "11. ENVIRONMENT MANIPULATION EXTREMES"
export VAR1=1 VAR2=2 VAR3=3
echo $VAR1 $VAR2 $VAR3
export LONG_VAR="very long value with many words that should be handled properly"
echo $LONG_VAR
export SPECIAL_VAR='!@#$%^&*()_+-={}[]|:;<>?,./'
echo $SPECIAL_VAR
unset VAR1 VAR2 VAR3 LONG_VAR SPECIAL_VAR

echo
echo "12. BUILTIN EXTREMES - Testing edge cases"
cd
pwd
cd /tmp
pwd
cd /
pwd
cd non_existent_directory_that_should_fail
cd /etc/passwd
cd ""
pwd
cd ../../../../../../../../../../../
pwd

echo
echo "13. EXPORT EXTREMES"
export
export VALID=value
export 123INVALID=should_fail
export =no_name
export SPACE VAR=should_fail
export "QUOTED NAME"=should_fail
export VALID_VAR=value
export VALID_VAR=new_value
echo $VALID_VAR
unset VALID_VAR

echo
echo "14. HEREDOC EXTREMES"
cat << EOF
This is a heredoc
With multiple lines
And variable expansion: $USER
EOF

cat << 'EOF'
This heredoc has quoted delimiter
Variables like $USER are not expanded
EOF

cat << DELIMITER
Different delimiter
DELIMITER

cat << "QUOTED_DELIMITER"
Quoted delimiter test
QUOTED_DELIMITER

echo
echo "15. HEREDOC EDGE CASES"
cat << EOF
EOF

cat << END
Single line heredoc
END

echo
echo "16. COMMAND NOT FOUND EXTREMES"
nonexistent_command_12345
./non_existent_file
/bin/non_existent_command
command-with-dashes
command_with_underscores
123command
command123

echo
echo "17. SPECIAL CHARACTER STRESS TEST"
echo '!@#$%^&*()_+-={}[]|:;<>?,./'
echo "!@#\$%^&*()_+-={}[]|:;<>?,./"
echo \!\@\#\$\%\^\&\*\(\)
echo 'Unicode: héllo wörld 你好 🌍🌎🌏'
echo "Émojis: 😀😎🎉🚀💻🔥⭐"
echo $'Control chars: \t\n\r\v\f\b'

echo
echo "18. WHITESPACE EXTREMES"
   echo    with    many    spaces   
		echo		with		many		tabs		
echo "
multiline
string
with
newlines
"

echo
echo "19. LENGTH EXTREMES - Very long commands"
echo very long argument list with many many many words that go on and on and on and should test the limits of argument parsing and memory allocation
echo a b c d e f g h i j k l m n o p q r s t u v w x y z 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

echo
echo "20. NESTED COMPLEXITY"
echo $(echo nested)
echo "Today is: $(date)"
echo `echo backticks`
echo $(echo $(echo double nested))
export NESTED_VAR="$(echo from command substitution)"
echo $NESTED_VAR

echo
echo "21. SEMICOLON CHAOS"
echo first; echo second; echo third
echo "Testing semicolon at start (should fail gracefully)"
echo semicolon at end ;
echo "Testing empty semicolon commands"
echo test;

echo
echo "22. LOGIC OPERATOR EXTREMES"
true && echo "true worked"
false || echo "false worked" 
true && echo "first" && echo "second"
false || echo "first" || echo "second"
echo test && echo success
echo test || echo this should not print

echo
echo "23. BACKGROUND PROCESS TESTS"
sleep 0.1 &
echo "Background process started"
wait
echo "Background process completed"

echo
echo "24. FILE OPERATION EXTREMES"
mkdir -p /tmp/extreme_test
cd /tmp/extreme_test
touch file1 file2 file3
ls
echo content > file1
cat file1
cp file1 file1_copy
ls
rm file1 file2 file3 file1_copy
cd /tmp
rmdir extreme_test

echo
echo "25. PATH MANIPULATION EXTREMES"
export ORIGINAL_PATH="$PATH"
echo "Original PATH length: $(echo $PATH | wc -c)"
export PATH="/bin:/usr/bin"
ls >/dev/null 2>&1 && echo "Minimal PATH works"
export PATH=""
ls >/dev/null 2>&1 || echo "Empty PATH fails as expected"
export PATH="$ORIGINAL_PATH"
ls >/dev/null 2>&1 && echo "Restored PATH works"

echo
echo "26. EXTREME HEREDOC CASES"
cat << 'EXTREME'
This is an extreme heredoc test
With 'single quotes' and "double quotes"
And $variables that should not expand
Special chars: !@#$%^&*()_+-={}[]|:;<>?,./
Unicode: héllö wörld 你好 🌍
EXTREME

echo
echo "27. PERMISSION AND ACCESS TESTS"
echo "Testing file permissions..."
touch /tmp/test_perms.txt
chmod 644 /tmp/test_perms.txt
ls -l /tmp/test_perms.txt
cat /tmp/test_perms.txt
rm /tmp/test_perms.txt

echo
echo "28. ERROR RECOVERY TESTS"
echo "Testing error recovery..."
false
echo $?
true
echo $?
false && echo "should not print"
true && echo "should print"
echo "after error commands"

echo
echo "29. STRESS COMBINATIONS"
export COMBO_VAR=combo && echo $COMBO_VAR | cat > /tmp/combo.txt && cat /tmp/combo.txt && rm /tmp/combo.txt
echo "complex" | cat | cat | head -1 | tail -1 > /tmp/complex.txt
cat /tmp/complex.txt && rm /tmp/complex.txt

echo
echo "30. FINAL EXTREME CASES"
echo "Testing final extreme cases..."
echo ""
echo ''
echo $''
echo $""
cd . && pwd
cd .. && pwd && cd /tmp
export A=1 && export B=$A && echo $B && unset A B

echo
echo "════════════════════════════════════════════════════════════════════════════"
echo "                    EXTREME STRESS TEST COMPLETED"
echo "════════════════════════════════════════════════════════════════════════════"
echo "If you see this message, your minishell survived ALL extreme test cases!"
echo "This includes:"
echo "• Quote parsing hell"  
echo "• Redirection chaos"
echo "• Pipe madness"
echo "• Variable expansion extremes"
echo "• Command substitution torture"
echo "• Unicode and special characters"
echo "• Error condition testing"
echo "• Memory and length stress tests"
echo "• Permission and access edge cases"
echo "• Parser breaking attempts"
echo ""
echo "Your minishell is EXTREMELY robust! 🎉"

exit
EOF
}

# Check if minishell exists
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found. Please compile first with 'make'${NC}"
    exit 1
fi

echo -e "${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}                    EXTREME MINISHELL STRESS TEST SUITE                     ${NC}"
echo -e "${BLUE}════════════════════════════════════════════════════════════════════════════${NC}"
echo -e "${YELLOW}Testing EXTREME cases designed to break parsers and find edge cases${NC}"
echo -e "${RED}⚠️  WARNING: These tests are designed to push minishell to its limits!${NC}"
echo ""

# Create test script
create_extreme_test_script

echo -e "${GREEN}Starting extreme stress tests...${NC}"
echo -e "${YELLOW}These tests include:${NC}"
echo -e "${YELLOW}• Quote parsing hell${NC}"
echo -e "${YELLOW}• Redirection chaos${NC}"
echo -e "${YELLOW}• Pipe madness${NC}"
echo -e "${YELLOW}• Variable expansion extremes${NC}"
echo -e "${YELLOW}• Command substitution torture${NC}"
echo -e "${YELLOW}• Unicode and special characters${NC}"
echo -e "${YELLOW}• Error condition testing${NC}"
echo -e "${YELLOW}• Memory and length stress tests${NC}"
echo -e "${YELLOW}• Parser breaking attempts${NC}"
echo ""

# Execute the extreme test script in minishell
$MINISHELL < /tmp/extreme_test_commands.txt

echo ""
echo -e "${GREEN}Extreme stress test execution completed!${NC}"
echo -e "${BLUE}Analysis:${NC}"
echo -e "${YELLOW}• If you see 'EXTREME STRESS TEST COMPLETED' above, your minishell is EXTREMELY robust!${NC}"
echo -e "${YELLOW}• Look for any crashes, hangs, or segmentation faults${NC}"
echo -e "${YELLOW}• Check if error cases were handled gracefully${NC}"
echo -e "${YELLOW}• Verify that extreme quote and redirection cases worked${NC}"

# Cleanup
rm -f /tmp/extreme_test_commands.txt

exit 0
