# Modular desk calculator (mdc)

A re-implementation of the [GNU program `dc`](https://linux.die.net/man/1/dc), in modular arithmetic.

As of right now, strings, arrays and macros aren't implemented.

The supported operations are:

- `<number>`, `_<number>`: pushes the number `number` onto the stack. If it is preceded by `_`, pushes the negative value for it.
- `m`: pops a value from the stack and sets it as the modulus for all subsequent operations; if set to 0 (default), then no modulus is applied. This number is referred to as `m`
- `M`: pushes onto the stack the value of `m`
- `%`: pops a number `x` from the stack and pushes back `x mod m`
- `+`: pops two numbers `a` and `b` and pushes back `(a + b) mod m` (or `a + b` if `m = 0`)
- `-`: pops two numbers `a` and `b` and pushes back `(a - b) mod m` (or `a - b` if `m = 0`)
- `*`: pops two numbers `a` and `b` and pushes back `(a * b) mod m` (or `a * b` if `m = 0`)
- `/`: pops two numbers `a` and `b` and pushes back `(a / b) mod m` (or `a // b` if `m = 0`); requires that `gcd(b, m)` divides `a`
- `^`: pops two numbers `a` and `b` and pushes back `(a ^ b) mod m` (or `a ^ b` if `m = 0`; in that case, `b` must be less than `ULONG_MAX`)
- `C`: pops two numbers `a` and `b` and pushes back `lcm(a, b)`
- `G`: pops two numbers `a` and `b` and pushes back `gcd(a, b)`
- `d`: duplicates the value on top of the stack
- `z`: pushes on the stack the current length of the stack
- `v`: pops a number `x` from the stack and computes the modular square root of `x`, see [Modular square root](https://www.rieselprime.de/ziki/Modular_square_root) for more information *(Note: composite moduli and moduli congruent to 1 mod 8 haven't been implemented yet.)*
- `~`: pops a number `x` from the stack and pushes back `-x mod m` (or `m` if `m = 0`)
- `f`: prints the entirety of the stack
- `p`: prints the top value of the stack
- `n`: pops a number from the stack and prints it, without a newline
- `c`: clears the stack
- `r`: swaps the position of the top two values of the stack
- `s<r>`: pops `x` from the stack and stores in on the top of the register `<r>` (overriding the previous value)
- `S<r>`: pops `x` from the stack and pushes it on top of the register `<r>`'s stack (shadowing the previous value)
- `l<r>`: pushes on the stack the top value of the register `<r>`
- `L<r>`: pops `x` from the stack of the register `<r>` and pushes it onto the stack (un-shadowing values below it)

Spaces are optional, except between numbers.
I really recommend reading through [`dc`'s documentation](https://linux.die.net/man/1/dc) if you aren't somewhat familiar with it already.

## Installation

To compile and use this software, you will need `git`, GNU Make, a C compiler and the [GNU MP library](https://gmplib.org/manual/index) (`gmp` on Arch Linux).

First, clone this repository, then compile it with `make`:

```sh
git clone https://github.com/adri326/mdc
cd mdc
make
```

The final binary will be put in `build/mdc`.
You can copy it into a directory that is in your PATH to have the command `mdc` available everywhere.

## Running

Simply execute the `mdc` binary; it will then read input from `stdin` and print output to `stdout`.

If you aren't piping anything into `mdc`, then it will act as a kind of terminal; you may write and edit commands, and submit them by pressing `enter`.
To exit, press `Ctrl+D` to send the EoF character.

## Example usage

```sh
31m # Set the modulus to 31
10 # Push 10
2* p # Prints 20
2* p # Prints 40%31 = 9
4* p # Prints (9*4) % 31 = 5
```

The following expression computes `(23*(3*1+40))² - 2 mod 103`:

```sh
103m
1 3* 40 + # (3*1 + 40)
23* 2^ # (23*(3*1+40))²
2- # (23*(3*1+40))² - 2
p # Prints 31
```

The following encrypts and decrypts a message using RSA:

```sh
2010942103422233250095259520183 sp # p
3503815992030544427564583819137 sq # q
lplq*p sn # n
lp1-lq1-Cm # Compute λ(n) and use it as modulo
2 16^ 1+ d se # e = 2^16+1
1r/p sd # d = e^-1 mod λ(n)

lnm # Use n as modulo
36762444129608 le ^p # Encode the message with (n, e); here the message is the decimal version of "Hello!"
ld ^p # Decode the message
```

For comparison, here is how you would have to do it using vanilla `dc`:

```sh
2010942103422233250095259520183 sp # p
3503815992030544427564583819137 sq # q
lplq*p sn # n
lp1- lq1- # Load p-1 and q-1
[dSarLa%d0<a]dsax+ # Compute gcd(p-1, q-1), thank you Wikipedia for the expression
lp1- lq1- *r/ sl # Compute λ(n) and store it
2 16^ 1+ d se # e = 2^16+1
lldsm le dsa1sv[dsb~rsqlbrldlqlv*-lvsdsvd0<x]dsxxldd[dlmr+]sx0>x p sd # Computes d = e^-1 mod λ(n), thanks Rosetta stone for this expression

36762444129608 le ln |p # Encode the message with (n, e); here the message is the decimal version of "Hello!"
ld ln |p # Decode the message
```

## Warranty

This software is provided as-is, there is no warranty, to the extent permitted by law.
See [LICENSE.md](./LICENSE.md) for more detail.

This software is not fit for cryptographic uses, use it at your own risk.
