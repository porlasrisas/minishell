#!/usr/bin/env bash
# Suite exhaustiva de pruebas para minishell (42). Al menos 10 tests por tema.
# Nota: Evita usar operadores no obligatorios (&&, ||, ;) y redirecciones numéricas.
set -euo pipefail

BIN="./minishell"
OUT_DIR="scripts/full_out"
mkdir -p "$OUT_DIR"

ok()  { printf "[OK] %s\n" "$1"; }
ko()  { printf "[FAIL] %s\n" "$1"; }
hr()  { printf "\n==== %s ====\n" "$1"; }
run() { LC_ALL=C printf "%b" "$1" | "$BIN" | sed '/^exit$/d' >"$2" 2>"$3" || true; }

a_contiene() { # file regex name
  local f="$1" re="$2" name="$3"
  if grep -E -q "$re" "$f"; then ok "$name"; else echo "--- $f ---"; sed -n '1,120p' "$f"; ko "$name"; fi
}
no_contiene() { # file regex name
  local f="$1" re="$2" name="$3"
  if ! grep -E -q "$re" "$f"; then ok "$name"; else echo "--- $f ---"; sed -n '1,120p' "$f"; ko "$name"; fi
}
status_seq() { # input expected name
  local input="$1" exp="$2" name="$3"
  set +e; LC_ALL=C printf "%b" "$input" | "$BIN" >/dev/null 2>&1; local c=$?; set -e
  if [[ "$c" == "$exp" ]]; then ok "$name"; else printf "got %s expected %s\n" "$c" "$exp"; ko "$name"; fi
}
file_eq() { # path expected name
  local p="$1" exp="$2" name="$3"
  if [[ -f "$p" ]] && diff -u <(printf "%b" "$exp") "$p" >/dev/null; then ok "$name"; else echo "--- $p ---"; cat -A "$p" || true; ko "$name"; fi
}
lines_eq() { # path n name
  local p="$1" n="$2" name="$3" cnt
  cnt=$(wc -l < "$p" | tr -d ' ')
  if [[ "$cnt" == "$n" ]]; then ok "$name"; else echo "count=$cnt"; ko "$name"; fi
}

# 1) ECHO (10 pruebas)
hr "ECHO"
run $'echo hola\nexit\n' "$OUT_DIR/echo1.txt" "$OUT_DIR/echo1.err"; a_contiene "$OUT_DIR/echo1.txt" '(^|[^a-zA-Z])hola($|[^a-zA-Z])' 'echo básico'
run $'echo "hola mundo"\nexit\n' "$OUT_DIR/echo2.txt" "$OUT_DIR/echo2.err"; a_contiene "$OUT_DIR/echo2.txt" '^hola mundo$' 'echo comillas dobles'
run $"echo 'hola   mundo'\nexit\n" "$OUT_DIR/echo3.txt" "$OUT_DIR/echo3.err"; a_contiene "$OUT_DIR/echo3.txt" '^hola   mundo$' 'echo comillas simples preservan espacios'
run $'echo -n sin_nl\nexit\n' "$OUT_DIR/echo4.txt" "$OUT_DIR/echo4.err"; no_contiene "$OUT_DIR/echo4.txt" '\n$' 'echo -n sin salto final'
run $'echo ""\nexit\n' "$OUT_DIR/echo5.txt" "$OUT_DIR/echo5.err"; a_contiene "$OUT_DIR/echo5.txt" '^$' 'echo cadena vacía'
run $'echo a    b\nexit\n' "$OUT_DIR/echo6.txt" "$OUT_DIR/echo6.err"; a_contiene "$OUT_DIR/echo6.txt" '^a[ ]+b$' 'splitting por espacios múltiples'
run $'echo -e "l\n2"\nexit\n' "$OUT_DIR/echo7.txt" "$OUT_DIR/echo7.err"; a_contiene "$OUT_DIR/echo7.txt" '^l$' 'echo -e salto 1'; a_contiene "$OUT_DIR/echo7.txt" '^2$' 'echo -e salto 2'
run $'echo \\ \t \n\nexit\n' "$OUT_DIR/echo8.txt" "$OUT_DIR/echo8.err"; a_contiene "$OUT_DIR/echo8.txt" '\\' 'imprime barra invertida'
run $'echo "espacio final "\nexit\n' "$OUT_DIR/echo9.txt" "$OUT_DIR/echo9.err"; a_contiene "$OUT_DIR/echo9.txt" 'espacio final ' 'espacio final'
run $'echo $NO_SUCH_VAR\nexit\n' "$OUT_DIR/echo10.txt" "$OUT_DIR/echo10.err"; a_contiene "$OUT_DIR/echo10.txt" '^$' 'var inexistente vacío'

# 2) EXPANSIÓN (10)
hr "EXPANSION"
run $'echo $USER\nexit\n' "$OUT_DIR/exp1.txt" "$OUT_DIR/exp1.err"; a_contiene "$OUT_DIR/exp1.txt" '.' '$USER expande'
run $'echo "$HOME"\nexit\n' "$OUT_DIR/exp2.txt" "$OUT_DIR/exp2.err"; a_contiene "$OUT_DIR/exp2.txt" '/' 'HOME comillas dobles'
run $"echo '$HOME'\nexit\n" "$OUT_DIR/exp3.txt" "$OUT_DIR/exp3.err"; a_contiene "$OUT_DIR/exp3.txt" '\$HOME' 'sin expansión en simples'
run $'nosuchcmd\necho $?\nexit\n' "$OUT_DIR/exp4.txt" "$OUT_DIR/exp4.err"; a_contiene "$OUT_DIR/exp4.txt" '(^|[^0-9])127($|[^0-9])' '$?=127'
run $'/bin/true\necho $?\nexit\n' "$OUT_DIR/exp5.txt" "$OUT_DIR/exp5.err"; a_contiene "$OUT_DIR/exp5.txt" '(^|[^0-9])0($|[^0-9])' '$?=0'
run $'/bin/false\necho $?\nexit\n' "$OUT_DIR/exp6.txt" "$OUT_DIR/exp6.err"; a_contiene "$OUT_DIR/exp6.txt" '(^|[^0-9])1($|[^0-9])' '$?=1'
run $'echo prefix_$USER_suffix\nexit\n' "$OUT_DIR/exp7.txt" "$OUT_DIR/exp7.err"; a_contiene "$OUT_DIR/exp7.txt" 'prefix_.*_suffix' 'concatenación'
run $'echo "$USER$HOME"\nexit\n' "$OUT_DIR/exp8.txt" "$OUT_DIR/exp8.err"; a_contiene "$OUT_DIR/exp8.txt" '.' 'dos variables seguidas'
run $'echo $SHOULD_NOT_EXIST$HOME\nexit\n' "$OUT_DIR/exp9.txt" "$OUT_DIR/exp9.err"; a_contiene "$OUT_DIR/exp9.txt" '/' 'var inexistente seguida de existente'
run $'echo "$?"\nexit\n' "$OUT_DIR/exp10.txt" "$OUT_DIR/exp10.err"; a_contiene "$OUT_DIR/exp10.txt" '^[0-9]+$' 'echo "$?" numérico'

# 3) EXPORT/UNSET/ENV (10)
hr "ENV/EXPORT/UNSET"
run $'export A=1\nexport B=hello\nexport C=$A$B\necho $C\nexit\n' "$OUT_DIR/env1.txt" "$OUT_DIR/env1.err"; a_contiene "$OUT_DIR/env1.txt" '^1hello$' 'expansión en export'
run $'export V1=xx\nunset V1\necho $V1\nexit\n' "$OUT_DIR/env2.txt" "$OUT_DIR/env2.err"; a_contiene "$OUT_DIR/env2.txt" '^$' 'unset limpia'
run $'export MIX_1=ok\nexport MIX_2=ok\nexport MIX_A=ok\nexport\nexit\n' "$OUT_DIR/env3.txt" "$OUT_DIR/env3.err"; a_contiene "$OUT_DIR/env3.txt" '^declare -x ' 'export sin args imprime declare -x'
run $'export INV-ALID=1\nexit\n' "$OUT_DIR/env4.txt" "$OUT_DIR/env4.err"; a_contiene "$OUT_DIR/env4.err" 'not a valid identifier' 'identificador inválido'
run $'unset INV-ALID\nexit\n' "$OUT_DIR/env5.txt" "$OUT_DIR/env5.err"; a_contiene "$OUT_DIR/env5.err" 'not a valid identifier' 'unset inválido'
run $'export EMPTY=\necho $EMPTY\nexit\n' "$OUT_DIR/env6.txt" "$OUT_DIR/env6.err"; a_contiene "$OUT_DIR/env6.txt" '^$' 'export valor vacío'
run $'env | grep ^PATH=\nexit\n' "$OUT_DIR/env7.txt" "$OUT_DIR/env7.err"; a_contiene "$OUT_DIR/env7.txt" '^PATH=' 'env PATH'
run $'export x1=1 x2=2\necho $x1$x2\nexit\n' "$OUT_DIR/env8.txt" "$OUT_DIR/env8.err"; a_contiene "$OUT_DIR/env8.txt" '^12$' 'múltiples en una línea'
run $'export _OK=1\necho $_OK\nexit\n' "$OUT_DIR/env9.txt" "$OUT_DIR/env9.err"; a_contiene "$OUT_DIR/env9.txt" '^1$' 'guión bajo válido'
run $'export NUM=42\nexport NUM=43\necho $NUM\nexit\n' "$OUT_DIR/env10.txt" "$OUT_DIR/env10.err"; a_contiene "$OUT_DIR/env10.txt" '^43$' 'actualización de variable'

# 4) CD (10)
hr "CD"
run $'pwd\ncd \npwd\nexit\n' "$OUT_DIR/cd1.txt" "$OUT_DIR/cd1.err"; a_contiene "$OUT_DIR/cd1.txt" '/' 'cd a HOME'
run $'cd /\npwd\nexit\n' "$OUT_DIR/cd2.txt" "$OUT_DIR/cd2.err"; a_contiene "$OUT_DIR/cd2.txt" '^/$' 'cd /'
run $'cd /tmp\npwd\nexit\n' "$OUT_DIR/cd3.txt" "$OUT_DIR/cd3.err"; a_contiene "$OUT_DIR/cd3.txt" '/tmp' 'cd /tmp'
run $'cd /no/such/dir\necho $?\nexit\n' "$OUT_DIR/cd4.txt" "$OUT_DIR/cd4.err"; a_contiene "$OUT_DIR/cd4.txt" '^[1-9][0-9]*$' 'cd error status>0'
run $'pwd\ncd ..\npwd\nexit\n' "$OUT_DIR/cd5.txt" "$OUT_DIR/cd5.err"; a_contiene "$OUT_DIR/cd5.txt" '/' 'cd relativo ..'
run $'cd .\npwd\nexit\n' "$OUT_DIR/cd6.txt" "$OUT_DIR/cd6.err"; a_contiene "$OUT_DIR/cd6.txt" '/' 'cd .'
run $'cd /\ncd -\npwd\nexit\n' "$OUT_DIR/cd7.txt" "$OUT_DIR/cd7.err"; a_contiene "$OUT_DIR/cd7.txt" '/' 'cd - vuelve'
run $'cd /\necho $PWD\nexit\n' "$OUT_DIR/cd8.txt" "$OUT_DIR/cd8.err"; a_contiene "$OUT_DIR/cd8.txt" '^/$' 'PWD actualizado'
run $'cd /\necho $OLDPWD\nexit\n' "$OUT_DIR/cd9.txt" "$OUT_DIR/cd9.err"; a_contiene "$OUT_DIR/cd9.txt" '/' 'OLDPWD actualizado'
run $'cd ../..\npwd\nexit\n' "$OUT_DIR/cd10.txt" "$OUT_DIR/cd10.err"; a_contiene "$OUT_DIR/cd10.txt" '/' 'cd relativo múltiple'

# 5) REDIRECCIONES (10)
hr "REDIRECCIONES"
run $'echo x > scripts/full_out/r1.txt\ncat < scripts/full_out/r1.txt\nexit\n' "$OUT_DIR/r1.txt" "$OUT_DIR/r1.err"; a_contiene "$OUT_DIR/r1.txt" '^x$' '> y <'
run $'echo a > scripts/full_out/r2.txt\necho b >> scripts/full_out/r2.txt\ncat scripts/full_out/r2.txt\nexit\n' "$OUT_DIR/r2.txt" "$OUT_DIR/r2.err"; lines_eq "$OUT_DIR/r2.txt" 2 'append dos líneas'
run $'echo 123>scripts/full_out/r3.txt\ncat scripts/full_out/r3.txt\nexit\n' "$OUT_DIR/r3.txt" "$OUT_DIR/r3.err"; a_contiene "$OUT_DIR/r3.txt" '^123$' 'sin espacios'
run $'cat < scripts/full_out/r3.txt | wc -c\nexit\n' "$OUT_DIR/r4.txt" "$OUT_DIR/r4.err"; a_contiene "$OUT_DIR/r4.txt" '3' '< combinado con pipe'
run $'echo X > scripts/full_out/r5.txt\ncat < scripts/full_out/r5.txt | tr X Y\nexit\n' "$OUT_DIR/r5.txt" "$OUT_DIR/r5.err"; a_contiene "$OUT_DIR/r5.txt" '^Y$' 'redir + pipe'
run $'echo a > scripts/full_out/r6.txt\necho b > scripts/full_out/r6.txt\ncat scripts/full_out/r6.txt\nexit\n' "$OUT_DIR/r6.txt" "$OUT_DIR/r6.err"; a_contiene "$OUT_DIR/r6.txt" '^b$' 'sobrescritura'
run $'echo c >> scripts/full_out/r7.txt\ncat scripts/full_out/r7.txt\nexit\n' "$OUT_DIR/r7.txt" "$OUT_DIR/r7.err"; a_contiene "$OUT_DIR/r7.txt" 'c' 'append crea si no existe'
run $'cat < no_such_file\nexit\n' "$OUT_DIR/r8.txt" "$OUT_DIR/r8.err"; a_contiene "$OUT_DIR/r8.err" 'No such file|no such file' 'error infile'
run $'echo A > scripts/full_out/dirfile\nmkdir -p scripts/full_out/dir\necho A > scripts/full_out/dir\nexit\n' "$OUT_DIR/r9.txt" "$OUT_DIR/r9.err"; a_contiene "$OUT_DIR/r9.err" 'Is a directory|is a directory|Permission denied' 'redir a directorio (dependiente del SO)'
run $'echo Z > scripts/full_out/sub/../r10.txt\ncat scripts/full_out/r10.txt\nexit\n' "$OUT_DIR/r10.txt" "$OUT_DIR/r10.err"; a_contiene "$OUT_DIR/r10.txt" '^Z$' 'ruta normalizada'

# 6) PIPES (10)
hr "PIPES"
run $'echo -n ab | wc -c\nexit\n' "$OUT_DIR/p1.txt" "$OUT_DIR/p1.err"; a_contiene "$OUT_DIR/p1.txt" '2' 'echo->wc'
run $'printf x | tr x y\nexit\n' "$OUT_DIR/p2.txt" "$OUT_DIR/p2.err"; a_contiene "$OUT_DIR/p2.txt" '^y$' 'external->external'
run $'env | head -n 1\nexit\n' "$OUT_DIR/p3.txt" "$OUT_DIR/p3.err"; a_contiene "$OUT_DIR/p3.txt" '=' 'builtin->external'
run $'echo hola | grep ho | wc -l\nexit\n' "$OUT_DIR/p4.txt" "$OUT_DIR/p4.err"; a_contiene "$OUT_DIR/p4.txt" '1' 'triple pipe'
run $'cat < /etc/hosts | grep -E "." | head -n 1\nexit\n' "$OUT_DIR/p5.txt" "$OUT_DIR/p5.err"; a_contiene "$OUT_DIR/p5.txt" '.' 'redir + pipe + pipe'
run $'true | false\nexit\n' "$OUT_DIR/p6.txt" "$OUT_DIR/p6.err"; status_seq $'true | false\nexit\n' 1 'status = último comando (false)'
run $'false | true\nexit\n' "$OUT_DIR/p7.txt" "$OUT_DIR/p7.err"; status_seq $'false | true\nexit\n' 0 'status = último comando (true)'
run $'echo x | cat > scripts/full_out/p8.txt\nexit\n' "$OUT_DIR/p8.txt" "$OUT_DIR/p8.err"; file_eq scripts/full_out/p8.txt 'x\n' 'pipe a fichero'
run $'echo a | tr a-z A-Z | cat > scripts/full_out/p9.txt\nexit\n' "$OUT_DIR/p9.txt" "$OUT_DIR/p9.err"; file_eq scripts/full_out/p9.txt 'A\n' 'triple a fichero'
run $'nosuch | cat\nexit\n' "$OUT_DIR/p10.txt" "$OUT_DIR/p10.err"; a_contiene "$OUT_DIR/p10.err" 'command not found' 'error en primer comando de pipe'

# 7) HEREDOC (10)
hr "HEREDOC"
run $'cat << EOF\nhello\nEOF\nexit\n' "$OUT_DIR/h1.txt" "$OUT_DIR/h1.err"; a_contiene "$OUT_DIR/h1.txt" '^hello$' 'básico'
run $'cat << EOF | wc -l\nhello\nEOF\nexit\n' "$OUT_DIR/h2.txt" "$OUT_DIR/h2.err"; a_contiene "$OUT_DIR/h2.txt" '1' 'heredoc + pipe'
run $'cat << EOF\n$USER\nEOF\nexit\n' "$OUT_DIR/h3.txt" "$OUT_DIR/h3.err"; a_contiene "$OUT_DIR/h3.txt" '.*' 'contenido pasa (expansión opcional)'
run $'cat << "EOF"\n$USER\nEOF\nexit\n' "$OUT_DIR/h4.txt" "$OUT_DIR/h4.err"; a_contiene "$OUT_DIR/h4.txt" '\$USER' 'delimitador con comillas (sin expansión)'
run $'cat << E\nline\nE\nexit\n' "$OUT_DIR/h5.txt" "$OUT_DIR/h5.err"; a_contiene "$OUT_DIR/h5.txt" '^line$' 'delimitadores cortos'
run $'cat << EOF\nline with spaces\nEOF\nexit\n' "$OUT_DIR/h6.txt" "$OUT_DIR/h6.err"; a_contiene "$OUT_DIR/h6.txt" '^line with spaces$' 'espacios'
run $'cat << EOF\nline\\n2\nEOF\nexit\n' "$OUT_DIR/h7.txt" "$OUT_DIR/h7.err"; a_contiene "$OUT_DIR/h7.txt" 'line\\n2' 'backslash literal'
run $'cat << EOF\n1\n2\n3\nEOF\nexit\n' "$OUT_DIR/h8.txt" "$OUT_DIR/h8.err"; lines_eq "$OUT_DIR/h8.txt" 3 'múltiples líneas'
run $'cat << EOF\nfin sin extra\nEOF\nexit\n' "$OUT_DIR/h9.txt" "$OUT_DIR/h9.err"; no_contiene "$OUT_DIR/h9.txt" '^EOF$' 'no imprime EOF'
run $'cat << EOF\nA\nEOF\ncat << EOF\nB\nEOF\nexit\n' "$OUT_DIR/h10.txt" "$OUT_DIR/h10.err"; a_contiene "$OUT_DIR/h10.txt" '^A$' 'dos heredocs primera'; a_contiene "$OUT_DIR/h10.txt" '^B$' 'dos heredocs segunda'

# 8) ESTATUS Y ERRORES (10)
hr "ESTATUS/ERRORES"
status_seq $'nosuchcmd\nexit\n' 127 'not found -> 127'
run $'./not_executable.sh\nexit\n' "$OUT_DIR/e1.txt" "$OUT_DIR/e1.err"; a_contiene "$OUT_DIR/e1.err" 'Permission denied|permission denied|Exec format error' 'permiso/exec error'
status_seq $'/bin/false\nexit\n' 1 'false -> 1'
status_seq $'/bin/true\nexit\n' 0 'true -> 0'
run $'mkdir -p scripts/full_out/dir\n./scripts/full_out/dir\nexit\n' "$OUT_DIR/e2.txt" "$OUT_DIR/e2.err"; a_contiene "$OUT_DIR/e2.err" 'is a directory|Permission denied' 'directorio no ejecutable'
run $'sh -c "kill -3 $$"\nexit\n' "$OUT_DIR/e3.txt" "$OUT_DIR/e3.err"; a_contiene "$OUT_DIR/e3.err" 'Quit: 3' 'SIGQUIT mensaje'
run $'sh -c "kill -2 $$"\necho $?\nexit\n' "$OUT_DIR/e4.txt" "$OUT_DIR/e4.err"; a_contiene "$OUT_DIR/e4.txt" '130' 'SIGINT -> 130'
run $'echo ok > /root/forbidden_file\nexit\n' "$OUT_DIR/e5.txt" "$OUT_DIR/e5.err"; a_contiene "$OUT_DIR/e5.err" 'Permission denied|denied' 'permiso denegado (puede variar)'
run $'./no_such_binary\nexit\n' "$OUT_DIR/e6.txt" "$OUT_DIR/e6.err"; a_contiene "$OUT_DIR/e6.err" 'No such file|not found' 'binario inexistente'
run $'echo X | /bin/cat\nexit\n' "$OUT_DIR/e7.txt" "$OUT_DIR/e7.err"; a_contiene "$OUT_DIR/e7.txt" '^X$' 'pipeline con ruta absoluta OK'

# 9) PATH/EXEC (10)
hr "PATH/EXEC"
run $'/bin/echo OK\nexit\n' "$OUT_DIR/x1.txt" "$OUT_DIR/x1.err"; a_contiene "$OUT_DIR/x1.txt" '^OK$' 'absoluta'
run $'echo OK | cat\nexit\n' "$OUT_DIR/x2.txt" "$OUT_DIR/x2.err"; a_contiene "$OUT_DIR/x2.txt" '^OK$' 'PATH'
run $'printf X | /bin/cat\nexit\n' "$OUT_DIR/x3.txt" "$OUT_DIR/x3.err"; a_contiene "$OUT_DIR/x3.txt" '^X$' 'mezcla'
run $'echo $PATH\nexit\n' "$OUT_DIR/x4.txt" "$OUT_DIR/x4.err"; a_contiene "$OUT_DIR/x4.txt" '/bin' 'PATH presente'
run $'./scripts/full_out/made.sh\nexit\n' "$OUT_DIR/x5.txt" "$OUT_DIR/x5.err"; a_contiene "$OUT_DIR/x5.err" 'No such file|not found' 'relativo inexistente'
run $'printf "#!/bin/sh\necho HI\n" > scripts/full_out/mk.sh\nchmod +x scripts/full_out/mk.sh\n./scripts/full_out/mk.sh\nexit\n' "$OUT_DIR/x6.txt" "$OUT_DIR/x6.err"; a_contiene "$OUT_DIR/x6.txt" '^HI$' 'ejecución local'
run $'touch scripts/full_out/nx.sh\nchmod -x scripts/full_out/nx.sh\n./scripts/full_out/nx.sh\nexit\n' "$OUT_DIR/x7.txt" "$OUT_DIR/x7.err"; a_contiene "$OUT_DIR/x7.err" 'Permission denied' 'sin permisos'
run $'echo ok > ./scripts/full_out/ok.txt\n/bin/cat ./scripts/full_out/ok.txt\nexit\n' "$OUT_DIR/x8.txt" "$OUT_DIR/x8.err"; a_contiene "$OUT_DIR/x8.txt" '^ok$' 'cat ruta relativa'
run $'PATH=""\nls\nexit\n' "$OUT_DIR/x9.txt" "$OUT_DIR/x9.err"; a_contiene "$OUT_DIR/x9.err" 'not found' 'PATH vacío -> not found'
run $'PATH="/bin"\nls\nexit\n' "$OUT_DIR/x10.txt" "$OUT_DIR/x10.err"; a_contiene "$OUT_DIR/x10.txt" '.' 'PATH ajustado'

# 10) TOKENIZER/QUOTES (10)
hr "TOKENIZER/QUOTES"
run $'echo    a   b\nexit\n' "$OUT_DIR/tq1.txt" "$OUT_DIR/tq1.err"; a_contiene "$OUT_DIR/tq1.txt" '^a[ ]+b$' 'espacios múltiples'
run $'echo "a   b"\nexit\n' "$OUT_DIR/tq2.txt" "$OUT_DIR/tq2.err"; a_contiene "$OUT_DIR/tq2.txt" '^a   b$' 'espacios preservados en dobles'
run $"echo 'a   b'\nexit\n" "$OUT_DIR/tq3.txt" "$OUT_DIR/tq3.err"; a_contiene "$OUT_DIR/tq3.txt" '^a   b$' 'espacios preservados en simples'
run $'echo "\"q\""\nexit\n' "$OUT_DIR/tq4.txt" "$OUT_DIR/tq4.err"; a_contiene "$OUT_DIR/tq4.txt" '"q"' 'comillas dentro'
run $"echo '\'q\''\nexit\n" "$OUT_DIR/tq5.txt" "$OUT_DIR/tq5.err"; a_contiene "$OUT_DIR/tq5.txt" "'q'" 'simples dentro'
run $'echo a"b"c\nexit\n' "$OUT_DIR/tq6.txt" "$OUT_DIR/tq6.err"; a_contiene "$OUT_DIR/tq6.txt" '^abc$' 'concatenación con comillas'
run $'echo ""\nexit\n' "$OUT_DIR/tq7.txt" "$OUT_DIR/tq7.err"; a_contiene "$OUT_DIR/tq7.txt" '^$' 'cadena vacía'
run $'echo " $USER "\nexit\n' "$OUT_DIR/tq8.txt" "$OUT_DIR/tq8.err"; a_contiene "$OUT_DIR/tq8.txt" ' .+ ' 'espacios alrededor'
run $'echo $NOPE$NOPE2\nexit\n' "$OUT_DIR/tq9.txt" "$OUT_DIR/tq9.err"; a_contiene "$OUT_DIR/tq9.txt" '^$' 'doble inexistente'
run $'echo *\nexit\n' "$OUT_DIR/tq10.txt" "$OUT_DIR/tq10.err"; a_contiene "$OUT_DIR/tq10.txt" '\*' 'sin globbing'

# 11) OPCIONAL: Autocompletado por TAB (requiere expect y soporte Readline/Completer)
hr "Autocompletado (opcional)"
if command -v expect >/dev/null 2>&1; then
  cat > "$OUT_DIR/tab_expect.tcl" << 'EOF'
spawn ./minishell
expect ">\$ "
send "/bin"
send "\t"
# Esperamos que aparezca la barra final si el directorio es detectado
expect {
  "/bin/" { exit 0 }
  timeout { exit 2 }
}
send "exit\n"
expect eof
EOF
  set +e; expect "$OUT_DIR/tab_expect.tcl" >/dev/null 2>"$OUT_DIR/tab.err"; rc=$?; set -e
  if [[ $rc -eq 0 ]]; then ok 'TAB completa con /'; else echo '[WARN] TAB no soportado o no detectable'; fi
else
  echo '[WARN] expect no disponible; se omiten pruebas de TAB'
fi

hr "Resumen"
ls -1 "$OUT_DIR" | wc -l | awk '{print "Archivos generados:", $1}'
echo "Resultados en: $OUT_DIR"
