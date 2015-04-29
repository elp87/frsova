#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <data.h>

void calc_data(struct fsh_data_t *data)
{
    fsh_data_calc_n(data);
    gmp_printf("n = %Zd\n\n", data->n);

    fsh_data_calc_v(data);
    gmp_printf("v = %Zd\n\n", data->v);

    fsh_data_calc_x(data);
    gmp_printf("x = %Zd\n\n", data->x);

    int e = 0;
    fsh_data_calc_y(data, e);
    gmp_printf("y = %Zd\n\n", data->y);

    /* Проверка */
    mpz_t l_side, r_side;
    mpz_init(l_side);
    mpz_init(r_side);

    fsh_data_check_left_side(l_side, data);
    fsh_data_check_right_side(r_side, data, e);

    gmp_printf("left side = %Zd\n", l_side);
    gmp_printf("right side = %Zd\n", r_side);

    if (mpz_cmp(l_side, r_side) == 0)
        printf("Проверка пройдена\n\n");
    else
        printf("Проверка не пройдена\n\n");
}

void calc_wiki_example()
{
    struct fsh_data_t data;

    mpz_t p, q, s, r;
    mpz_init_set_d(p, 683);
    mpz_init_set_d(q, 811);
    mpz_init_set_d(s, 43215);
    mpz_init_set_d(r, 38177);
    fsh_data_init(&data, p , q, s, r);

    calc_data(&data);
}

int is_simple(mpz_ptr a)
{
    mpz_t zero_mpz, two_mpz;
    mpz_init_set_ui(zero_mpz, 0);
    mpz_init_set_ui(two_mpz, 2);

    mpz_t sqrt_a, sqrt_a1;
    mpz_init(sqrt_a);
    mpz_init(sqrt_a1);

    mpz_sqrt(sqrt_a, a);
    mpz_add_ui(sqrt_a1, sqrt_a, 1);

    mpz_t mod_result;
    mpz_init(mod_result);
    mpz_mod(mod_result, a, two_mpz);

    if (mpz_cmp(mod_result, zero_mpz) == 0)
    {
        return 0;
    }

    mpz_t counter;
    for(mpz_init_set_d(counter, 3);
        mpz_cmp(sqrt_a1, counter) >= 0;
        mpz_add(counter, counter, two_mpz))
    {
        mpz_mod(mod_result, a, counter);
        if (mpz_cmp(mod_result, zero_mpz) == 0)
        {
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
    }
    while(is_simple(p) == 0);
    gmp_printf("\nНайдено простое p - %Zd\n\n", p);

    do
    {
        do
        {
            mpz_urandomm(q, randstate, p_max);
            mpz_add(q, q, p_min);
        }
        while(is_simple(q) == 0);

    }
    while(mpz_cmp(p,q) == 0); /* Повторяем поиск, если p == q */
    gmp_printf("\nНайдено простое q - %Zd\n\n", q);

    return 0;
}
