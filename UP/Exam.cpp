#include <iostream>
#include <openssl/sha.h>
#include <openssl/aes.h>

#define CHUNK_LEN 160 

int main()
{
	FILE* wordlist;
	fopen_s(&wordlist, "wordlist.txt", "rb");

	// 1
	unsigned char hash[SHA256_DIGEST_LENGTH];

	SHA256_CTX ctx;
	SHA256_Init(&ctx);

	fseek(wordlist, 0, SEEK_END);
	int len = ftell(wordlist);
	fseek(wordlist, 0, SEEK_SET);

	unsigned char* buf = (unsigned char*)malloc(len);
	fread(buf, len, 1, wordlist);
	unsigned char* tmpBuf = buf;

	while (len > 0) {
		if (len > CHUNK_LEN) {
			SHA256_Update(&ctx, tmpBuf, CHUNK_LEN);
		}
		else {
			SHA256_Update(&ctx, tmpBuf, len);
		}
		len -= CHUNK_LEN;
		tmpBuf += CHUNK_LEN;
	}

	SHA256_Final(hash, &ctx);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		printf("%02h", hash[i]);
	}



	// 2

	FILE* aeskeyFile;
	fopen_s(&aeskeyFile, "aeskey.txt", "rb");
	unsigned char key[16];
	unsigned char iv[16];
	fread(key, 8, 16, aeskeyFile);
	fread(iv, 8, 16, aeskeyFile);
	
	long int encLen = SHA256_DIGEST_LENGTH;
	unsigned char* encHash = (unsigned char*)malloc(encLen);

	AES_KEY aesKey;
	AES_set_encrypt_key(key, 128, &aesKey);
	AES_cbc_encrypt(hash, encHash, encLen, &aesKey, iv, AES_ENCRYPT);


	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
		printf("%02h", encHash[i]);
	}


}
