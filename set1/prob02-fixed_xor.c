#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

/* remember: compile with -lgmp flag */
/* written as a one-off script mostly because (as of now) it doesn't seem to
 * be needed by any other challenge. if that ends up not being the case,
 * it'll be rewritten with properly general functions and everything. */
int main(void)
{
    char *hex1 = "1c0111001f010100061a024b53535009181c";
    char *hex2 = "686974207468652062756c6c277320657965";

    mpz_t hexval1, hexval2, result;

    mpz_init_set_str(hexval1, hex1, 16);
    mpz_init_set_str(hexval2, hex2, 16);
    mpz_init(result);

    mpz_xor(result, hexval1, hexval2);

    gmp_printf("%Zx\n", result);
} 
