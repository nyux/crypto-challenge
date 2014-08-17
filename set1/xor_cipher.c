#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "utility.h"

#define SAMPLE_FILE "sample.txt"

/* five of the more common symbols in the english language */
double ratio_of_e, ratio_of_a, ratio_of_s, ratio_of_r, ratio_of_space;

void generate_frequencies()
{
    char c;
    long total = 0;
    long frequencies[256];
    FILE *sample = fopen(SAMPLE_FILE, "r");

    if (!sample) utility_file_error(SAMPLE_FILE);

    for (int i = 0; i < 256; i++) frequencies[i] = 0;

    while ((c = fgetc(sample)) != EOF)
    {
        frequencies[c]++;
        total++;
    }

    ratio_of_e = (frequencies['e'] + frequencies['E']) / (double) total;
    ratio_of_a = (frequencies['a'] + frequencies['A']) / (double) total;
    ratio_of_r = (frequencies['r'] + frequencies['R']) / (double) total;
    ratio_of_s = (frequencies['s'] + frequencies['S']) / (double) total;
    ratio_of_space = frequencies[' '] / (double) total;

    fclose(sample);
}


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

int good_enough(double ratio1, double ratio2)
{
    return fabs(ratio1 - ratio2) < .02;
}

bool xor_cipher_heuristic(const char *str_to_examine)
{
    long frequencies[256];
    long total = 0;
    int score;

    for (int i = 0; i < 256; i++) frequencies[i] = 0;

    for (; *str_to_examine; str_to_examine++)
    {
        frequencies[*str_to_examine]++;
        total++;
    }
    
    double local_ratio_of_e = (frequencies['e'] + frequencies['E']) / (double) total;
    double local_ratio_of_a = (frequencies['a'] + frequencies['A']) / (double) total;
    double local_ratio_of_r = (frequencies['r'] + frequencies['R']) / (double) total;
    double local_ratio_of_s = (frequencies['s'] + frequencies['S']) / (double) total;
    double local_ratio_of_space = frequencies[' '] / (double) total;
   
    score =  good_enough(local_ratio_of_e, ratio_of_e)
        + good_enough(local_ratio_of_a, ratio_of_a)
        + good_enough(local_ratio_of_r, ratio_of_r)
        + good_enough(local_ratio_of_s, ratio_of_s)
        + good_enough(local_ratio_of_space, ratio_of_space);

    return score > 1;
}

void xor_cipher_decode(char *hex_str)
{
    char *xor_ascii = utility_hex_to_ascii(hex_str);
    char *translated_str;

    if (!xor_ascii) utility_malloc_error();

    generate_frequencies();

    for (int i = 0; i < 255; i++)
    {
        translated_str = xor_cipher_translate(xor_ascii, i);

        if (!translated_str) continue;

        if (xor_cipher_heuristic(translated_str)) 
        {
            printf("%s\n", translated_str);
        }

        free(translated_str);
    }

    free(xor_ascii);
}

int main(void)
{
    xor_cipher_decode("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
}
