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
