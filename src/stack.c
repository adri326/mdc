#include <stdlib.h>
#include <assert.h>
#include "stack.h"

mpz_stack_t new_stack(size_t alloc) {
    mpz_stack_t res = {
        .length = 0,
        .alloc = alloc
    };
    res.buffer = malloc(sizeof(mpz_t) * alloc);
    mpz_init(res.modulus);

    for (size_t n = 0; n < alloc; n++) {
        mpz_init(res.buffer[n]);
    }

    return res;
}

void free_stack(mpz_stack_t stack) {
    for (size_t n = 0; n < stack.alloc; n++) {
        mpz_clear(stack.buffer[n]);
    }
    mpz_clear(stack.modulus);
    free(stack.buffer);
}

void stack_push(mpz_stack_t* stack, const mpz_t num) {
    if (stack->length == stack->alloc) {
        // For some strange reason realloc doesn't work
        mpz_t* new_buffer = malloc(sizeof(mpz_t) * 2 * stack->alloc);
        assert(stack->buffer != NULL);

        size_t n;
        for (n = 0; n < stack->alloc; n++) {
            mpz_init(new_buffer[n]);
            mpz_set(new_buffer[n], stack->buffer[n]);
            mpz_clear(stack->buffer[n]);
        }
        free(stack->buffer);
        for (; n < stack->alloc * 2; n++) {
            mpz_init(new_buffer[n]);
        }
        stack->alloc = 2 * stack->alloc;
        stack->buffer = new_buffer;
    }

    mpz_set(stack->buffer[stack->length++], num);
}

void stack_pop(mpz_stack_t* stack, mpz_t res) {
    assert(stack->length > 0);

    mpz_set(res, stack->buffer[--stack->length]);
}
