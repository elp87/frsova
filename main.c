#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <data.h>

void calc_data(struct fsh_data_t *data)
{

    fsh_data_calc_v(data);
    gmp_printf("v = %Zd\n\n", data->v);

    fsh_data_calc_x(data);
    gmp_printf("x = %Zd\n\n", data->x);

    /* Проверка e = 0 */
    int e = 0;
    fsh_data_calc_y(data, e);
    gmp_printf("y = %Zd\n\n", data->y);

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

    /* Проверка е = 1 */
    e = 1;
    fsh_data_calc_y(data, e);
    gmp_printf("y = %Zd\n\n", data->y);

    mpz_t l_side1, r_side1;
    mpz_init(l_side1);
    mpz_init(r_side1);

    fsh_data_check_left_side(l_side1, data);
    fsh_data_check_right_side(r_side1, data, e);

    gmp_printf("left side = %Zd\n", l_side);
    gmp_printf("right side = %Zd\n", r_side);

    if (mpz_cmp(l_side1, r_side1) == 0)
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
    fsh_data_init(&data, p , q);

    fsh_data_calc_n(&data);
    gmp_printf("n = %Zd\n\n", data.n);

    fsh_data_init_s(&data, s);
    fsh_data_init_r(&data, r);

    calc_data(&data);
}

void calc_rand_example()
{
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

    fsh_data_calc_n(&data);
    gmp_printf("n = %Zd\n\n", data.n);

    fsh_data_calc_s(&data, data.n, randstate);
    gmp_printf("s = %Zd\n\n", data.s);

    fsh_data_calc_r(&data, data.n, randstate);
    gmp_printf("r = %Zd\n\n", data.r);

    calc_data(&data);
}


int main(void)
{
    printf("Проверка примера по wiki\n");
    calc_wiki_example();    

    printf("Проверка на случайных числах\n");
    calc_rand_example();

    return 0;
}
