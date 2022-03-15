#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <gmpxx.h>

using namespace std;

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

// add
void EncryptAdd(mpz_ptr res, mpz_ptr c1, mpz_ptr c2, mpz_ptr n_2)
{
    mpz_mul(res, c1, c2);
    mpz_mod(res, res, n_2);
}

