#ifndef STACK_H
#define STACK_H

#include <gmp.h>

struct mpz_stack {
    mpz_t* buffer;
    size_t alloc;
    size_t length;

    mpz_t modulus;
};
typedef struct mpz_stack mpz_stack_t;

mpz_stack_t new_stack(size_t alloc);

void stack_push(mpz_stack_t* stack, const mpz_t num);
void stack_pop(mpz_stack_t* stack, mpz_t res);
void free_stack(mpz_stack_t stack);

#endif // STACK_H
