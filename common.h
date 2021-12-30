#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <gmpxx.h>
111
void GenKey(mpz_ptr n, mpz_ptr g, mpz_ptr lambda, mpz_ptr mu, mp_bitcnt_t bits);
void Encryption(mpz_ptr c, mpz_ptr m, mpz_ptr n, mpz_ptr g, mpz_ptr n_2);
void Decryption(mpz_ptr m, mpz_ptr c, mpz_ptr lambda, mpz_ptr n_2, mpz_ptr mu, mpz_ptr n);


#endif