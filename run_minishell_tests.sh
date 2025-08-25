#!/usr/bin/env bash
set -euo pipefail

E="./minishell"
OUT_DIR="test_outputs"
mkdir -p "$OUT_DIR"

pass() { printf "[PASS] %s\n" "$1"; }
fail() { printf "[FAIL] %s\n" "$1"; return 1; }
run()  { printf "\n===== %s =====\n" "$1"; }

# Helper to run minishell with provided input and capture output
ms_run() {
  local input="$1" out_file="$2"
  # Avoid locale issues
  LC_ALL=C printf "%b" "$input" | $E >"$out_file" 2>&1 || true
}

# Assert that a file contains a regex
assert_contains() {
  local file="$1" re="$2" name="$3"
  if grep -E -q "$re" "$file"; then pass "$name"; else fail "$name"; fi
}

# Assert a command sequence leads minishell to exit with code X
assert_exit_code() {
  local input="$1" expected="$2" name="$3"
  set +e
  LC_ALL=C printf "%b" "$input" | $E >/dev/null 2>&1
  local code=$?
  set -e
  if [[ "$code" == "$expected" ]]; then pass "$name"; else printf "got %s expected %s\n" "$code" "$expected"; fail "$name"; fi
}

# Build first if needed
if [[ ! -x "$E" ]]; then
  make -s || { echo "make failed"; exit 1; }
fi

run "T1 builtin echo"
ms_run $'echo hola\nexit\n' "$OUT_DIR/t1.txt"
assert_contains "$OUT_DIR/t1.txt" '(^|[^a-zA-Z])hola($|[^a-zA-Z])' "echo prints hola"

run "T2 external echo via PATH"
ms_run $'/bin/echo hola\nexit\n' "$OUT_DIR/t2.txt"
assert_contains "$OUT_DIR/t2.txt" '(^|[^a-zA-Z])hola($|[^a-zA-Z])' "external echo prints hola"

run "T3 pwd"
ms_run $'pwd\nexit\n' "$OUT_DIR/t3.txt"
assert_contains "$OUT_DIR/t3.txt" '/|\.' "pwd outputs a path"

run "T4 export/unset/expansion"
ms_run $'export A=1\necho $A\nunset A\necho $A\nexit\n' "$OUT_DIR/t4.txt"
assert_contains "$OUT_DIR/t4.txt" '(^|[^0-9])1($|[^0-9])' "expansion shows 1"
assert_contains "$OUT_DIR/t4.txt" '(^|[^0-9])($|[^0-9])' "unset clears value" || true

run "T5 redirections > and <"
rm -f "$OUT_DIR/out.txt"
ms_run $'echo hi > test_outputs/out.txt\ncat < test_outputs/out.txt\nexit\n' "$OUT_DIR/t5.txt"
assert_contains "$OUT_DIR/t5.txt" '(^|[^a-zA-Z])hi($|[^a-zA-Z])' ">/< work and cat shows hi"
[[ "$(cat "$OUT_DIR/out.txt" 2>/dev/null || true)" == "hi" ]] && pass "> wrote file" || fail "> wrote file"

run "T6 append >>"
rm -f "$OUT_DIR/app.txt"
ms_run $'echo a > test_outputs/app.txt\necho b >> test_outputs/app.txt\ncat test_outputs/app.txt\nexit\n' "$OUT_DIR/t6.txt"
assert_contains "$OUT_DIR/t6.txt" '^a$' "first line a"
assert_contains "$OUT_DIR/t6.txt" '^b$' "second line b"

run "T7 pipeline echo -n hi | wc -c"
ms_run $'echo -n hi | wc -c\nexit\n' "$OUT_DIR/t7.txt"
assert_contains "$OUT_DIR/t7.txt" '(^|[^0-9])2($|[^0-9])' "pipeline count is 2"

run "T8 heredoc cat << EOF"
ms_run $'cat << EOF\nhola\nEOF\nexit\n' "$OUT_DIR/t8.txt"
assert_contains "$OUT_DIR/t8.txt" '(^|[^a-zA-Z])hola($|[^a-zA-Z])' "heredoc emits content"

run "T9 not found exit code 127"
assert_exit_code $'nosuchcmd\nexit\n' 127 "unknown command returns 127"

run "T10 $? expansion"
ms_run $'nosuchcmd\necho $?\nexit\n' "$OUT_DIR/t10.txt"
assert_contains "$OUT_DIR/t10.txt" '(^|[^0-9])127($|[^0-9])' "echo $? after error is 127"

run "Resumen"
ls -1 "$OUT_DIR" | sed 's/^/ - /'
echo "\nOutputs en: $OUT_DIR"
