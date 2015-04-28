#include <stdio.h>
#include <gmp.h>
#include <stdint.h>

void calc_wiki_example()
{
    /* Инициализируем стандартные значения */
    mpz_t two_mpz;
    mpz_init_set_d(two_mpz, 2);

    /* Инициализируем p, q. Считаем n */
    mpz_t p, q, n;
    mpz_init_set_d(p, 683);
    mpz_init_set_d(q, 811);
    mpz_init(n);
    mpz_mul(n, p, q);
    gmp_printf("n = %Zd\n\n", n);

    /* Инициализируем s. Считаем v */
    mpz_t s, v;
    mpz_init_set_d(s, 43215);
    mpz_init(v);
    mpz_powm(v, s, two_mpz, n);
    gmp_printf("v = %Zd\n\n", v);

    /* Инициализируем r. Cчитаем x */
    mpz_t r, x;
    mpz_init_set_d(r, 38177);
    mpz_init(x);
    mpz_powm(x, r, two_mpz, n);
    gmp_printf("x = %Zd\n\n", x);

    /* Отправляем 1 */
    int e = 1;
    mpz_t y;
    mpz_init(y);
    if (e == 0)
    {
        mpz_set(y, r);
    }
    else
    {
        mpz_t rs;
        mpz_init(rs);
        mpz_mul(rs, r, s);
        mpz_mod(y, rs, n);
    }
    gmp_printf("y = %Zd\n\n", y);

    /* Проверка */
    mpz_t l_side;
    mpz_init(l_side);
    mpz_powm(l_side, y, two_mpz, n);
    gmp_printf("left side = %Zd\n", l_side);

    mpz_t ve, xve, r_side;
    mpz_init(ve);
    mpz_init(xve);
    mpz_init(r_side);
    mpz_pow_ui(ve, v, e);
    mpz_mul(xve, x, ve);
    mpz_mod(r_side, xve, n);
    gmp_printf("right side = %Zd\n", r_side);

    if (mpz_cmp(l_side, r_side) == 0)
        printf("Проверка пройдена\n");
}

int main(void)
{
    calc_wiki_example();

    return 0;
}
