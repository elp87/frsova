#ifndef DATA_H
#define DATA_H

#include <gmp.h>

#define HASH_SIZE 8

struct fsh_data_t {
    mpz_t p, q, r;
    mpz_t n, u, y;
    mpz_t s;

    mpz_t _two_mpz;
    mpz_t _m_two_mpz;

    mpz_t sj[HASH_SIZE];
    mpz_t v[HASH_SIZE];

    unsigned int e[HASH_SIZE];
};

void fsh_data_init(struct fsh_data_t *data,
                   mpz_ptr p,
                   mpz_ptr q)
{    
    mpz_init_set(data->p, p);
    mpz_init_set(data->q, q);    

    mpz_init_set_d(data->_two_mpz, 2);
    mpz_init_set_d(data->_m_two_mpz, -2);
}


void fsh_data_calc_n(struct fsh_data_t *data)
{
    mpz_init(data->n);
    mpz_mul(data->n, data->p, data->q);
}

void fsh_data_select_s_array(struct fsh_data_t *data, mpz_ptr n, gmp_randstate_t randstate)
{
    uint8_t i=0;

    for (i = 0; i < 8; i++)
    {
        mpz_init(data->sj[i]);

        do
        {
            mpz_urandomm(data->sj[i], randstate, n);
            mpz_nextprime(data->sj[i], data->sj[i]);
        }
        while(mpz_cmp(data->n, data->sj[i]) < 1);
    }
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
    uint8_t i = 0;
    for (i = 0; i < HASH_SIZE; i++)
    {
        mpz_init(data->v[i]);
        mpz_powm(data->v[i], data->sj[i], data->_m_two_mpz, data->n);
    }
}

void fsh_data_calc_u(struct fsh_data_t *data)
{
    mpz_init(data->u);
    mpz_powm(data->u, data->r, data->_two_mpz, data->n);
}

void fsh_data_calc_s(struct fsh_data_t *data)
{
    mpz_init_set(data->s, data->r);
    int i = 0;
    mpz_t temp;
    mpz_init(temp);
    for (i = 0;i < HASH_SIZE; i++)
    {
        mpz_pow_ui(temp, data->sj[i], data->e[i]); // sj^ej
        mpz_mul(data->s, data->s, temp); // s *= sj^ej
    }
    mpz_mod(data->s, data->s, data->n);
}

//-------- Bob
void fsh_calc_w(mpz_t *w, struct fsh_data_t data)
{
    mpz_pow_ui(w, data.s, 2);

    int i;
    mpz_t temp;
    mpz_init(temp);
    for (i = 0; i < HASH_SIZE; i++)
    {
        mpz_pow_ui(temp, data.v[i], data.e[i]);
        mpz_mul(w, w, temp);
    }
    mpz_mod(w, w, data.n);
}

#endif // DATA_H
