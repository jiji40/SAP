#include <stdio.h>
#include <malloc.h>
#include <openssl/applink.c>
#include <openssl/pem.h>
#include <openssl/rsa.h>


int main()
{
	RSA *rsaKP = NULL; // pointer to RSA OpenSSL structure

	rsaKP = RSA_new(); // allcation of RSA structure
	rsaKP = RSA_generate_key(1024, 65535, NULL, NULL); 	// generates the RSA key pair
	
	RSA_check_key(rsaKP); // check if the RSA key pair was generated fine

	FILE *fpPriv = NULL;
	fopen_s(&fpPriv, "privKeyFile.pem","w+");
	PEM_write_RSAPrivateKey(fpPriv, rsaKP, NULL, NULL, 0 ,0, NULL); // save the RSA private key outside in file
	fclose(fpPriv);

	FILE *fpPub = NULL;
	fopen_s(&fpPub, "pubKeyFile.pem","w+");
	PEM_write_RSAPublicKey(fpPub, rsaKP);	// save the RSA public key outside in file
	fclose(fpPub);

	RSA_free(rsaKP);

	printf("\n The RSA key pair generated! \n");

	return 0;
}