#define _CRT_SECURE_NO_WARNINGS
#define KEY_LENGTH 16
#define MESSAGE_CHUNK 160
#include <iostream>
#include <string.h>
#include <fstream>
#include <malloc.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/applink.c>
#include <openssl/pem.h>
#include <openssl/rsa.h>

void generateFileHashValue(FILE* file, unsigned char* hash) {
	unsigned char* fileBuffer = NULL;
	SHA256_CTX ctx;
	SHA256_Init(&ctx);

	fseek(file, 0, SEEK_END);
	int fileLength = ftell(file);
	fseek(file, 0, SEEK_SET);
	//printf("%d\n", fileLength);

	fileBuffer = (unsigned char*)malloc(fileLength);
	fread(fileBuffer, fileLength, 1, file);
	unsigned char* tmpBuffer = fileBuffer;

	while (fileLength > 0) {
		if (fileLength > MESSAGE_CHUNK) {
			SHA256_Update(&ctx, tmpBuffer, MESSAGE_CHUNK);
		}
		else {
			SHA256_Update(&ctx, tmpBuffer, fileLength);
		}
		fileLength -= MESSAGE_CHUNK;
		tmpBuffer += MESSAGE_CHUNK;
	}

	SHA256_Final(hash, &ctx);

}

int main()
{
	// 1

	FILE* b;
	FILE* bEnc;
	errno = 0;
	fopen_s(&b, "brancusi.txt", "rb");
	fopen_s(&bEnc, "bEnc.bin", "wb");

	fseek(b, 0, SEEK_END);
	long int length = ftell(b);
	fseek(b, 0, SEEK_SET);
	long int encLength = 0;
	if ((length % KEY_LENGTH) == 0) {
		encLength = length;
	}
	else {
		encLength = ((length / KEY_LENGTH) * KEY_LENGTH) + KEY_LENGTH;
	}

	unsigned char* inputBuf = (unsigned char*)malloc(encLength);
	unsigned char* outputBuf = (unsigned char*)malloc(encLength);
	memset(inputBuf, 0x00, encLength);
	fread(inputBuf, length, 1, b);

	unsigned char key[KEY_LENGTH] = { 0x23, 0x4D, 0x61, 0x73, 0x74, 0x65, 0x72, 0x2D, 0x49, 0x53, 0x4D, 0x32, 0x30, 0x31, 0x37, 0x23 };

	AES_KEY aesKey;
	unsigned char IV[KEY_LENGTH];
	AES_set_encrypt_key(key, 128, &aesKey);
	memset(&IV, 0x01, sizeof(IV));
	AES_cbc_encrypt(inputBuf, outputBuf, encLength, &aesKey, IV, AES_ENCRYPT);

	fwrite(&length, sizeof(length), 1, bEnc);
	fwrite(outputBuf, encLength, 1, bEnc);

	free(inputBuf);
	free(outputBuf);
	fclose(b);
	fclose(bEnc);


	// 2
	FILE* bHash;

	fopen_s(&b, "brancusi.txt", "rb");
	fopen_s(&bHash, "b-SHA256.txt", "w");

	unsigned char* fileBuf = NULL;
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256_CTX ctx;
	SHA256_Init(&ctx);

	fseek(b, 0, SEEK_END);
	int fileLength = ftell(b);
	fseek(b, 0, SEEK_SET);

	fileBuf = (unsigned char*)malloc(fileLength);
	fread(fileBuf, fileLength, 1, b);
	unsigned char* tmpBuffer = fileBuf;

	while (fileLength > 0) {
		if (fileLength > MESSAGE_CHUNK) {
			SHA256_Update(&ctx, tmpBuffer, MESSAGE_CHUNK);
		}
		else {
			SHA256_Update(&ctx, tmpBuffer, fileLength);
		}
		fileLength -= MESSAGE_CHUNK;
		tmpBuffer += MESSAGE_CHUNK;
	}

	SHA256_Final(hash, &ctx);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		fprintf(bHash, "%02X ", hash[i]);
	}


	return 0;


}