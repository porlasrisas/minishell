#!/bin/bash

echo "🧪 PRUEBAS RÁPIDAS MINISHELL 🧪"
echo "==============================="

# Test 1: Variables sin comillas
echo "1. Variable sin comillas:"
echo 'echo $USER' | ./minishell 2>/dev/null | tail -1

# Test 2: Variables con comillas dobles
echo "2. Variable con comillas dobles:"
echo 'echo "$USER"' | ./minishell 2>/dev/null | tail -1

# Test 3: Variables inexistentes
echo "3. Variable inexistente:"
echo 'echo $NOEXISTE' | ./minishell 2>/dev/null | tail -1

# Test 4: Export con expansión
echo "4. Export con expansión:"
echo 'export TEST_HOME=$HOME' | ./minishell 2>/dev/null
echo 'echo $TEST_HOME' | ./minishell 2>/dev/null | tail -1

# Test 5: Redirección simple
echo "5. Redirección de salida:"
echo 'echo "test redirection" > test_simple.txt' | ./minishell 2>/dev/null
if [ -f test_simple.txt ]; then
    echo "✅ Archivo creado: $(cat test_simple.txt)"
    rm test_simple.txt
else
    echo "❌ Archivo no creado"
fi

# Test 6: Redirección append
echo "6. Redirección append:"
echo 'echo "linea1" > test_append.txt' | ./minishell 2>/dev/null
echo 'echo "linea2" >> test_append.txt' | ./minishell 2>/dev/null
if [ -f test_append.txt ]; then
    echo "✅ Archivo con append:"
    cat test_append.txt
    rm test_append.txt
else
    echo "❌ Append no funciona"
fi

# Test 7: Pipe
echo "7. Pipe simple:"
echo 'echo "hello world" | grep hello' | ./minishell 2>/dev/null | tail -1

# Test 8: Heredoc
echo "8. Heredoc:"
{
    echo 'cat << EOF'
    echo 'test heredoc'
    echo 'EOF'
} | ./minishell 2>/dev/null | grep "test heredoc"

echo "==============================="
echo "✅ Pruebas completadas"
