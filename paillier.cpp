#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <iostream>

using namespace std;

// 11
void GenKey(mp_bitcnt_t bits, mpz_ptr n, mpz_ptr g, mpz_ptr lambda, mpz_ptr mu, mpz_ptr n_2)
{
    // Random State
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);

    // Generate Two big Primes
    mpz_t PrimeP, PrimeQ;
    mpz_init(PrimeP);
    mpz_init(PrimeQ);
    mpz_urandomb(PrimeP, grt, bits);
    mpz_urandomb(PrimeQ, grt, bits);
    mpz_setbit(PrimeP, bits);
    mpz_setbit(PrimeQ, bits);
    mpz_nextprime(PrimeP, PrimeP);
    mpz_nextprime(PrimeQ, PrimeQ);

    // Generate Public Key (n, g)
    // n = PrimeP * PrimeQ
    // g = n + 1, if PrimeP and PrimeQ have the same length
    mpz_mul(n, PrimeP, PrimeQ);
    mpz_add_ui(g, n, 1);

    // PrimeP = PrimeP-1
    // PrimeQ = PrimeQ-1
    mpz_sub_ui(PrimeP, PrimeP, 1);
    mpz_sub_ui(PrimeQ, PrimeQ, 1);

    // Generate Secret Key (lambda, mu)
    // lambda = (PrimeP-1) * (PrimeQ-1)
    // mu = lambda -1
    mpz_mul(lambda, PrimeP, PrimeQ);
    mpz_sub_ui(mu, lambda, 1);

    // n_2 = n^2
    mpz_mul(n_2, n, n);
}

void Encryption(mpz_ptr c, mpz_ptr m, mpz_ptr g, mpz_ptr n, mpz_ptr n_2)
{
    // Random Key
    mpz_t r;
    mpz_init_set_ui(r, 2);

    // gm = g^m mod n^2
    // rn = r^n mod n^2
    mpz_t gm;
    mpz_t rn;
    mpz_init(gm);
    mpz_init(rn);
    mpz_powm(gm, g, m, n_2);
    mpz_powm(rn, r, n, n_2);
    // c = g^m * r^n mod n^2 = (g^m mod n^2) * (r^n mod n^2) mod n^2
    // c= gm * rn mod n^2
    mpz_mul(c, gm, rn);
    mpz_mod(c, c, n_2);
}

void Decryption(mpz_ptr res, mpz_ptr c, mpz_ptr lambda, mpz_ptr n, mpz_ptr n_2)
{
    mpz_t l;
    mpz_init(l);
    mpz_powm(l, c, lambda, n_2);
    mpz_sub_ui(l, l, 1);
    mpz_div(l, l, n);

    mpz_t lambdainvert;
    mpz_init(lambdainvert);
    mpz_invert(lambdainvert, lambda, n);
    mpz_mul(l, l, lambdainvert);
    mpz_mod(res, l, n);
}

// Quantized to int16
void Encode(int &res, float raw_num, float Rmax, float Rmin)
{
    // S = (Rmax - Rmin) / (Qmax - Qmin)
    // Z = 0
    // Q = R / S + Z
    int new_raw_num;
    new_raw_num = raw_num > 3 ? 3 : raw_num;
    new_raw_num = raw_num < -3 ? -3 : raw_num;
    float S = (Rmax - Rmin) / 131072;
    float Z = 65536 - Rmax / S; // 0
    int Q = new_raw_num / S + Z;
    cout << raw_num << " is quantized to " << Q << endl;
    res = Q;
}

void Decode(float &res, int encoded_num, float Rmax, float Rmin)
{
    float S = (Rmax - Rmin) / 131072;
    float Z = 65536 - Rmax / S; // 0
    float R = (encoded_num - Z) * S;
    cout << encoded_num << " is restore to " << R << endl;
    res = R;
}

int main()
{
    // raw_num -> encoded_num -> encrypt -> decrypt -> decode

    // Encode a float number
    float raw_num = 3.44896;
    int encoded_num = 0;
    float decoded_num = 0;
    Encode(encoded_num, raw_num, 3, -3);

    // 编码后的明文
    mpz_t encoded_plain;
    mpz_init_set_si(encoded_plain, encoded_num);
    gmp_printf("encoded_plain = %Zd\n", encoded_plain);

    // 编码后的密文
    mpz_t encoded_cipher;
    mpz_init(encoded_cipher);

    // 解码后的密文
    mpz_t decoded_cipher;
    mpz_init(decoded_cipher);

    // 解码后的明文
    mpz_t decoded_plain;
    mpz_init(decoded_plain);

    // Paillier procedure
    mpz_t n, g, lambda, mu, n_2;
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mu);
    mpz_init(n_2);
    mp_bitcnt_t bits = 1024;
    GenKey(bits, n, g, lambda, mu, n_2);
    Encryption(encoded_cipher, encoded_plain, g, n, n_2);
    gmp_printf("encoded_cipher = %Zd\n", encoded_cipher);
    Decryption(decoded_plain, encoded_cipher, lambda, n, n_2);
    gmp_printf("decoded_plain = %Zd\n", decoded_plain);

    // mpz转化为int
    encoded_num = mpz_get_si(decoded_plain);
    cout << "encoded number after decryption is " << encoded_num << endl;

    // Decode a number in int16 presentation
    Decode(decoded_num, encoded_num, 3, -3);
    cout << "the origin number is " << decoded_num << endl;

    // 释放内存
    mpz_clear(encoded_plain);
    mpz_clear(encoded_cipher);
    mpz_clear(decoded_plain);
    mpz_clear(decoded_cipher);
    mpz_clear(n);
    mpz_clear(g);
    mpz_clear(lambda);
    mpz_clear(mu);
    mpz_clear(n_2);

    return 0;
}
