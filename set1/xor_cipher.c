#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"

char* xor_cipher_translate(char *xor_ascii, char byte_to_xor)
{
    size_t xor_ascii_len = strlen(xor_ascii);
    char *translated_ascii = malloc(xor_ascii_len + 1);

    if (!translated_ascii) utility_malloc_error();

    strncpy(translated_ascii, xor_ascii, xor_ascii_len);
    translated_ascii[xor_ascii_len] = '\0';

    for (int i = 0; i < xor_ascii_len; i++)
    {
        translated_ascii[i] ^= byte_to_xor;

        if ((unsigned char) translated_ascii[i] > 127)
        {
            free(translated_ascii);
            return NULL;
        }
    }

    return translated_ascii;
}

void xor_cipher_heuristic(const char *str_to_examine)
{

}

void xor_cipher_decode(char *hex_str)
{
    char *xor_ascii = utility_hex_to_ascii(hex_str);
    char *translated_str;

    if (!xor_ascii) utility_malloc_error();

    for (int i = 0; i < 255; i++)
    {
        translated_str = xor_cipher_translate(xor_ascii, i);

        if (!translated_str) continue;

        xor_cipher_heuristic(translated_str);

        free(translated_str);
    }

    free(xor_ascii);
}

int main(void)
{
    xor_cipher_decode("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
}
