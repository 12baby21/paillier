#include <iostream>
#include <gmpxx.h>
#include <string>
#include <time.h>
#include "common.h"
#include "modPaillier.h"

using namespace std;

mpz_class q_inv_p, hp, hq;
mpz_class p, q, psquare, qsquare, g, n, nsquare, dip, dop, diq, doq;
mpz_class p_1, q_1;

// shadow
mpz_class lambda, mu, PrimeP, PrimeQ;

// 公钥加密私钥解密
int main(int argc, char *argv[])
{
    // 新的密钥生成方式
    mp_bitcnt_t a = 1024, b = 512;
    paillierSDU::GenKey(p, dip, dop, a, b);
    paillierSDU::GenKey(q, diq, doq, a, b);
    // gmp_printf("p\n%Zd\n", p);
    // gmp_printf("q\n%Zd\n", q);
    psquare = p * p;
    qsquare = q * q;
    // gmp_printf("psquare\n%Zd\n", psquare);
    // gmp_printf("qsquare\n%Zd\n", qsquare);
    p_1 = p - 1;
    q_1 = q - 1;
    n = p * q;
    g = n + 1;
    nsquare = n * n;

    // preComputation
    mpz_class g_1p2, g_1q2;
    mpz_class lp, lq;
    mpz_powm(g_1p2.get_mpz_t(), g.get_mpz_t(), p_1.get_mpz_t(), psquare.get_mpz_t());
    mpz_powm(g_1q2.get_mpz_t(), g.get_mpz_t(), q_1.get_mpz_t(), qsquare.get_mpz_t());
    // gmp_printf("g_1p2\n%Zd\n", g_1p2);
    // gmp_printf("g_1q2\n%Zd\n", g_1q2);
    hp = paillierSDU::L_Function(g_1p2, p) % psquare;
    hq = paillierSDU::L_Function(g_1q2, q) % qsquare;
    mpz_invert(hp.get_mpz_t(), hp.get_mpz_t(), p.get_mpz_t());
    mpz_invert(hq.get_mpz_t(), hq.get_mpz_t(), q.get_mpz_t());

    // Modified Paillier
    mpz_class m(5);
    gmp_printf("%Zd\n", m);
    mpz_class c;
    auto start_time = clock();
    paillierSDU::Encryption(c, m);
    paillierSDU::Decryption(m, c);
    auto end_time = clock();

    cout << "execution time = " << 1.0 * (end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;

    // result
    gmp_printf("%Zd\n", m);

    return 0;
}
