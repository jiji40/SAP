#include "Cipher.h"

class Cipher {
public:
	static unsigned char ALGO_AES_CBC;
	static unsigned char ALGO_RSA_PKCS1;
	static unsigned char MODE_ENCRYPT;
	static unsigned char MODE_DECRYPT;

	static Cipher create_instance(unsigned char algorithm) {
		return Cipher(algorithm);
	}

private:
	unsigned char algorithm;

	Cipher(unsigned char algorithm) {
		this->algorithm = algorithm;
	}
};

unsigned char Cipher::ALGO_AES_CBC = 'A';
unsigned char Cipher::ALGO_RSA_PKCS1 = 'R';
unsigned char Cipher::MODE_ENCRYPT = 'E';
unsigned char Cipher::MODE_DECRYPT = 'D';