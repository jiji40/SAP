#include <stdio.h>
#include <malloc.h>
#include <openssl/md5.h>

#define MESSAGE_CHUNK 128 

// cl.exe /IC:\OpenSSL\include /FoMD5App C:\OpenSSL\lib\VC\libeay32MTd.lib ex30_MD5.cpp
// 		/I - add path to OpenSSL folder where the header files are stored 
//		/Fo - different name for the output exe file (MD5App.exe instead ex30_MD5.exe)

// Visual Studio setup for OpenSSL
//		1. Debugging: Command Arguments (e.g. D:\f1.txt)
//		2. C/C++: General: Additional Include Directories (e.g. C:\OpenSSL\include)
//		3. Linker: Command Line: Additional options (e.g. C:\OpenSSL\lib\VC\libeay32MTd.lib)

int main(int argc, char**argv)
{
	if (argc == 2) {

		FILE* f = NULL;
		errno_t err;
		MD5_CTX ctx;

		unsigned char finalDigest[MD5_DIGEST_LENGTH];
		MD5_Init(&ctx);

		unsigned char* fileBuffer = NULL;

		err = fopen_s(&f, argv[1], "rb");
		if (err == 0) {
			fseek(f, 0, SEEK_END);
			int fileLen = ftell(f);
			fseek(f, 0, SEEK_SET);

			fileBuffer = (unsigned char*)malloc(fileLen);
			fread(fileBuffer, fileLen, 1, f);
			unsigned char* tmpBuffer = fileBuffer;

			while (fileLen > 0) {
				if (fileLen > MESSAGE_CHUNK) {
					MD5_Update(&ctx, tmpBuffer, MESSAGE_CHUNK);
				}
				else {
					MD5_Update(&ctx, tmpBuffer, fileLen);
				}
				fileLen -= MESSAGE_CHUNK;
				tmpBuffer += MESSAGE_CHUNK;
			}

			MD5_Final(finalDigest, &ctx);

			int count = 0;
			printf("\nMD5 = ");
			for (int i = 0; i<MD5_DIGEST_LENGTH; i++) {
				printf("%02X ", finalDigest[i]);
				printf(" ");
			}

			printf("\n");

			fclose(f);
		}

	}
	else {
		printf("\n Usage Mode: ProgMainMD5.exe fSrc.txt \n\n");
		return 1;
	}

	return 0;
}
