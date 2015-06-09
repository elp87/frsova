#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <time.h>
#include <data.h>
#include <string.h>
#include <LibSha1.h>

void sha1_to_bitarray(unsigned int *e[], SHA1_HASH hash)
{
    int byte_counter;
    int bit_counter;
    int hash_counter = 0;
    uint8_t byte;
    for (byte_counter = 0; byte_counter < 20; byte_counter++)
    {
        byte = hash.bytes[byte_counter];
        for (bit_counter = 0; bit_counter < 8; bit_counter++)
        {
            e[hash_counter] = (unsigned int*)((byte & (HASH_MASK << bit_counter)) != 0);
            hash_counter++;
        }
    }
}

void get_hash(SHA1_HASH *hash, char message[])
{
    Sha1Context hash_context;

    Sha1Initialise(&hash_context);
    Sha1Update(&hash_context, message, (uint32_t)strlen(message));
    Sha1Finalise(&hash_context, hash);
}

//----------------------------------------------

int main(void)
{    
    char message[1024];
    printf("Введите сообщение: ");
    scanf("%s", message);

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
    fsh_data_select_s_array(&data, data.n, randstate);

    // Вычисляем vj = sj^(-2) mod n
    fsh_data_calc_v(&data);

    ////////////// Signature generation //////////////
    // Выбираем случайное r
    fsh_data_calc_r(&data, data.n, randstate);
    gmp_printf("r = %Zd\n", data.r);

    // Вычисляем u = r^2 mod(n)
    fsh_data_calc_u(&data);
    gmp_printf("u = %Zd\n", data.u);

    // Конвертируем data.u из mpz_t в char
    char * u_ch = mpz_get_str(NULL, 10, data.u);

    // Добавляем u в конец сообщения
    char ann_mod_message[1024];
    strcpy(ann_mod_message, message);
    strcat(ann_mod_message, u_ch);
    printf("Модифицированное сообщение - %s\n", ann_mod_message);

    // Хэшируем SHA1
    SHA1_HASH hash;
    uint16_t i;
    get_hash(&hash, ann_mod_message);

    printf("hash: ");
    for( i=0; i<sizeof(hash); i++ )
    {
        printf( "%2.2x ", hash.bytes[i] );
    }
    printf("\n");


    // SHA1 -> {0, 1}
    sha1_to_bitarray(&data.e, hash);


    // Считаем s
    fsh_data_calc_s(&data);
    gmp_printf("s = %Zd\n", data.s);

    ///////////////// Verification ////////////////
    mpz_t w;
    mpz_init(w);
    fsh_calc_w(&w, data);
    gmp_printf("w = %Zd\n", w);

    // Конвертируем data.u из mpz_t в char
    char * w_ch = mpz_get_str(NULL, 10, w);

    // Добавляем u в конец сообщения
    char bob_mod_message[1024];
    strcpy(bob_mod_message, message);
    strcat(bob_mod_message, w_ch);
    printf("Модифицированное сообщение - %s\n", bob_mod_message);

    // Хэшируем SHA1
    SHA1_HASH bob_hash;
    get_hash(&bob_hash, bob_mod_message);

    printf("hash: ");
    for( i=0; i<sizeof(hash); i++ )
    {
        printf( "%2.2x ", bob_hash.bytes[i] );
    }
    printf("\n");

    // SHA1 -> {0, 1}
    unsigned int e[HASH_SIZE];
    sha1_to_bitarray(&e, bob_hash);

    if(memcmp(&data.e, &e, HASH_SIZE) == 0)
    {
        printf("Успешно\n");
    }
    else
    {
        printf("Ошибка");
    }
    return 1;
}
