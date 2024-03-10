#!/bin/bash

usage() {
  echo "Uso: $0 -p <caminho> -f <nome_do_arquivo>"
  exit 1
}

if [ "$#" -eq 0 ]; then
  usage
fi

path=""
filename=""

while getopts ":p:f:" opt; do
  case $opt in
    p)
      path="$OPTARG"
      ;;
    f)
      filename="$OPTARG"
      ;;
    \?)
      echo "Opção inválida: -$OPTARG"
      usage
      ;;
    :)
      echo "A opção -$OPTARG requer um argumento."
      usage
      ;;
  esac
done

if [ -z "$path" ] || [ -z "$filename" ]; then
  echo "Erro: Forneça caminho e nome do arquivo."
  usage
fi

# Gera o caminho completo do arquivo
ca_filename="${path%/}/$filename"

# Verifica a existência do OpenSSL
if ! command -v openssl &>/dev/null; then
  echo "Erro: OpenSSL não encontrado. Certifique-se de que está instalado."
  exit 1
fi

openssl genrsa -des3 -out "$ca_filename.key" 2048
openssl req -new -x509 -days 1826 -key "$ca_filename.key" -out "$ca_filename.crt"

echo "Chave e certificado do CA gerados com sucesso!"
echo "Chave: $ca_filename.key"
echo "Certificado: $ca_filename.crt"
