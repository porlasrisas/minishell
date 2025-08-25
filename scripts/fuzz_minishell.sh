#!/usr/bin/env bash
# Fuzz y test exhaustivo para minishell (subject 42)
# Requiere: bash, timeout, diff, grep, sed, awk
set -e -o pipefail

BIN="./minishell"
OUT="scripts/out"
REF_SHELL="/bin/sh"   # Referencia para algunos comportamientos (no todos aplican)
mkdir -p "$OUT"

ok()  { printf "[OK] %s\n" "$1"; }
ko()  { printf "[FAIL] %s\n" "$1"; }
hr()  { printf "\n==== %s ====\n" "$1"; }
# Filtramos líneas "exit" que imprime minishell al salir
run() { LC_ALL=C printf "%b" "$1" | $BIN | sed '/^exit$/d' >"$2" 2>"$3" || true; }

# Helpers
check_regex() { # file regex desc
  if grep -E -q "$2" "$1"; then ok "$3"; else echo "--- $1 ---"; sed -n '1,120p' "$1"; ko "$3"; fi
}

# 1) Builtins básicos y con redirecciones
hr "BUILTINS: echo/pwd/env/export/unset/exit"
run $'echo hola\nexit\n' "$OUT/echo.txt" "$OUT/echo.err"
check_regex "$OUT/echo.txt" '(^|[^a-zA-Z])hola($|[^a-zA-Z])' 'echo imprime hola'
run $'pwd\nexit\n' "$OUT/pwd.txt" "$OUT/pwd.err"
check_regex "$OUT/pwd.txt" '/|\.' 'pwd prints path'
run $'env\nexit\n' "$OUT/env.txt" "$OUT/env.err"
check_regex "$OUT/env.txt" '^PATH=' 'env muestra PATH'
run $'export A=1\necho $A\nunset A\necho $A\nexit\n' "$OUT/export_unset.txt" "$OUT/export_unset.err"
check_regex "$OUT/export_unset.txt" '(^|[^0-9])1($|[^0-9])' 'export A=1'
check_regex "$OUT/export_unset.txt" '^$' 'unset A limpia valor'

# 2) Redirecciones
hr ">, >>, <, combinadas"
rm -f "$OUT/o.txt" "$OUT/a.txt"
run $'echo x > scripts/out/o.txt\ncat < scripts/out/o.txt\nexit\n' "$OUT/redir1.txt" "$OUT/redir1.err"
check_regex "$OUT/redir1.txt" '^x$' '> y <'
run $'echo a > scripts/out/a.txt\necho b >> scripts/out/a.txt\ncat scripts/out/a.txt\nexit\n' "$OUT/redir2.txt" "$OUT/redir2.err"
check_regex "$OUT/redir2.txt" '^a$' 'append a'
check_regex "$OUT/redir2.txt" '^b$' 'append b'
# output redir sobre builtin
run $'echo z > scripts/out/builtin_out.txt\nexit\n' "$OUT/redir3.txt" "$OUT/redir3.err"
check_regex scripts/out/builtin_out.txt '^z$' 'redir de builtin echo > file'

# 3) Pipelines
hr "Pipelines simples y múltiples"
run $'echo -n hi | wc -c\nexit\n' "$OUT/pipe1.txt" "$OUT/pipe1.err"
check_regex "$OUT/pipe1.txt" '(^|[^0-9])2($|[^0-9])' 'echo->wc'
run $'echo hola | tr a-z A-Z | grep HOLA\nexit\n' "$OUT/pipe2.txt" "$OUT/pipe2.err"
check_regex "$OUT/pipe2.txt" '^HOLA$' 'pipeline de 3 cmds'
# pipelines con builtins
run $'env | grep ^SHELL= | wc -l\nexit\n' "$OUT/pipe3.txt" "$OUT/pipe3.err"
check_regex "$OUT/pipe3.txt" '^[[:space:]]*[0-9]+[[:space:]]*$' 'pipeline con builtin env'

# 4) Heredoc
hr "Heredoc"
run $'cat << EOF\nline1\nline2\nEOF\nexit\n' "$OUT/hd1.txt" "$OUT/hd1.err"
check_regex "$OUT/hd1.txt" '^line1$' 'heredoc line1'
check_regex "$OUT/hd1.txt" '^line2$' 'heredoc line2'

# 5) Expansión de variables
hr "Expansión de $VAR, $?, comillas"
run $'echo $HOME\nexit\n' "$OUT/exp1.txt" "$OUT/exp1.err"
check_regex "$OUT/exp1.txt" '/' '$HOME expande'
run $'echo "$USER"\nexit\n' "$OUT/exp2.txt" "$OUT/exp2.err"
check_regex "$OUT/exp2.txt" '.' '"$USER" expande'
run $"echo '$USER'\nexit\n" "$OUT/exp3.txt" "$OUT/exp3.err"
check_regex "$OUT/exp3.txt" '\$USER' 'no expande en comillas simples'
run $'nosuchcmd\necho $?\nexit\n' "$OUT/exp4.txt" "$OUT/exp4.err"
check_regex "$OUT/exp4.txt" '(^|[^0-9])127($|[^0-9])' '$? tras not found'

# 6) Errores y edge cases
hr "Errores: not found, permisos, sintaxis"
run $'nosuchcmd\nexit\n' "$OUT/err1.txt" "$OUT/err1.err"
check_regex "$OUT/err1.err" 'command not found' 'mensaje not found'
# permiso denegado
printf '#!/bin/false\n' > "$OUT/noexec.sh"; chmod 000 "$OUT/noexec.sh"
run $'./scripts/out/noexec.sh\nexit\n' "$OUT/err2.txt" "$OUT/err2.err"
check_regex "$OUT/err2.err" 'Permission denied|permission denied' 'permiso denegado'
# sintaxis de pipes
run $'| echo x\nexit\n' "$OUT/err3.txt" "$OUT/err3.err"
check_regex "$OUT/err3.txt" '^$' 'pipe inicial rechazado'
run $'echo x | | wc\nexit\n' "$OUT/err4.txt" "$OUT/err4.err"
check_regex "$OUT/err4.txt" '^$' 'doble pipe rechazado'
# redirecciones sin target
run $'>\nexit\n' "$OUT/err5.txt" "$OUT/err5.err"
check_regex "$OUT/err5.txt" '^$' 'redir sin fichero'

# 7) PATH y rutas
hr "PATH y rutas"
run $'/bin/echo OK\nexit\n' "$OUT/path1.txt" "$OUT/path1.err"
check_regex "$OUT/path1.txt" '^OK$' 'ruta absoluta'
run $'echo OK | cat\nexit\n' "$OUT/path2.txt" "$OUT/path2.err"
check_regex "$OUT/path2.txt" '^OK$' 'bin en PATH'

# 8) Señales (no interactivo: simulamos SIGINT en heredoc es complicado)
hr "Señales (parcial)"
run $'sh -c "kill -3 $$"\nexit\n' "$OUT/sig1.txt" "$OUT/sig1.err" || true
if grep -q 'Quit: 3' "$OUT/sig1.err"; then ok 'SIGQUIT mensaje'; else echo '[WARN] SIGQUIT no observado (entorno)'; fi

# 9) Fuzzing básico de tokens (sin &&, ||, ;)
hr "Fuzz tokens"
TOKS=( echo pwd env export unset cat grep wc head tail tr sort uniq cut sed xargs true false )
for i in $(seq 1 50); do
  cmd=""
  words=$(( (RANDOM % 4) + 1 ))
  for j in $(seq 1 $words); do
    t=${TOKS[$RANDOM % ${#TOKS[@]}]}
    arg=$((RANDOM%2))
    if [[ $arg -eq 1 ]]; then t="$t $((RANDOM%100))"; fi
    if [[ -z "$cmd" ]]; then cmd="$t"; else cmd="$cmd | $t"; fi
  done
  if [[ $((RANDOM%3)) -eq 0 ]]; then cmd="$cmd > $OUT/fz_$i.txt"; fi
  run "$cmd\nexit\n" "$OUT/fuzz_$i.txt" "$OUT/fuzz_$i.err"
 done
ok "Fuzz aleatorio ejecutado (50 casos)"

hr "Resumen"
ls -1 "$OUT" | wc -l | awk '{print "Archivos generados:", $1}'
echo "Resultados en: $OUT"
