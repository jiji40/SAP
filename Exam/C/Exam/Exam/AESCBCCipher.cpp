#include "AESCBCCipher.h"
#include "openssl/aes.h"
#include "Cipher.cpp"
#include "string.h"

class AESCBCCipher :
    public Cipher {
private:
    AES_KEY aes_key;
    unsigned char ivec[16];
public:
    void init_cipher(
        unsigned char* user_key,
        unsigned short int bit_key_length,
        unsigned short array_key_offset,
        unsigned char* iv,
        unsigned short array_iv_offset,
        unsigned char mode) {

        memcpy(&ivec, iv + array_iv_offset, sizeof(ivec));
        if (mode == MODE_DECRYPT) {
            AES_set_encrypt_key(user_key + array_key_offset, bit_key_length, &aes_key);
        }
        else {
            AES_set_decrypt_key(user_key + array_key_offset, bit_key_length, &aes_key);
        }
    }
};
