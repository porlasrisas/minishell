#!/bin/bash

echo "=== TEST HEREDOC DIRECTO ==="
echo "Creando test script para heredoc..."

# Crear un script que simule input interactivo para heredoc
cat << 'SCRIPT' > test_input.txt
cat << EOF
linea 1
linea 2
EOF
SCRIPT

echo "Contenido del script:"
cat test_input.txt

echo ""
echo "Ejecutando en minishell:"
cat test_input.txt | ./minishell

echo "=== FIN TEST ==="
