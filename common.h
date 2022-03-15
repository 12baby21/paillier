#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <gmpxx.h>

void GenKey(mp_bitcnt_t bits, mpz_ptr n, mpz_ptr g, mpz_ptr lambda, mpz_ptr mu, mpz_ptr n_2);
void Encryption(mpz_ptr c, mpz_ptr m, mpz_ptr g, mpz_ptr n, mpz_ptr n_2);
void Decryption(mpz_ptr res, mpz_ptr c, mpz_ptr lambda, mpz_ptr n, mpz_ptr n_2);
void EncryptAdd(mpz_ptr res, mpz_ptr c1, mpz_ptr c2, mpz_ptr n_2);
void EncryptMul(mpz_ptr res, mpz_ptr c, unsigned m, mpz_ptr n_2);
void Encode(mpz_ptr res, mpz_ptr n, float scalar, int scale = 1e6);
void Decode(float &res, mpz_ptr n, mpz_ptr cipher, int scale_factor = 1e6);

#endif