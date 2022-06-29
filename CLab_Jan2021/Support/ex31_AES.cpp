
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <openssl/aes.h>

int main(int argc, char** argv)
{
	if(argc == 5) {
		FILE* fSrc = NULL, *fDst = NULL;
		errno_t err;
		char opt[3];
		char mode[7];
		strcpy(opt, argv[1]);
		strcpy(mode, argv[2]);

		AES_KEY akey;
		unsigned char* inBuf = NULL;
		unsigned char* outBuf;
		unsigned char ivec[16]; // Initialization Vector
		// the Key - could be a message digest
		// key length is 16 bytes (128 bits)
		unsigned char userSymmetricKey[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
		unsigned char wrongSymmetricKey[16] = { 0x11, 0x11, 0xf2, 0xf3, 0xc4, 0x55, 0xa6, 0xa7, 0xa0, 0xa1, 0x92, 0x93, 0x94, 0x95, 0x56, 0x77 };

		if(strcmp(opt, "-e") == 0) {
			fopen_s(&fSrc, argv[3], "rb"); // to be read; to be enc/dec; plaintext file
			fopen_s(&fDst, argv[4], "wb"); // output; ciphertext file
			fseek(fSrc, 0, SEEK_END);
			long int inLen = ftell(fSrc);
			fseek(fSrc, 0, SEEK_SET);
			long int outLen = 0;
			if((inLen % 16) == 0)
				outLen = inLen;
			else
				outLen = ((inLen/16)*16) + 16; // adjusted input length according to number of blocks needed by AES algo
			
			inBuf = (unsigned char*)malloc(outLen);
			outBuf = (unsigned char*)malloc(outLen);
			memset(inBuf, 0x00, outLen);
			fread(inBuf, inLen, 1, fSrc); // the entire content of the plaintext file is added into inBuf

			AES_set_encrypt_key(userSymmetricKey, 128, &akey); // set the key into internal OpenSSL structure used later in other OpenSSL API calls
			if(strcmp(mode, "-ecb") == 0) {
				for(int i = 0; i < (outLen/16); i++)
					AES_encrypt(&(inBuf[i*16]), &(outBuf[i*16]), &akey); // performs the encryption for AES-ECB for each data block (16 bytes each block)
			} else {
				memset(&ivec, 0x01, sizeof(ivec)); // ivec = 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01 0x01
				AES_cbc_encrypt(inBuf, outBuf, outLen, &akey, ivec, AES_ENCRYPT); // performs the encryption for AES-CBC for entire content
			}
			
			fwrite(&inLen, sizeof(inLen), 1, fDst); // write the size of the plaintext in the output/encrypted file
			fwrite(outBuf, outLen, 1, fDst); // write the ciphertext (byte-by-byte) after the size of the plaintext file
			free(outBuf);
			free(inBuf);
			fclose(fDst);
			fclose(fSrc);
		} else {
			fopen_s(&fSrc, argv[3], "rb"); // ciphertext file
			fopen_s(&fDst, argv[4], "wb"); // restored plaintext
			fseek(fSrc, 0, SEEK_END);
			
			long int inLen = ftell(fSrc) - 4; // -4 is for the bytes occupied by the length of the plaintext (see line 51)
			fseek(fSrc, 0, SEEK_SET);
			long int outLen = 0;		// the length of the plaintext file
			fread(&outLen, sizeof(outLen), 1, fSrc);
			
			inBuf = (unsigned char*)malloc(inLen); // entire ciphertext is allocated and read from the ciphertext input file
			outBuf = (unsigned char*)malloc(inLen); // buffer for the restored plaintext
			memset(inBuf, 0x00, inLen);
			fread(inBuf, inLen, 1, fSrc);

			// check decryption by using wrongSymmetricKey instead 
			AES_set_decrypt_key(userSymmetricKey, 128, &akey); // load the decription key into AES_KEY structure
			if(strcmp(mode, "-ecb") == 0) {
				for(int i = 0; i < (inLen/16); i++)
					AES_decrypt(&(inBuf[i*16]), &(outBuf[i*16]), &akey); // performs a decryption operation at block level (16 bytes long)
			} else {
				// check AESA-CBC decryption with wrong IV; only first restored block is affected by this change
				memset(&ivec, 0x01, sizeof(ivec));
				AES_cbc_encrypt(inBuf, outBuf, inLen, &akey, ivec, AES_DECRYPT); // same name like in the enc case; performs decription of the entire ciphertext
			}
			
			fwrite(outBuf, outLen, 1, fDst); // write into the destionation/restored file; length of the restored file is outLen (read from the first 4 bytes in ciphertext file)
			free(outBuf);
			free(inBuf);
			fclose(fDst);
			fclose(fSrc);
		}
	} else {
		printf("\n Usage Mode: OpenSSLProj.exe -e -cbc fSrc.txt fDst.txt");
		printf("\n Usage Mode: OpenSSLProj.exe -d -ecb fSrc.txt fDst.txt");
		return 1;
	}
	printf("\n Process done.");
	return 0;
}
