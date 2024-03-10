#!/bin/bash
sudo apt-get update
sudo apt-get install openssl
# Gera a chave CA e certificado. Solicita uma a senha para ca.key:
openssl genrsa -des3 -out ca.key 2048;
openssl req -new -x509 -days 1826 -key ca.key -out ca.crt;
# Gera a chave do servidor e a solicitação de assinatura do certificado. Certifique-se de que o "Common Name" seja o nome do host do broker.
openssl genrsa -out server.key 2048;
openssl req -new -out server.csr -key server.key;
# Assina o certificado do servidor como CA.
openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 360;
# Gera a chave do cliente e a solicitação de assinatura do certificado. Certifique-se de que o "Common" seja exclusivo do cliente. Este será o seu nome de usuário.
openssl genrsa -out client.key 2048;
openssl req -new -out client.csr -key client.key;
# Assina o certificado do cliente como CA.
openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 360
