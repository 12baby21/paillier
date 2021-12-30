#include <gmp.h>
#include <stdio.h>
#include <time.h>


void Quickmodm(mpz_ptr res, mpz_ptr base, mpz_ptr e, mpz_ptr modulo)
{
    clock_t time_start = clock();
    mp_size_t n = mpz_sizeinbase(e, 2);
    char e_char[n];
    // 0 -> 1 high -> low
    mpz_get_str(e_char, 2, e);
    gmp_printf("The binary (%d byte(s)) representation of %Zd is: ", n, e);
    for(int i  = 0; i < 12; i++) {
        gmp_printf("%c", e_char[i]);
    }
    printf("\n");

    // Save base^0 to base^{2^n}
    // Can be calculated during some overlapped time
    mpz_t pows[11];
    mpz_init_set(pows[0], base);
    gmp_printf("pows[0] = %Zd\n", pows[0]);
    mpz_t tmp;
    mpz_init(tmp);
    for(int i = 1; i <= 10 ; i++)
    {
        mpz_init(pows[i]);
        mpz_mul(tmp, pows[i-1], pows[i-1]);
        mpz_mod(tmp, tmp, modulo);
        mpz_init_set(pows[i], tmp);
        gmp_printf("pows[%d] = %Zd\n", i, pows[i]); 
    }
    mpz_clear(tmp);

    //  powm
    for(int i = 0; i <= 10; i++)
    {
        if(e_char[i] == '1')
        {
            mpz_mul(res, res, pows[10-i]);
            mpz_mod(res, res, modulo);
        }
    }
    
    clock_t time_end = clock();

    printf("Time elapsed %ldms\n", time_end-time_start);
}


int main()
{
    mpz_t res, base, e, modulo;
    mpz_init_set_ui(res, 1);
    mpz_init_set_ui(base, 98765);
    mpz_init_set_ui(e, 1234);
    mpz_init_set_ui(modulo, 123557);

    Quickmodm(res, base, e, modulo);
    gmp_printf("The result of %Zd ^ %Zd mod %Zd is %Zd\n", base, e, modulo, res);


    mpz_clear(res);
    mpz_clear(base);
    mpz_clear(e);
    mpz_clear(modulo);
    
    return 0;
}