#include <iostream>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <gmpxx.h>

using namespace std;

/**
 * @brief 
 * 
 * @param N: the length of prime 
 */
void GenKey(mpz_ptr n, mpz_ptr g, mpz_ptr lambda, mpz_ptr mu, mp_bitcnt_t bits)
{
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);

    // generate two big primes p and q
    mpz_t p;
    mpz_init(p);
    // mpz_urandomb(mpz_t rop, gmp_randstate_t, mp_bitcnt_t n)
    // generate a random uniform integer in [0, 2^n-1], the result is saved in rop
    mpz_urandomb(p, grt, bits);
    mpz_nextprime(p, p);

    mpz_t q;
    mpz_init(q);
    // mpz_urandomb(mpz_t rop, gmp_randstate_t, mp_bitcnt_t n)
    // generate a random uniform integer in [0, 2^n-1], the result is saved in rop
    mpz_urandomb(q, grt, bits);
    mpz_nextprime(q, q);
    
    // Generate one = 1 for convenience
    mpz_t one;
    mpz_init(one);
    mpz_set_si(one, 1);

    // PubKey: n = p*q
    mpz_init(n);
    mpz_mul(n, p, q);
    // PubKey: g = n+1
    mpz_init(g);
    mpz_add(g, n, one);

    // p = p-1; q = q-1;
    mpz_sub(p, p, one);
    mpz_sub(q, q, one);

    // SecKey: lambda = lcm(p-1, q-1)
    mpz_init(lambda);
    mpz_lcm(lambda, p, q);
    // SecKey: mu = lambda - 1
    mpz_init(mu);
    mpz_sub(mu, mu, one);
}

void Encryption(mpz_ptr c, mpz_ptr m, mpz_ptr n, mpz_ptr g, mpz_ptr n_2)
{
    mpz_t e;
    mpz_init_set_ui(e, 65537);
    
    mpz_t r;
    mpz_init(r);
    mpz_invert(r, e, n);

    mpz_t gm, rn;
    mpz_init(gm);
    mpz_init(rn);
    mpz_powm(gm, g, m, n_2);
    mpz_powm(rn, r, n, n_2);

    mpz_mul(c, gm, rn);
    mpz_mod(c, c, n_2);
 
}

void Decryption(mpz_ptr m, mpz_ptr c, mpz_ptr lambda, mpz_ptr n_2, mpz_ptr mu, mpz_ptr n)
{
    mpz_t l;
    mpz_init(l);
    mpz_powm(l, c, lambda, n_2);
    mpz_mul(l, l, mu);
    mpz_mod(m, l, n);
}

