#include <iostream>
#include <gmpxx.h>
#include "common.h"
using namespace std;

extern mpz_class q_inv_p, dip, dop, diq, doq;
extern mpz_class p, q, psquare, qsquare, g, n, nsquare;
extern mpz_class hp, hq;

namespace paillierSDU
{
    void GenKey(mpz_class &p, mpz_class &dip, mpz_class &dop, mp_bitcnt_t a, mp_bitcnt_t b)
    {
        dip = GenRandomPrime(b);

    step2:
        dop = GenRandomNumber(a - b);
        // set even
        mpz_clrbit(dop.get_mpz_t(), 0);
        // cout << mpz_sizeinbase(dop.get_mpz_t(), 2) << endl;
        // gmp_printf("%Zd\n", dop);

    step3:
        p = dip * dop + 1;

        if (MillerRabin_Prime(p) == false)
        {
            size_t l_dop = mpz_sizeinbase(dop.get_mpz_t(), 2);
            if (l_dop == a - b)
            {
                dop = dop + 2;
                goto step3;
            }
            else
            {
                goto step2;
            }
        }
        size_t l_p = mpz_sizeinbase(p.get_mpz_t(), 2);
        if (l_p != a)
            goto step2;
    }

    void Encryption(mpz_class &C, mpz_class &M)
    {
        mpz_class gm, rn, r;
        r = GenRandomPrime(50);
        mpz_powm(gm.get_mpz_t(), g.get_mpz_t(), M.get_mpz_t(), nsquare.get_mpz_t());
        mpz_powm(rn.get_mpz_t(), r.get_mpz_t(), n.get_mpz_t(), nsquare.get_mpz_t());
        C = (gm * rn) % nsquare;
    }

    void Decryption(mpz_class &M, mpz_class &C)
    {
        mpz_class phi_p, M_p, phi_q, M_q;

        mpz_powm(phi_p.get_mpz_t(), C.get_mpz_t(), dip.get_mpz_t(), psquare.get_mpz_t());
        mpz_class x_dop;
        mpz_powm(x_dop.get_mpz_t(), phi_p.get_mpz_t(), dop.get_mpz_t(), psquare.get_mpz_t());
        M_p = (((x_dop - 1) / p) * hp) % p;

        mpz_powm(phi_q.get_mpz_t(), C.get_mpz_t(), diq.get_mpz_t(), qsquare.get_mpz_t());
        mpz_class x_doq;
        mpz_powm(x_doq.get_mpz_t(), phi_q.get_mpz_t(), doq.get_mpz_t(), qsquare.get_mpz_t());
        M_q = (((x_doq - 1) / q) * hq) % q;

        mpz_class h;
        h = q_inv_p * (M_p - M_q) % p;

        M = M_q + h * q;
    }

    mpz_class L_Function(mpz_class &x, mpz_class &p)
    {
        mpz_class res = (x - 1) / p;
        return res;
    }
};