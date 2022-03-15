#include <stdlib.h>
#include <stdio.h>
#include <gmpxx.h>
#include <iostream>
#include "common.h"
using namespace std;




// Quantized to int16
void Encode(int &res, float raw_num, float Rmax, float Rmin)
{
    // S = (Rmax - Rmin) / (Qmax - Qmin)
    // Z = 0
    // Q = R / S + Z
    float new_raw_num;
    new_raw_num = raw_num > Rmax ? Rmax : raw_num;
    new_raw_num = raw_num < Rmin ? Rmin : raw_num;
    float S = (Rmax - Rmin) / 65535;
    float Z = 32767 - Rmax / S; // 0
    int Q = new_raw_num / S + Z;
    cout << raw_num << " is quantized to " << Q << endl;
    res = Q;
}

void Decode(float &res, int encoded_num, float Rmax, float Rmin)
{
    float S = (Rmax - Rmin) / 65535;
    float Z = 32767 - Rmax / S; // 0
    float R = (encoded_num - Z) * S;
    cout << encoded_num << " is restore to " << R << endl;
    res = R;
}

int main()
{
    // raw_num -> encoded_num -> encrypt -> decrypt -> decode

    // Encode a float number
    float raw_num1 = 0.135;
    float raw_num2 = -0.246;
    int encoded_num1 = 0;
    float decoded_num1 = 0;
    int encoded_num2 = 0;
    float decoded_num2 = 0;
    Encode(encoded_num1, raw_num1, 10, -10);
    Encode(encoded_num2, raw_num2, 10, -10);
    

    // 编码后的明文
    mpz_t encoded_plain1;
    mpz_init_set_si(encoded_plain1, encoded_num1);
    mpz_t encoded_plain2;
    mpz_init_set_si(encoded_plain2, encoded_num2);

    // 编码后的密文
    mpz_t encoded_cipher1;
    mpz_init(encoded_cipher1);
    mpz_t encoded_cipher2;
    mpz_init(encoded_cipher2);

    // 解码后的密文
    mpz_t decoded_cipher1;
    mpz_init(decoded_cipher1);
    mpz_t decoded_cipher2;
    mpz_init(decoded_cipher2);

    // 解码后的明文
    mpz_t decoded_plain1;
    mpz_init(decoded_plain1);
    mpz_t decoded_plain2;
    mpz_init(decoded_plain2);

    // Paillier procedure
    mpz_t n, g, lambda, mu, n_2;
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mu);
    mpz_init(n_2);
    mp_bitcnt_t bits = 1024;
    GenKey(bits, n, g, lambda, mu, n_2);
    Encryption(encoded_cipher1, encoded_plain1, g, n, n_2);
    Encryption(encoded_cipher2, encoded_plain2, g, n, n_2);

    // add
    mpz_t aaa;
    mpz_init(aaa);
    EncryptAdd(aaa, encoded_cipher1, encoded_cipher2, n_2);

    mpz_t bbb;
    mpz_init(bbb);
    Decryption(bbb, aaa, lambda, n, n_2);

    // mpz转化为int
    int encoded_res = mpz_get_si(bbb);
    cout << "encoded_res after decryption is " << encoded_res << endl;


    // Decode a number in int16 presentation
    float output;
    Decode(output, encoded_res, 10, -10);
    cout << raw_num1 << " + " << raw_num2 << " = " << output << endl;

    return 0;
}
