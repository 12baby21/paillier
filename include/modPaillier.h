#ifndef _MODPAILLIER_H_
#define _MODPAILLIER_H_
#include <gmpxx.h>
#include "common.h"

namespace paillierSDU
{
    void GenKey(mpz_class &p, mpz_class &dip, mpz_class &dop, mp_bitcnt_t a, mp_bitcnt_t b);
    void Encryption(mpz_class &C, mpz_class &M);
    void Decryption(mpz_class &M, mpz_class &C);
    mpz_class L_Function(mpz_class &x, mpz_class &p);
}
#endif