#include <stdlib.h>
#include <stdio.h>
#include <gmpxx.h>
#include <iostream>
#include <string>
#include "common.h"
using namespace std;
// 公钥加密私钥解密
int main(int argc, char *argv[])
{
    // Paillier procedure
    mpz_t n, g, lambda, mu, n_2, R;
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mu);
    mpz_init(n_2);
    mpz_init(R);
    mp_bitcnt_t bits = 1024;
    GenKey(bits, n, g, lambda, mu, n_2);
    GenRandom(R, 1024);

    // operands
    float op = -0.176348f;
    float res;
    mpz_t m, c, maskC;
    mpz_init(m);
    mpz_init(c);
    mpz_init(maskC);
    Encode(m, n, op, 1e6);
    Encryption(c, m, g, n, n_2);
    Encryption(maskC, R, g, n, n_2);
    EncryptAdd(maskC, c, maskC, n_2);
    Decryption(m, maskC, lambda, n, n_2);
    mpz_sub(m, m, R);
    Decode(res, n, m,  false, 1e6);
    gmp_printf("res = %f\n", res);


    return 0;
}
