#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <openssl/applink.c>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/md5.h>

int main(int argc, char** argv)
{
	if(argc == 3) {
		FILE* fsrc = NULL;
		FILE* fsig = NULL;
		errno_t err;
    	MD5_CTX ctx;

		// Stage #1 Compute the message digest for the restored plaintext
    	unsigned char finalDigest[MD5_DIGEST_LENGTH]; // the computed message digest
		unsigned char* fileBuffer = NULL;
		MD5_Init(&ctx);

		err = fopen_s(&fsrc, argv[1], "rb");
		fseek(fsrc, 0, SEEK_END);
		int fileLen = ftell(fsrc);
		fseek(fsrc, 0, SEEK_SET);

		fileBuffer = (unsigned char*)malloc(fileLen); 
		fread(fileBuffer, fileLen, 1, fsrc);
		unsigned char* tmpBuffer = fileBuffer;

		while (fileLen > 0) {
			if (fileLen > MD5_DIGEST_LENGTH) {
				MD5_Update(&ctx, tmpBuffer, MD5_DIGEST_LENGTH);
			}
			else {
				MD5_Update(&ctx, tmpBuffer, fileLen);
			}
			fileLen -= MD5_DIGEST_LENGTH;
			tmpBuffer += MD5_DIGEST_LENGTH;
		}

		MD5_Final(finalDigest, &ctx);
		
    	for( int i=0; i<MD5_DIGEST_LENGTH; i++) 
        	printf( "%2X ", finalDigest[i] );   
		printf("\n");

		fclose(fsrc);

		// Stage #2 Decrypt the content of e-signature and compare it with the message digest resulted from Stage #1
		err = fopen_s(&fsig, argv[2], "rb");
			
		RSA* apub;
		FILE* f;
		int ret;
		unsigned char* buf = NULL;
		unsigned char* last_data = NULL;

		apub = RSA_new();
	
		f = fopen("pubKeyFile.pem","r");
		apub = PEM_read_RSAPublicKey(f, NULL, NULL, NULL); // public key is loaded into RSA key pair structure
		fclose(f);

		buf = (unsigned char *) malloc(RSA_size(apub));

		fread(buf, RSA_size(apub), 1, fsig); // reads the e-signature from the file (a single block)
			
		last_data = (unsigned char *) malloc(16); // buffer for the restored message digest

		RSA_public_decrypt(RSA_size(apub), buf, last_data, apub, RSA_PKCS1_PADDING); // decryption performed by using PUBLIC key

		fclose(fsig);
			
		if ( memcmp(last_data, finalDigest, 16) == 0 ) // comparision between the computed and restored messages digests
			printf("\n Signature OK!\n");
		else
			printf("\n Signature is wrong!\n");
			
		free(last_data);
		free(buf);

		RSA_free(apub);	
			
    } 
	else {
		printf("\n Usage mode: OpenSSLProj.exe fSrc.txt eSignFsrc.txt");
		return 1;
    }

    return 0;
}