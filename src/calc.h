#ifndef CALC_H
#define CALC_H

#include <stdio.h>
#include <gmp.h>

#include "stack.h"
#define N_REGISTERS 256

#ifndef eprintf
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif

struct mdc_state {
    mpz_stack_t stack;
    mpz_stack_t registers[N_REGISTERS];

    mpz_t modulus;
};
typedef struct mdc_state mdc_state_t;

mdc_state_t new_state();
void free_state(mdc_state_t state);

void handle_line(mdc_state_t* state, char* buffer);

#endif // CALC_H
