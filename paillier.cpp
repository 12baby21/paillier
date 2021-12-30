#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <gmp.h>

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


int main()
{
    // Randomly init 100 * 100 words
    
    // plaintext
    mpz_t m1;
    mpz_init_set_str(m1, "1356205320457610288745198967657644166379972189839804389074591563666634066646564410685955217825048626066190866536592405966964024022236587593447122392540033893121248948780525117822889230574978651418075403357439692743398250207060920929117606033490559159560987768768324823011579283223392964454439904542675637683985296529882973798752471233683249209762843835985174607047556306705224118165162905676610067022517682197138138621344578050034245933990790845007906416093198845798901781830868021761765904777531676765131379495584915533823288125255520904108500256867069512326595285549579378834222350197662163243932424184772115345", 0);
    //mpz_t m2;
    mpz_t m2;
    mpz_init_set_str(m2, "1356205320457610288745198967657644166379972189839804389074591563666634066646564410685955217825048626066190866536592405966964024022236587593447122392540033893121248948780525117822889230574978651418075403357439692743398250207060920929117606033490559159560987768768324823011579283223392964454439904542675637683985296529882973798752471233683249209762843835985174607047556306705224118165162905676610067022517682197138138621344578050034245933990790845007906416093198845798901781830868021761765904777531676765131379495584915533823288125255520904108500256867069512326595285549579378834222350197662163243932424184772115345", 0);
    //mpz_t m2;
    //mpz_init_set(m2, 1);

    // ciphertext
    mpz_t c1;
    mpz_init(c1);
    mpz_t c2;
    mpz_init(c2);

    // result
    mpz_t res;
    mpz_init(res);

    clock_t start = clock();
    
    // PubKey(n, g)
    // SecKey(lambda, mu)
    mpz_t n, g, lambda, mu, n_2;
    mpz_init(n);
    mpz_init(g);
    mpz_init(lambda);
    mpz_init(mu);
    mpz_init(n_2);
    mp_bitcnt_t bits = 1024;

    clock_t ge0 = clock();
    GenKey(bits, n, g, lambda, mu, n_2);
    clock_t ge1 = clock();
    
    clock_t en0 = clock();
    Encryption(c1, m1, g, n, n_2);
    Encryption(c2, m2, g, n, n_2);
    clock_t en1 = clock();
    //Encryption(c2, m2, g, n, n_2);

    clock_t cal0 = clock();
    // res = m1 + m2
    mpz_mul(res, c1, c2);
    clock_t cal1 = clock();


    clock_t de0 = clock();
    Decryption(res, res, lambda, n, n_2);
    clock_t de1 = clock();


    clock_t finish = clock();

    // Print Result
    gmp_printf("%Zd\n+\n%Zd\n=\n%Zd\n", m1, m2, res);
    printf("Key Generation took %ldus\n", (ge1 - ge0));
    printf("Encrytion took %ldus\n", (en1 - en0));
    printf("Addtioin took %ldus\n", (cal1 - cal0));
    printf("Decrytion took %ldus\n", de1 - de0);
    printf("Total took %ldus\n", finish - start);
    return 0;
}

