#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <openssl/applink.c>
#include <openssl/pem.h>
#include <openssl/rsa.h>

int main(int argc, char** argv)
{
	if(argc == 4) {
		FILE* fsrc = NULL;
		FILE* fdst = NULL;
		FILE* frst = NULL;
		errno_t err;
    	
		err = fopen_s(&fsrc, argv[1], "rb");
		fseek(fsrc, 0, SEEK_END);
		int fileLen = ftell(fsrc);
		fseek(fsrc, 0, SEEK_SET);
		
		
		RSA* apub;
		RSA* apriv;
		FILE* f;
		
		unsigned char* e_data = NULL;
		unsigned char* last_data = NULL;

		apriv = RSA_new();
		apub = RSA_new();

		// RSA encryption
		f = fopen("pubKeyFile.pem","r");
		apub = PEM_read_RSAPublicKey(f, NULL, NULL, NULL); // loads the public key (components) into RSA internal structure
		fclose(f);

		err = fopen_s(&fdst, argv[2], "wb");
		
		unsigned char* fsrcbuf = (unsigned char*)malloc(RSA_size(apub) + 1);
		fsrcbuf[RSA_size(apub)] = 0x00;
		e_data = (unsigned char *) malloc(RSA_size(apub));
		if (fileLen != RSA_size(apub)) {
			while(fread_s(fsrcbuf, RSA_size(apub), sizeof(unsigned char), RSA_size(apub), fsrc) == RSA_size(apub)) { 
				RSA_public_encrypt(RSA_size(apub), fsrcbuf, e_data, apub, RSA_NO_PADDING); // encryption for a complete input data block by using the public key; no padding is required
				fwrite(e_data, sizeof(unsigned char), RSA_size(apub), fdst); // write the result of encryption (encrypted data block) into enc file
			}
		} 
		else {
			fread_s(fsrcbuf, RSA_size(apub), sizeof(unsigned char), RSA_size(apub), fsrc);
		}

		RSA_public_encrypt(fileLen % RSA_size(apub), fsrcbuf, e_data, apub, RSA_PKCS1_PADDING); // encryption for the incomplete last input data block; the completion is made by the padding at content level
		fwrite(e_data, sizeof(unsigned char), RSA_size(apub), fdst); // write the result of last last block encryption into enc file

		// RSA decryption
		f = fopen("privKeyFile.pem","r");
		apriv = PEM_read_RSAPrivateKey(f, NULL, NULL, NULL); // loads the private key (components) into RSA structure
		fclose(f);

		free(e_data);
		e_data = (unsigned char *) malloc(RSA_size(apub));
		last_data = (unsigned char *) malloc(RSA_size(apub));
		fclose(fdst);

		fopen_s(&fdst, argv[2], "rb");
		fseek(fdst, 0, SEEK_END);
		int fileLen2 = ftell(fdst);
		fseek(fdst, 0, SEEK_SET);

		int maxChunks = fileLen2 / RSA_size(apub); // number of encrypted data blocks
		int currentChunk = 1; 

		err = fopen_s(&frst, argv[3], "wb");

		if (fileLen2 != RSA_size(apub)) {
			while(fread_s(e_data, RSA_size(apub), sizeof(unsigned char), RSA_size(apub), fdst) == RSA_size(apub)) { // read one single encrypted complete data block
				if(currentChunk != maxChunks) {
					RSA_private_decrypt(RSA_size(apub), e_data, last_data, apriv, RSA_NO_PADDING); // decryption of the complete encrypted data block; no padding required
					fwrite(last_data, sizeof(unsigned char), RSA_size(apub), frst);
					currentChunk++; 
				}
			}
		}
		else {
			fread_s(e_data, RSA_size(apub), sizeof(unsigned char), RSA_size(apub), fdst);
		}

		RSA_private_decrypt(RSA_size(apub), e_data, last_data, apriv, RSA_PKCS1_PADDING); // decryption of the last (incomplete) encrypted data block; padding required
		fwrite(last_data, sizeof(unsigned char),fileLen % RSA_size(apub), frst); // fileLen - length of the original/initial plaintext	
		// fwrite(last_data, sizeof(unsigned char), RSA_size(apub), frst); // the clear padding will be added to the end of the restored file
		
		free(last_data);
		free(e_data);
		free(fsrcbuf);

		RSA_free(apub);
		RSA_free(apriv);

		fseek(frst, 0, SEEK_END);
		printf("Nr. of bytes on the decrypted file: %d \n", ftell(frst));
		fseek(fsrc, 0, SEEK_END);										
		printf("Nr. of bytes on the input file: %d", ftell(fsrc));		
		
		fclose(fsrc);
		fclose(frst);
		fclose(fdst);

    } else {
		printf("\n Usage mode: OpenSSLProj.exe f1.txt encryptf1.txt f9.txt");
		return 1;
    }

    return 0;
}