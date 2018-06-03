#pragma once
#include <Arduino.h>
#include "StringBuffer.cpp"
#include <xxtea.h>
#include <Base64.h>


static int uStatusCode(char *response) {
  char *temp = strstr(response, " ");
  int space = temp - response + 1;

  char subbuff[4] = {'\0'};
  memcpy( subbuff, &response[space], 3);

  return atoi(subbuff);
}


static int uFindHttpBody(char *response) {
  char *search = "\r\n\r\n";
  char *temp = strstr(response, search);
  return temp - response + 4;
}


static char* uDecryptBody(char *data, char *encryptionPassword) {
  int dataLen = strlen(data);
  int decodedLen = base64_dec_len(data, dataLen);
  char* decodedData = new char[decodedLen] {'\0'};
  base64_decode(decodedData, data, dataLen);
  char *json = xxtea_decrypt(decodedData, decodedLen, encryptionPassword, &decodedLen);
  delete [] decodedData;

  return json;
}


static char* uEncryptBody(char *json, char *encryptionPassword) {
  int size;
  char *data = xxtea_encrypt(json, strlen(json), encryptionPassword, &size);
  int encodedLen = base64_enc_len(size);
  char *encoded = new char[encodedLen];
  base64_encode(encoded, data, size);
  delete [] data;
  
  return encoded;
}
