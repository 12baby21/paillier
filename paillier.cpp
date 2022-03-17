#include <stdlib.h>
#include <stdio.h>
#include <gmpxx.h>
#include <iostream>
#include <string>
#include "common.h"
using namespace std;

int main(int argc, char* argv[])
{
    // raw_num -> encoded_num -> encrypt -> decrypt -> decode

    // Encode a float number
    float op1 = -5.0f;
    float op2 = -2.24677f;
    float scalar = -7.111111111f;
    
    if(argc > 1) 
    {
        op1 = stof(argv[1]);
        op2 = stof(argv[2]);
        scalar = stof(argv[3]);
    }
    mpz_t encoded_op1;
    mpz_t encoded_op2;
    mpz_t encoded_scalar;
    mpz_init(encoded_op1);
    mpz_init(encoded_op2);
    mpz_init(encoded_scalar);
    float decoded_op1;
    float decoded_op2;

    // Paillier procedure
    mpz_t n, g, lambda, mu, n_2;
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mu);
    mpz_init(n_2);
    mp_bitcnt_t bits = 1024;
    GenKey(bits, n, g, lambda, mu, n_2);

    Encode(encoded_op1, n, op1, 1e6);
    Encode(encoded_op2, n, op2, 1e6);
    Encode(encoded_scalar, n, scalar, 1e6);
    // gmp_printf("encoded_op1 = %Zd\n", encoded_op1);
    // gmp_printf("encoded_op2 = %Zd\n", encoded_op2);
    // gmp_printf("encoded_scalar = %Zd\n", encoded_scalar);
    
    // Encryption
    mpz_t cipher_op1;
    mpz_t cipher_op2;
    mpz_init(cipher_op1);
    mpz_init(cipher_op2);
    Encryption(cipher_op1, encoded_op1, g, n, n_2);
    Encryption(cipher_op2, encoded_op2, g, n, n_2);

    // add
    mpz_t add_res;
    mpz_init(add_res);
    EncryptAdd(add_res, cipher_op1, cipher_op2, n_2);

    // mul
    mpz_t mul_res;
    mpz_init(mul_res);
    // gmp_printf("%Zd\n", encoded_op2);
    EncryptMul(mul_res, cipher_op1, encoded_scalar, n, n_2);
    // gmp_printf("mul_res = %Zd\n", mul_res);


    // Decrypion
    mpz_t my_res_add;
    mpz_init(my_res_add);

    Decryption(my_res_add, add_res, lambda, n, n_2);
    float ret1 = 0;
    Decode(ret1, n, my_res_add, false, 1e6);
    printf("(%f) + (%f) = %f\n", op1, op2, ret1);

    mpz_t my_res_mul;
    mpz_init(my_res_mul);
    Decryption(my_res_mul, mul_res, lambda, n, n_2);
    // gmp_printf("my_res_mul = %Zd\n", my_res_mul);
    float ret2 = 0;
    Decode(ret2, n, my_res_mul, true, 1e6);
    printf("(%f) * (%f) = %f\n", op1, scalar, ret2);

    return 0;
}
