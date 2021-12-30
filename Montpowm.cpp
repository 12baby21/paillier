#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>

void bn_expand(char* expanded, mpz_ptr A, int base)
{
    mpz_get_str(expanded, base, A);
}

void bn_MultMod(mpz_ptr res, mpz_ptr A, mpz_ptr B, mpz_ptr modulo)
{
    // res = A * B * 2^{-M} mod modulo

    // Get bit size
    mp_size_t n = mpz_sizeinbase(B, 2);
    char* ToBits = (char*)malloc(sizeof(char)*n);
    bn_expand(ToBits, B, 2);

    mpz_ptr tmp;
    mpz_init(tmp);
    int c0 = 0;

    for(int i = 0; i <= n; i++)
    {
        c0 = mpz_get_ui(res);
        mpz_mul_ui(tmp, A, ToBits[i]-'0');
        mpz_add(res, res, tmp);
        mpz_mul_ui(tmp, modulo, c0);
        mpz_add(res, res, tmp);
        mpn_rshift(res, res, n, )

    }
}

void bn_PowMod













void Montpowm(mpz_ptr res, mpz_ptr base, mpz_ptr e, mpz_ptr modulo)
{
    mpz_t base0;
    mpz_init(base0);

    
}
