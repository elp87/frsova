#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <time.h>
#include <data.h>
#include <LibSha1.h>


int main(void)
{    
    char string[80];
    Sha1Context context;
    SHA1_HASH hash;
    uint16_t i;
    unsigned int bits[8];
    uint8_t byte;

    printf("Введите сообщение: ");
    scanf("%s", string);

    // Хэшируем
    Sha1Initialise(&context);
    Sha1Update(&context, string, (uint32_t)strlen(string) );
    Sha1Finalise(&context, &hash);

    // Выводим результат хэш-функции
    for( i=0; i<sizeof(hash); i++ )
    {
        printf( "%2.2x ", hash.bytes[i] );
    }
    printf("\n");

    byte = hash.bytes[0];

    unsigned char mask = 1; // Битовая маска
    for (i = 0; i < 8; i++)
    {
        bits[i] = (byte & (mask << i)) != 0;
    }

    struct fsh_data_t data;

    mpz_t p, q;
    mpz_t p_min, p_max;
    mpz_t two_mpz;
    mpz_init(p);
    mpz_init(q);
    mpz_init(p_min);
    mpz_init(p_max);
    mpz_init_set_d(two_mpz, 2);

    mpz_pow_ui(p_min, two_mpz, 128);
    mpz_pow_ui(p_max, two_mpz, 256);
    gmp_randstate_t randstate;
    gmp_randinit_default(randstate);

    time_t t1 = time(NULL);
    gmp_randseed_ui(randstate, t1);

    mpz_urandomm(p, randstate, p_max);
    mpz_add(p, p, p_min);
    mpz_nextprime(p, p);
    gmp_printf("Найдено простое p - %Zd\n", p);

    do
    {
        mpz_urandomm(q, randstate, p_max);
        mpz_add(q, q, p_min);
        mpz_nextprime(q, q);
    }
    while(mpz_cmp(p,q) == 0); /* Повторяем поиск, если p == q */
    gmp_printf("Найдено простое q - %Zd\n", q);

    fsh_data_init(&data, p , q);

    // Вычисляем n = p * q
    fsh_data_calc_n(&data);
    gmp_printf("n = %Zd\n\n", data.n);

    // Выбираем случайные s
    fsh_data_calc_s(&data, data.n, randstate);

    // Вычисляем vj = sj^(-2) mod n
    fsh_data_calc_v(&data);

    ////////////// Signature generation //////////////
    // Выбираем случайное r
    fsh_data_calc_r(&data, data.n, randstate);
    gmp_printf("r = %Zd\n", data.r);

    // Вычисляем u = r^2 mod(n)
    fsh_data_calc_u(&data);
    gmp_printf("u = %Zd\n", data.u);

    return 1;
}
