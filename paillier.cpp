#include <stdlib.h>
#include <stdio.h>
#include <gmpxx.h>
#include <iostream>
#include <string>
#include "common.h"
using namespace std;

int main(int argc, char *argv[])
{
    // raw_num -> encoded_num -> encrypt -> decrypt -> decode

    // Encode a float number
    float op1 = -0.520279f;
    float op2 = -2.24677f;
    float scalar = 3.732;

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


    mpz_t res;
    mpz_init(res);
    float fres = 0;
    encrypt_calGrad_weight(res, cipher_op1, encoded_scalar, lambda, n, n_2);
    Decryption(res, res, lambda, n, n_2);
    Decode(fres, n, res, true, 1e6);
    cout << fres << endl;

    


    return 0;
}
