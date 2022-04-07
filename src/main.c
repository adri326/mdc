#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <gmp.h>
#include <unistd.h>
#include "stack.h"

#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define twoop(lhs, rhs) \
    mpz_t lhs, rhs; \
    mpz_inits(lhs, rhs, NULL); \
    stack_pop(stack, rhs); \
    stack_pop(stack, lhs);

void divide(mpz_t numerator, mpz_t denominator, mpz_t modulus) {
    mpz_t v1, v2;
    mpz_inits(v1, v2, NULL);
    mpz_set_ui(v2, 1);

    mpz_t a, b, tmp, q;
    mpz_inits(a, b, tmp, q, NULL);
    mpz_set(a, modulus);
    mpz_set(b, denominator);
    mpz_mod(denominator, denominator, modulus);

    while (mpz_sgn(b) > 0) {
        // tmp = b
        mpz_set(tmp, b);
        // q = a / b
        // b = a % b
        mpz_fdiv_qr(q, b, a, b);
        // a = tmp
        mpz_set(a, tmp);

        // tmp = v1 - q * v2
        mpz_mul(tmp, v2, q);
        mpz_sub(tmp, v1, tmp);
        // v1 = v2
        mpz_set(v1, v2);
        // v2 = tmp
        mpz_set(v2, tmp);
    }

    mpz_mod(v1, v1, modulus);
    // q = numerator / a
    // tmp = numerator % a
    mpz_fdiv_qr(q, tmp, numerator, a);

    // If tmp != 0, print error
    if (mpz_sgn(tmp) != 0) {
        eprintf("Error: ");
        mpz_out_str(stderr, 10, numerator);
        eprintf(" isn't divisible by GCD(");
        mpz_out_str(stderr, 10, denominator);
        eprintf(", ");
        mpz_out_str(stderr, 10, modulus);
        eprintf(") = ");
        mpz_out_str(stderr, 10, a);
        eprintf("\n");

        mpz_clears(v1, v2, NULL);
        mpz_clears(a, b, tmp, q, NULL);
        return;
    }

    // Return q * v1
    mpz_mul(numerator, q, v1);

    mpz_clears(v1, v2, NULL);
    mpz_clears(a, b, tmp, q, NULL);
}

void mod_sqrt(mpz_t value, mpz_t modulus) {
    mpz_t tmp;
    mpz_init(tmp);

    mpz_mod_ui(tmp, modulus, 8);
    unsigned long int b = mpz_get_ui(tmp);

    if (b % 2 == 1) {
        mpz_set_ui(tmp, 1);
        mpz_sub(tmp, modulus, tmp);
        mpz_divexact_ui(tmp, tmp, 2);
        mpz_powm(tmp, value, tmp, modulus);
        if (mpz_cmp_ui(tmp, 1) != 0) {
            eprintf("Number ");
            mpz_out_str(stderr, 10, value);
            eprintf(" has no square root in modulus ");
            mpz_out_str(stderr, 10, modulus);
            eprintf("\n");
            mpz_clear(tmp);
            return;
        }
    }

    if (b == 3 || b == 7) {
        mpz_set_ui(tmp, 1);
        mpz_add(tmp, tmp, modulus);
        mpz_divexact_ui(tmp, tmp, 4);
        mpz_powm(value, value, tmp, modulus);
        mpz_mod(value, value, modulus);
    } else if (b == 5) {
        mpz_t v, i;
        mpz_inits(v, i, NULL);

        mpz_set_ui(tmp, 5);
        mpz_sub(tmp, modulus, tmp);
        mpz_divexact_ui(tmp, tmp, 8);

        mpz_set(v, value);
        mpz_add(v, v, v); // v = 2a
        mpz_powm(v, v, tmp, modulus); // = (2a)^(...)

        eprintf("v: ");
        mpz_out_str(stderr, 10, v);
        eprintf("\n");

        mpz_mul(i, v, v);
        mpz_mod(i, i, modulus);
        mpz_mul(i, i, value);
        mpz_add(i, i, i); // i = 2av²
        mpz_mod(i, i, modulus);

        eprintf("i: ");
        mpz_out_str(stderr, 10, i);
        eprintf("\n");

        mpz_set_ui(tmp, 1);
        mpz_sub(i, i, tmp); // i = 2av²-1
        mpz_mul(i, i, v); // i = v(2av²-1)
        mpz_mul(value, value, i); // value = av(2av²-1)

        mpz_mod(value, value, modulus);

        mpz_clears(v, i, NULL);
    } else {
        eprintf("Square root not implemented for modulus ");
        mpz_out_str(stderr, 10, modulus);
        eprintf("\n");
    }

    mpz_clear(tmp);
}

void handle_line(mpz_stack_t* stack, char* buffer) {
    size_t offset = 0;
    while (buffer[offset]) {
        // printf("%c", buffer[offset]);
        if (buffer[offset] >= '0' && buffer[offset] <= '9') {
            size_t number_start = offset;
            offset += 1;
            while (buffer[offset] >= '0' && buffer[offset] <= '9') offset += 1;

            char* number_raw = (char*)malloc(sizeof(char) * (offset - number_start + 1));
            strncpy(number_raw, &buffer[number_start], offset - number_start);
            number_raw[offset - number_start] = 0;

            mpz_t number;
            mpz_init(number);
            mpz_set_str(number, number_raw, 10);
            stack_push(stack, number);
            mpz_clear(number);

            free(number_raw);
            offset -= 1;
        } else if (buffer[offset] == 'p') {
            mpz_out_str(stdout, 10, stack->buffer[stack->length - 1]);
            printf("\n");
        } else if (buffer[offset] == 'n') {
            mpz_t number;
            mpz_init(number);
            stack_pop(stack, number);
            mpz_out_str(stdout, 10, number);
            mpz_clear(number);
        } else if (buffer[offset] == 'f') {
            for (size_t n = 0; n < stack->length; n++) {
                mpz_out_str(stdout, 10, stack->buffer[stack->length - 1 - n]);
                printf("\n");
            }
        } else if (buffer[offset] == 'c') {
            stack->length = 0;
        } else if (buffer[offset] == 'm') {
            mpz_t number;
            mpz_init(number);
            stack_pop(stack, number);

            mpz_set(stack->modulo, number);

            mpz_clear(number);
        } else if (buffer[offset] == 'M') {
            stack_push(stack, stack->modulo);
        } else if (buffer[offset] == '+') {
            twoop(lhs, rhs);
            mpz_add(lhs, lhs, rhs);
            if (mpz_sgn(stack->modulo) > 0) {
                mpz_mod(lhs, lhs, stack->modulo);
            }
            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == '%') {
            mpz_t number;
            mpz_init(number);
            stack_pop(stack, number);
            mpz_mod(number, number, stack->modulo);
            stack_push(stack, number);
            mpz_clear(number);
        } else if (buffer[offset] == '-') {
            twoop(lhs, rhs);
            mpz_sub(lhs, lhs, rhs);
            if (mpz_sgn(stack->modulo) > 0) {
                mpz_mod(lhs, lhs, stack->modulo);
            }
            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == '*') {
            twoop(lhs, rhs);
            mpz_mul(lhs, lhs, rhs);
            if (mpz_sgn(stack->modulo) > 0) {
                mpz_mod(lhs, lhs, stack->modulo);
            }
            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == '/') {
            twoop(lhs, rhs);

            if (mpz_sgn(stack->modulo) > 0) {
                divide(lhs, rhs, stack->modulo);
                mpz_mod(lhs, lhs, stack->modulo);
            } else {
                mpz_fdiv_q(lhs, lhs, rhs);
            }

            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == '^') {
            twoop(lhs, rhs);

            if (mpz_sgn(stack->modulo) > 0) {
                mpz_powm(lhs, lhs, rhs, stack->modulo);
            } else {
                if (mpz_fits_ulong_p(rhs)) {
                    mpz_pow_ui(lhs, lhs, mpz_get_ui(rhs));
                } else {
                    eprintf("Cannot raise to the power of ");
                    mpz_out_str(stderr, 10, rhs);
                    eprintf("\n");
                }
            }

            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == 'C') {
            // L[C]M
            twoop(lhs, rhs);

            mpz_lcm(lhs, lhs, rhs);

            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == 'G') {
            // [G]CD
            twoop(lhs, rhs);

            mpz_gcd(lhs, lhs, rhs);

            stack_push(stack, lhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == 'r') {
            // reverse
            twoop(lhs, rhs);
            stack_push(stack, lhs);
            stack_push(stack, rhs);
            mpz_clears(lhs, rhs, NULL);
        } else if (buffer[offset] == 'd') {
            stack_push(stack, stack->buffer[stack->length - 1]);
        } else if (buffer[offset] == 'z') {
            mpz_t number;
            mpz_init(number);
            mpz_set_ui(number, stack->length);

            stack_push(stack, number);

            mpz_clear(number);
        } else if (buffer[offset] == 'v') {
            mpz_t number;
            mpz_init(number);
            stack_pop(stack, number);

            mod_sqrt(number, stack->modulo);

            stack_push(stack, number);

            mpz_clear(number);
        } else if (buffer[offset] == '~') {
            mpz_t number;
            mpz_init(number);
            stack_pop(stack, number);

            mpz_mod(number, number, stack->modulo);
            mpz_sub(number, stack->modulo, number);

            stack_push(stack, number);
            mpz_clear(number);
        }

        offset += 1;
    }
    fflush(stdout);
}

int main() {
    mpz_stack_t stack = new_stack(10);
    #define BUFFER_SIZE 1024
    char buffer[BUFFER_SIZE] = {0,};

    while (true) {
        int status = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1);
        if (status > 0) {
            buffer[status] = 0;
            handle_line(&stack, buffer);
        } else {
            break;
        }
    }

    free_stack(stack);
}
