#!/bin/bash

# Colores para mejor visualización
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Contador de pruebas
total_tests=0
passed_tests=0

print_test() {
    echo -e "${BLUE}=== $1 ===${NC}"
    ((total_tests++))
}

print_success() {
    echo -e "${GREEN}✓ PASS: $1${NC}"
    ((passed_tests++))
}

print_error() {
    echo -e "${RED}✗ FAIL: $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠ WARNING: $1${NC}"
}

# Función para ejecutar comando en minishell
run_test() {
    local cmd="$1"
    local expected="$2"
    local description="$3"
    
    echo "Input: $cmd"
    echo "$cmd" | timeout 5 ./minishell 2>/dev/null | tail -n +2 | head -n 1
    echo ""
}

echo -e "${YELLOW}🧪 SCRIPT DE PRUEBAS COMPLETO PARA MINISHELL 🧪${NC}"
echo "=================================================="

# Verificar que minishell existe
if [ ! -f "./minishell" ]; then
    echo -e "${RED}ERROR: ./minishell no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

print_test "BUILTINS BÁSICOS"
echo 'pwd' | ./minishell | grep -q "/Users/"
if [ $? -eq 0 ]; then
    print_success "pwd funciona"
else
    print_error "pwd no funciona"
fi

print_test "ECHO BÁSICO"
run_test 'echo "Hola mundo"' "" "Echo con comillas dobles"
run_test 'echo Hola mundo' "" "Echo sin comillas"
run_test 'echo -n "Sin newline"' "" "Echo con flag -n"

print_test "VARIABLES DE ENTORNO - EXPANSIÓN SIN COMILLAS"
run_test 'echo $USER' "" "Variable sin comillas (debería expandir)"
run_test 'echo $HOME' "" "Variable HOME sin comillas"
run_test 'echo $PATH | head -c 20' "" "Variable PATH sin comillas"

print_test "VARIABLES DE ENTORNO - EXPANSIÓN CON COMILLAS DOBLES"
run_test 'echo "$USER"' "" "Variable con comillas dobles"
run_test 'echo "Mi usuario es: $USER"' "" "Variable en string con comillas dobles"
run_test 'echo "Directorio: $PWD"' "" "Variable PWD en string"

print_test "VARIABLES DE ENTORNO - SIN EXPANSIÓN CON COMILLAS SIMPLES"
run_test "echo '\$USER'" "" "Variable con comillas simples (no debe expandir)"
run_test "echo 'Mi usuario: \$USER'" "" "Variable en string con comillas simples"

print_test "VARIABLES INEXISTENTES"
run_test 'echo $VARIABLE_INEXISTENTE' "" "Variable inexistente sin comillas"
run_test 'echo "$VARIABLE_INEXISTENTE"' "" "Variable inexistente con comillas dobles"

print_test "EXPORT - CREACIÓN DE VARIABLES"
run_test 'export TEST_VAR="valor_test"' "" "Export variable simple"
run_test 'export PATH_BACKUP=$PATH' "" "Export con expansión de variable"
run_test 'export NUM=42' "" "Export variable numérica"

print_test "VERIFICACIÓN DE EXPORT"
run_test 'export TEST_VAR=test && echo $TEST_VAR' "" "Verificar variable exportada"
run_test 'export PATH_TEST=$HOME && echo $PATH_TEST' "" "Verificar expansión en export"

print_test "UNSET - ELIMINACIÓN DE VARIABLES"
run_test 'export TEMP_VAR=temp && unset TEMP_VAR && echo $TEMP_VAR' "" "Unset de variable"

print_test "ENV - LISTADO DE VARIABLES"
echo 'env | grep USER' | ./minishell | grep -q "USER="
if [ $? -eq 0 ]; then
    print_success "env muestra variables"
else
    print_error "env no muestra variables correctamente"
fi

print_test "REDIRECCIONES - SALIDA"
echo 'echo "contenido_test" > test_output.txt' | ./minishell
if [ -f "test_output.txt" ] && grep -q "contenido_test" "test_output.txt" 2>/dev/null; then
    print_success "Redirección de salida (>) funciona"
    rm -f test_output.txt
else
    print_error "Redirección de salida (>) no funciona"
fi

print_test "REDIRECCIONES - APPEND"
echo 'echo "linea1" > test_append.txt && echo "linea2" >> test_append.txt' | ./minishell
if [ -f "test_append.txt" ] && [ $(wc -l < "test_append.txt" 2>/dev/null) -eq 2 ] 2>/dev/null; then
    print_success "Redirección append (>>) funciona"
    rm -f test_append.txt
else
    print_error "Redirección append (>>) no funciona"
fi

print_test "REDIRECCIONES - ENTRADA"
echo "test_content" > test_input.txt
echo 'cat < test_input.txt' | ./minishell | grep -q "test_content"
if [ $? -eq 0 ]; then
    print_success "Redirección de entrada (<) funciona"
else
    print_error "Redirección de entrada (<) no funciona"
fi
rm -f test_input.txt

print_test "PIPES SIMPLES"
echo 'echo "hello world" | grep hello' | ./minishell | grep -q "hello"
if [ $? -eq 0 ]; then
    print_success "Pipe simple funciona"
else
    print_error "Pipe simple no funciona"
fi

print_test "PIPES CON BUILTINS"
echo 'env | grep USER' | ./minishell | grep -q "USER="
if [ $? -eq 0 ]; then
    print_success "Pipe con builtin funciona"
else
    print_error "Pipe con builtin no funciona"
fi

print_test "HEREDOC BÁSICO"
echo -e 'cat << EOF\nlinea de heredoc\notra linea\nEOF' | ./minishell 2>/dev/null | grep -q "linea de heredoc"
if [ $? -eq 0 ]; then
    print_success "Heredoc básico funciona"
else
    print_error "Heredoc básico no funciona"
fi

print_test "COMANDOS EXTERNOS"
echo 'ls /bin | head -5' | ./minishell | wc -l | grep -q "5"
if [ $? -eq 0 ]; then
    print_success "Comandos externos funcionan"
else
    print_error "Comandos externos no funcionan"
fi

print_test "CASOS DE ERROR"
echo 'comando_inexistente 2>/dev/null' | ./minishell >/dev/null 2>&1
# No checkeamos el resultado porque esperamos que falle

print_test "SINTAXIS MIXTA"
run_test 'export MIX_VAR="test" && echo "Variable: $MIX_VAR"' "" "Export y echo combinados"
run_test 'echo "Comillas dobles: $USER" y '\''comillas simples: $USER'\''' "" "Mezcla de tipos de comillas"

# Limpiar archivos de prueba
rm -f test_*.txt

echo ""
echo "=================================================="
echo -e "${BLUE}📊 RESUMEN DE PRUEBAS${NC}"
echo "Total de pruebas: $total_tests"
echo "Pruebas exitosas: $passed_tests"
echo "Pruebas fallidas: $((total_tests - passed_tests))"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "${GREEN}🎉 ¡TODAS LAS PRUEBAS PASARON!${NC}"
    exit 0
else
    echo -e "${YELLOW}⚠️  Algunas pruebas fallaron. Revisa la implementación.${NC}"
    exit 1
fi
