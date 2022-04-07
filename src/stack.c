#include <stdlib.h>
#include <assert.h>
#include "stack.h"

mpz_stack_t new_stack(size_t alloc) {
    mpz_stack_t res = {
        .length = 0,
        .alloc = alloc
    };
    res.buffer = malloc(sizeof(mpz_t) * alloc);

    for (size_t n = 0; n < alloc; n++) {
        mpz_init(res.buffer[n]);
    }

    return res;
}

void stack_push(mpz_stack_t* stack, const mpz_t num) {
    if (stack->length == stack->alloc) {
        stack->alloc = 2 * stack->alloc;
        stack->buffer = realloc(stack->buffer, stack->alloc);
        assert(stack->buffer != NULL);

        for (size_t n = stack->length; n < stack->alloc; n++) {
            mpz_init(stack->buffer[n]);
        }
    }

    mpz_set(stack->buffer[stack->length++], num);
}

void stack_pop(mpz_stack_t* stack, mpz_t res) {
    assert(stack->length > 0);

    mpz_set(res, stack->buffer[--stack->length]);
}
