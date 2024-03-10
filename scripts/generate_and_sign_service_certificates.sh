#!/bin/bash

usage() {
  echo "Uso: $0 -p <caminho> -f <nome_do_arquivo> -c <ca_certificate_filename> -k <ca_key_filename>"
  exit 1
}

if [ "$#" -eq 0 ]; then
  usage
fi

path=""
filename=""
ca_certificate_filename=""
ca_key_filename=""

while getopts ":p:f:c:k:" opt; do
  case $opt in
    p)
      path="$OPTARG"
      ;;
    f)
      filename="$OPTARG"
      ;;
    c)
      ca_certificate_filename="$OPTARG"
      ;;
    k)
      ca_key_filename="$OPTARG"
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

if [ -z "$path" ] || [ -z "$filename" ] || [ -z "$ca_certificate_filename" ] || [ -z "$ca_key_filename" ]; then
  echo "Erro: Forneça caminho, nome do arquivo, certificado CA e chave privada CA."
  usage
fi

if ! [ -e "$ca_certificate_filename" ] || ! [ -e "$ca_key_filename" ]; then
  echo "Erro: Arquivos CA não encontrados. Verifique os caminhos informados e tente novamente."
  exit 1
fi

service_filename="${path%/}/$filename"

openssl genrsa -out "$service_filename.key" 2048
openssl req -new -out "$service_filename.csr" -key "$service_filename.key"
openssl x509 -req -in "$service_filename.csr" -CA "$ca_certificate_filename" -CAkey "$ca_key_filename" -CAcreateserial -out "$service_filename.crt" -days 360

echo "Chave e certificado CA gerados e assinados com sucesso!"
echo "Chave: $service_filename.key"
echo "Certificado: $service_filename.crt"
