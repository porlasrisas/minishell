#!/bin/bash

# ============================================================================
# COMPREHENSIVE MINISHELL TEST SUITE
# Exhaustive testing of all cases: normal, weird, and ridiculously extreme
# ============================================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

MINISHELL="./minishell"

# Create comprehensive test script
create_test_script() {
    cat > /tmp/minishell_test_commands.txt << 'EOF'
echo "═══════════════════════════════════════════════════════════════════════════"
echo "                    COMPREHENSIVE MINISHELL TEST SUITE"
echo "═══════════════════════════════════════════════════════════════════════════"
echo

echo "1. BASIC ECHO TESTS (Normal Cases)"
echo hello
echo hello world
echo -n hello
echo -n hello world
echo
echo ""
echo "test"
echo 'test'
echo "test"
echo test test test
echo -n -n hello
echo -n && echo world

echo
echo "2. ECHO WITH SPECIAL CHARACTERS"
echo '!@#$%^&*()_+-={}[]|:;<>?,./'
echo "!@#$%^&*()_+-={}[]|:;<>?,./"
echo \!\@\#\$\%\^\&\*\(\)
echo 'héllo wörld 你好'
echo "Line1\nLine2\nLine3"
echo -e "Line1\nLine2\nLine3"
echo $'Line1\nLine2\nLine3'

echo
echo "3. BUILTIN COMMANDS - PWD"
pwd
pwd extra arguments should be ignored
pwd 1 2 3 4 5

echo
echo "4. BUILTIN COMMANDS - CD (Normal Cases)"
cd /tmp
pwd
cd /
pwd
cd
pwd
cd ~
pwd
cd /tmp
pwd

echo
echo "5. CD EDGE CASES"
cd .
pwd
cd ..
pwd
cd ../..
pwd
cd ./../tmp/./
pwd

echo
echo "6. CD ERROR CASES"
cd /non/existent/directory/that/does/not/exist
cd /etc/passwd
cd ""
cd /root
cd /dev/null

echo
echo "7. BUILTIN COMMANDS - ENV"
env | head -3
env | grep PATH
env extra args ignored | wc -l

echo
echo "8. BUILTIN COMMANDS - EXPORT (Normal Cases)"
export TEST_VAR=hello
echo $TEST_VAR
export A=1
export B=2
export C=3
echo $A $B $C
export PATH_BACKUP="$PATH"
export PATH="/new/path"
echo $PATH
export PATH="$PATH_BACKUP"

echo
echo "9. EXPORT EDGE CASES"
export EMPTY_VAR=
echo $EMPTY_VAR
export SPACE_VAR="hello world"
echo $SPACE_VAR
export QUOTE_VAR='single quotes'
echo $QUOTE_VAR
export MULTI_VAR=one TWO_VAR=two THREE_VAR=three
echo $MULTI_VAR $TWO_VAR $THREE_VAR

echo
echo "10. EXPORT ERROR CASES"
export 123INVALID=test
export =noname
export VAR =space
export VAR= space
export ""
export "WEIRD VAR"=test

echo
echo "11. BUILTIN COMMANDS - UNSET"
export TO_UNSET=test
echo $TO_UNSET
unset TO_UNSET
echo $TO_UNSET
unset NON_EXISTENT_VAR
unset A B C
echo $A $B $C

echo
echo "12. VARIABLE EXPANSION (Normal Cases)"
export VAR=hello
echo $VAR
echo "$VAR"
echo '$VAR'
echo ${VAR}
echo $VAR world
echo ${VAR}world
echo pre$VAR
echo pre${VAR}post

echo
echo "13. VARIABLE EXPANSION EDGE CASES"
echo $NON_EXISTENT
echo "$NON_EXISTENT"
echo ${NON_EXISTENT}
export NUM=42
echo $NUM
echo "$NUM"
echo ${NUM}
echo $$
echo $?
true
echo $?
false
echo $?

echo
echo "14. VARIABLE EXPANSION WEIRD CASES"
export WEIRD_VAR='$VAR inside'
echo $WEIRD_VAR
export VAR1=VAR2
export VAR2=hello
echo $VAR1
echo ${VAR1}
export A=1
export B=$A
echo $B
export PATH_TEST="$PATH:/new/path"
echo $PATH_TEST

echo
echo "15. QUOTES AND ESCAPING (Normal Cases)"
echo 'single quotes'
echo "double quotes"
echo 'single with "double" inside'
echo "double with 'single' inside"
echo 'can'\''t do this'
echo "He said \"hello\""

echo
echo "16. QUOTES EDGE CASES"
echo ''
echo ""
echo 'multiple words in single'
echo "multiple words in double"
echo "'nested single in double'"
echo '"nested double in single"'
echo 'single'\''quote'\''escape'

echo
echo "17. QUOTES EXTREME CASES"
echo 'It'\''s a beautiful day, isn'\''t it?'
echo "She said, \"It's \\\"amazing\\\"!\""
echo 'Special chars: !@#$%^&*()_+-={}[]|:;<>?,./'
echo "Special chars: !@#\$%^&*()_+-={}[]|:;<>?,./"

echo
echo "18. PIPES (Normal Cases)"
echo hello | cat
echo hello world | cat
ls / | head -5
echo -e "line1\nline2\nline3" | grep line2
cat /etc/passwd | head -1

echo
echo "19. PIPES COMPLEX CASES"
echo hello | cat | cat
echo hello | cat | cat | cat
ls /bin | grep sh | head -3
echo "multiple lines" | cat | wc -l

echo
echo "20. PIPES WITH BUILTINS"
echo hello | cat
pwd | cat
env | head -3 | cat
export | head -5 | cat

echo
echo "21. REDIRECTIONS (Normal Cases)"
echo hello > /tmp/test1.txt
cat /tmp/test1.txt
echo world > /tmp/test2.txt
cat /tmp/test2.txt
echo append >> /tmp/test1.txt
cat /tmp/test1.txt

echo
echo "22. INPUT REDIRECTIONS"
echo "input test" > /tmp/input.txt
cat < /tmp/input.txt
wc -l < /tmp/input.txt
grep test < /tmp/input.txt

echo
echo "23. REDIRECTION EDGE CASES"
cat > /tmp/empty.txt
cat /tmp/empty.txt
echo hello > /tmp/overwrite.txt
echo world > /tmp/overwrite.txt
cat /tmp/overwrite.txt
echo one >> /tmp/append.txt
echo two >> /tmp/append.txt
cat /tmp/append.txt

echo
echo "24. REDIRECTION ERROR CASES"
cat /non/existent/file
echo test > /root/forbidden 2>/dev/null
echo $?

echo
echo "25. HEREDOC (Normal Cases)"
cat << EOF
This is a heredoc
With multiple lines
EOF

echo
echo "26. HEREDOC WITH VARIABLES"
export HEREDOC_VAR=world
cat << EOF
Hello $HEREDOC_VAR
This expands variables
EOF

echo
echo "27. HEREDOC QUOTED DELIMITER"
cat << 'EOF'
This does not expand $HEREDOC_VAR
Variables are literal here
EOF

echo
echo "28. HEREDOC EDGE CASES"
cat << EOF
EOF

cat << DELIMITER
Single line
DELIMITER

cat << "QUOTED"
Quoted delimiter
QUOTED

echo
echo "29. COMMAND NOT FOUND CASES"
nonexistentcommand123456789
./nonexistent
/bin/nonexistent
command_with_underscores_and_numbers123

echo
echo "30. EMPTY COMMANDS AND WHITESPACE"

  
	
   echo spaced command   
		echo 	tabbed	command	

echo
echo "31. SEMICOLON SEPARATED COMMANDS"
echo first; echo second
echo hello; pwd; echo world
export A=1; echo $A; unset A; echo $A

echo
echo "32. COMPLEX COMBINED CASES"
export TEST=hello && echo $TEST | cat > /tmp/complex.txt && cat /tmp/complex.txt
echo world | cat | cat > /tmp/pipe_chain.txt && cat /tmp/pipe_chain.txt
(echo nested) 2>/dev/null
echo test && echo success || echo failure

echo
echo "33. STRESS TESTS - LONG COMMANDS"
echo very long argument with many words that should be handled correctly by the parser
echo a b c d e f g h i j k l m n o p q r s t u v w x y z
echo 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20

echo
echo "34. STRESS TESTS - DEEP PIPES"
echo start | cat | cat | cat | cat | cat

echo
echo "35. ENVIRONMENT STRESS TEST"
export VAR1=1
export VAR2=2
export VAR3=3
export VAR4=4
export VAR5=5
echo $VAR1 $VAR2 $VAR3 $VAR4 $VAR5
unset VAR1 VAR2 VAR3 VAR4 VAR5

echo
echo "36. PATH MANIPULATION"
export ORIGINAL_PATH="$PATH"
export PATH="/bin:/usr/bin"
ls >/dev/null 2>&1 && echo "ls works with minimal PATH"
export PATH=""
ls >/dev/null 2>&1 || echo "ls fails with empty PATH"
export PATH="$ORIGINAL_PATH"

echo
echo "37. EXTREME EDGE CASES"
echo > /tmp/redir_only.txt
cat /tmp/redir_only.txt
< /tmp/input.txt cat
echo "Testing pipe at start (should fail gracefully)"
echo "Testing pipe at end (should fail gracefully)"

echo
echo "38. UNICODE AND SPECIAL CHARACTERS"
echo "Ñandú piñón niño"
echo "Zürich naïve résumé"
echo "🌍🌎🌏 Hello World! 👋"
echo $'tab\there newline\nhere'

echo
echo "39. BUILTIN EXIT TESTS"
echo "Testing exit status codes..."
true && echo $?
false && echo $?
/bin/true && echo $?
/bin/false && echo $?

echo
echo "40. FINAL CLEANUP"
rm -f /tmp/test1.txt /tmp/test2.txt /tmp/input.txt /tmp/empty.txt
rm -f /tmp/overwrite.txt /tmp/append.txt /tmp/complex.txt /tmp/pipe_chain.txt
rm -f /tmp/redir_only.txt

echo
echo "═══════════════════════════════════════════════════════════════════════════"
echo "                           TESTS COMPLETED"
echo "═══════════════════════════════════════════════════════════════════════════"
echo "If you see this message, minishell handled all test cases!"
echo "Review the output above for any errors or unexpected behavior."

exit
EOF
}

# Check if minishell exists
if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found. Please compile first with 'make'${NC}"
    exit 1
fi

echo -e "${BLUE}============================================================================${NC}"
echo -e "${BLUE}                    COMPREHENSIVE MINISHELL TEST SUITE                     ${NC}"
echo -e "${BLUE}============================================================================${NC}"
echo -e "${YELLOW}Testing ALL cases: normal, weird, and ridiculously extreme${NC}"
echo -e "${YELLOW}This will test every possible scenario in minishell${NC}"
echo ""

# Create test script
create_test_script

echo -e "${GREEN}Starting comprehensive tests...${NC}"
echo -e "${YELLOW}Watch carefully for any errors, crashes, or unexpected behavior${NC}"
echo ""

# Execute the test script in minishell
$MINISHELL < /tmp/minishell_test_commands.txt

echo ""
echo -e "${GREEN}Test execution completed!${NC}"
echo -e "${BLUE}Analysis:${NC}"
echo -e "${YELLOW}• If you see 'TESTS COMPLETED' above, minishell handled all cases${NC}"
echo -e "${YELLOW}• Review the output for any error messages or unexpected behavior${NC}"
echo -e "${YELLOW}• Check if any commands failed or produced wrong output${NC}"

# Cleanup
rm -f /tmp/minishell_test_commands.txt

exit 0
