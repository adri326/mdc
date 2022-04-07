#include <stdio.h>
#include <gmp.h>
#include "stack.h"

int main() {
    mpz_stack_t stack = new_stack(10);

    mpz_t x;
    mpz_init(x);
    mpz_set_str(x, "10", 10);
    stack_push(&stack, x);

    mpz_t y;
    mpz_init(y);
    stack_pop(&stack, y);
    mpz_out_str(stdout, 10, y);
}
