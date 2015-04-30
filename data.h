#ifndef DATA_H
#define DATA_H

#include <gmp.h>

struct fsh_data_t {
    mpz_t p, q, s, r;
    mpz_t n, v, x, y;

    mpz_t _two_mpz;

};

void fsh_data_init(struct fsh_data_t *data,
                   mpz_ptr p,
                   mpz_ptr q)
{    
    mpz_init_set(data->p, p);
    mpz_init_set(data->q, q);    

    mpz_init_set_d(data->_two_mpz, 2);
}


void fsh_data_calc_n(struct fsh_data_t *data)
{
    mpz_init(data->n);
    mpz_mul(data->n, data->p, data->q);
}

void fsh_data_init_s(struct fsh_data_t *data, mpz_ptr s)
{
    mpz_init_set(data->s, s);
}

void fsh_data_calc_s(struct fsh_data_t *data, mpz_ptr n, gmp_randstate_t randstate)
{
    mpz_init(data->s);

    do
    {
        mpz_urandomm(data->s, randstate, n);
        mpz_nextprime(data->s, data->s);
    }
    while(mpz_cmp(data->n, data->s) < 1);
}

void fsh_data_calc_r(struct fsh_data_t *data, mpz_ptr n, gmp_randstate_t randstate)
{
    mpz_init(data->r);

    do
    {
        mpz_urandomm(data->r, randstate, n);
        mpz_nextprime(data->r, data->r);
    }
    while(mpz_cmp(data->n, data->r) < 1);
}

void fsh_data_init_r(struct fsh_data_t *data, mpz_ptr r)
{
    mpz_init_set(data->r, r);
}

void fsh_data_calc_v(struct fsh_data_t *data)
{
    mpz_init(data->v);
    mpz_powm(data->v, data->s, data->_two_mpz, data->n);
}

void fsh_data_calc_x(struct fsh_data_t *data)
{
    mpz_init(data->x);
    mpz_powm(data->x, data->r, data->_two_mpz, data->n);
}

void fsh_data_calc_y(struct fsh_data_t *data, int e)
{
    mpz_init(data->y);
    if (e == 0)
    {
        mpz_set(data->y, data->r);
    }
    else
    {
        mpz_t rs;
        mpz_init(rs);
        mpz_mul(rs, data->r, data->s);
        mpz_mod(data->y, rs, data->n);
    }
}

void fsh_data_check_left_side(mpz_ptr left_side, struct fsh_data_t *data)
{
    mpz_powm(left_side, data->y, data->_two_mpz, data->n);
}

void fsh_data_check_right_side(mpz_ptr right_side, struct fsh_data_t *data, int e)
{
    mpz_t ve, xve;
    mpz_init(ve);
    mpz_init(xve);
    mpz_pow_ui(ve, data->v, e);
    mpz_mul(xve, data->x, ve);
    mpz_mod(right_side, xve, data->n);
}
#endif // DATA_H
