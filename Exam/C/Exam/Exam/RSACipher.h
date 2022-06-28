#pragma once
#include "Cipher.h"
#include "openssl/rsa.h"

class RSACipher :
    public Cipher
{
private:
    RSA* rsa_key_pair;
public:
    RSACipher();
    ~RSACipher() {
        delete(rsa_key_pair);
    }

    int generate_key_pair(int bit_modulus_length, unsigned long public_exp) {
        rsa_key_pair = RSA_generate_key(bit_modulus_length, public_exp, 0, 0);

        if (rsa_key_pair == NULL) return -1;
        return 1;
    }

    unsigned char* public_encrypt(
        unsigned char* in_buffer,
        unsigned short byte_in_length,
        unsigned short in_offset,
        unsigned short* byte_out_length) {

        int size = RSA_size(rsa_key_pair);

        unsigned char* enc = (unsigned char*)malloc(size);

        int pos;
        for (pos = in_offset; pos < byte_in_length; pos += size) {
            RSA_public_encrypt(size, in_buffer, enc, rsa_key_pair, RSA_NO_PADDING);
        }

        return enc;
    }
};

