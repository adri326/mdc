# Modular desk calculator (mdc)

A re-implementation of the [GNU program `dc`](https://linux.die.net/man/1/dc), in modular arithmetic.

As of right now, strings, registers, arrays and macros aren't implemented.

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

Spaces are optional, except between numbers.

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

## Warranty

This software is provided as-is, there is no warranty, to the extent permitted by law.
See [LICENSE.md](./LICENSE.md) for more detail.

This software is not fit for cryptographic uses, use it at your own risk.
