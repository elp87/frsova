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

int is_simple(mpz_ptr a)
{
    mpz_t zero_mpz, two_mpz;
    mpz_init_set_ui(zero_mpz, 0);
    mpz_init_set_ui(two_mpz, 2);

    mpz_t half_a, half_a1;
    mpz_init(half_a);
    mpz_init(half_a1);

    mpz_div(half_a, a, two_mpz);
    mpz_add_ui(half_a1, half_a, 1);

    mpz_t mod_result;
    mpz_init(mod_result);
    mpz_mod(mod_result, a, two_mpz);

    if (mpz_cmp(mod_result, zero_mpz) == 0)
    {
        printf(" - не простое - делится на 2\n");
        return 0;
    }

    mpz_t counter;
    for(mpz_init_set_d(counter, 3);
        mpz_cmp(half_a1, counter) >= 0;
        mpz_add(counter, counter, two_mpz))
    {
        mpz_mod(mod_result, a, counter);
        if (mpz_cmp(mod_result, zero_mpz) == 0)
        {
            gmp_printf(" - не простое - делится на %Zd\n", counter);
            return 0;
        }
    }

    return 1;
}

int main(void)
{
    calc_wiki_example();

    mpz_t p_min, p_max, p, q;
    mpz_t two_mpz;
    mpz_init(p_min);
    mpz_init(p_max);
    mpz_init(p);
    mpz_init(q);
    mpz_init_set_ui(two_mpz, 2);
    mpz_pow_ui(p_min, two_mpz, 16);
    mpz_pow_ui(p_max, two_mpz, 32);
    gmp_randstate_t randstate;
    gmp_randinit_default(randstate);

    do
    {
        mpz_urandomm(p, randstate, p_max);
        mpz_add(p, p, p_min);
        gmp_printf("Проверка %Zd\n", p);
    }
    while(is_simple(p) == 0);
    gmp_printf("\nНайдено простое p - %Zd\n\n", p);

    do
    {
        do
        {
            mpz_urandomm(q, randstate, p_max);
            mpz_add(q, q, p_min);
            gmp_printf("Проверка %Zd\n", q);
        }
        while(is_simple(q) == 0);

    }
    while(mpz_cmp(p,q) == 0); /* Повторяем поиск, если p == q */
    gmp_printf("\nНайдено простое q - %Zd\n\n", q);

    return 0;
}
