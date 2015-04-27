#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <stribog.h>

int main(void)
{    
    /* Определение стандартных значений */
    mpz_t zero_mpz;
    mpz_init_set_d(zero_mpz, 0);

    mpz_t one_mpz;
    mpz_init_set_d(one_mpz, 1);

    /* ------------- */
    mpz_t p;
    int a;
    mpz_t b;
    mpz_t q;
    mpz_t xp, yp;
    mpz_t d;
    mpz_t xq, yq;

    mpz_init_set_str(p, "4531 ACD1 FE00 23C7 550D 267B 6B2F EE80 922B 14B2 FFB9 0F04 D4EB 7C09 B5D2 D15D"
                        "F1D8 5274 1AF4 704A 0458 047E 80E4 546D 35B8 336F AC22 4DD8 1664 BBF5 28BE 6373", 16);
    a = 7;
    mpz_init_set_str(b, "1CFF 0806 A311 16DA 29D8 CFA5 4E57 EB74 8BC5 F377 E494 00FD D788 B649 ECA1 AC43"
                        "6183 4013 B2AD 7322 480A 89CA 58E0 CF74 BC9E 540C 2ADD 6897 FAD0 A308 4F30 2ADC", 16);
    mpz_init_set_str(q ,"4531 ACD1 FE00 23C7 550D 267B 6B2F EE80 922B 14B2 FFB9 0F04 D4EB 7C09 B5D2 D15D"
                        "A82F 2D7E CB1D BAC7 1990 5C5E ECC4 23F1 D86E 25ED BE23 C595 D644 AAF1 87E6 E6DF", 16);
    mpz_init_set_str(xp,"24D1 9CC6 4572 EE30 F396 BF6E BBFD 7A6C 5213 B3B3 D705 7CC8 25F9 1093 A68C D762"
                        "FD60 6112 62CD 838D C6B6 0AA7 EEE8 04E2 8BC8 4997 7FAC 33B4 B530 F1B1 2024 8A9A", 16);
    mpz_init_set_str(yp,"2BB3 12A4 3BD2 CE6E 0D02 0613 C857 ACDD CFBF 061E 91E5 F2C3 F324 47C2 59F3 9B2C"
                        "83AB 156D 77F1 496B F7EB 3351 E1EE 4E43 DC1A 18B9 1B24 640B 6DBB 92CB 1ADD 371E", 16);
    mpz_init_set_str(d, "BA60 48AA DAE2 41BA 4093 6D47 756D 7C93 091A 0E85 1466 9700 EE75 08E5 08B1 0207"
                        "2E81 23B2 200A 0563 322D AD28 27E2 714A 2636 B7BF D18A ADFC 6296 7821 FA18 DD4", 16);
    mpz_init_set_str(xq,"115D C5BC 9676 0C7B 4859 8D8A B9E7 40D4 C4A8 5A65 BE33 C181 5B5C 320C 8546 21DD"
                        "5A51 5856 D133 14AF 69BC 5B92 4C8B 4DDF F75C 4541 5C1D 9DD9 DD33 612C D530 EFE1", 16);
    mpz_init_set_str(yq,"37C7 C90C D40B 0F56 21DC 3AC1 B751 CFA0 E263 4FA0 503B 3D52 639F 5D7F B72A FD61"
                        "EA19 9441 D943 FFE7 F0C7 0A27 59A3 CDB8 4C11 4E1F 9339 FDF2 7F35 ECA9 3677 BEEC", 16);
    gmp_printf("%Zd\n\n", yq);
        
    uint8_t message[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

    /* Берем хэш от сообщения message */
    char *h512 = get_hash512(message);
    printf("Хэш - %s\n", h512);

    mpz_t hash_mpz;
    mpz_init_set_str(hash_mpz, h512, 16);
    gmp_printf("Десятичное представление хэша - %Zd\n\n", hash_mpz);

    /* e = z % q ,
       где z - десятичное представление хэша (hash_mpz) */
    mpz_t e;
    mpz_init(e);
    mpz_mod(e, hash_mpz, q);

    if (mpz_cmp(e, zero_mpz) == 0)
        mpz_init_set_d(e, 1);

    gmp_printf("e = %Zd\n\n", e);

    /* k - случайное. 0 < k < q
       Генерируем число от 0 до (q - 2). Прибавляем 1 и получаем случайное от 1 до (q - 1) включительно*/
    mpz_t k;
    mpz_init(k);
    gmp_randstate_t randstate;
    gmp_randinit_default(randstate);

    mpz_urandomm(k, randstate, q - 2);
    mpz_add(k, k, one_mpz);
    gmp_printf("k = %Zd\n\n", k);

    /* r = xC % q , где
       С = k * p,
       если r = 0, вернуться к расчету k*/
    mpz_t C;
    mpz_init(C);
    mpz_mul(C, k, p);
    gmp_printf("C = %Zd\n\n", C);

    return 0;
}
