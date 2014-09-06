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


char* xor_cipher_translate(char *xor_ascii, size_t ascii_len, char byte_to_xor)
{
    char *translated_ascii = malloc(ascii_len + 1);

    if (!translated_ascii) utility_malloc_error();

    memcpy(translated_ascii, xor_ascii, ascii_len);
    translated_ascii[ascii_len] = '\0';

    for (int i = 0; i < ascii_len; i++)
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

bool xor_cipher_heuristic(const char *str_to_examine, size_t str_len)
{
    long frequencies[256];
    long total = 0;
    int score;

    for (int i = 0; i < 256; i++) frequencies[i] = 0;

    for (int i = 0; i < str_len; i++, str_to_examine++)
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
    size_t ascii_len = utility_ascii_len(hex_str);
    char *translated_str;

    if (!xor_ascii) utility_malloc_error();

    generate_frequencies();

    for (int i = 0; i < 255; i++)
    {
        translated_str = xor_cipher_translate(xor_ascii, ascii_len, i);

        if (!translated_str) continue;

        if (xor_cipher_heuristic(translated_str, ascii_len)) 
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
    xor_cipher_decode("773e7271687b3e67716b3e222d");
    xor_cipher_decode("6b10263c6e3b2c69282b3a26253c3d2c2530693e26272d2c3b2f3c256928272d69006e24692a262439252c3d2c25306920276925263f2c693e203d216930263c6769757a6b69756464691d21283d693e283a693d212c693a2c273d2c272a2c6900693e283a693d3b3020272e693d26692e2c3d6930263c693d26692d2c2a262d2c65692b2c2a283c3a2c69006925262626263f2c6930263c69286925263d676907263e69212c3b2c6e3a69233c3a3d693a26242c6924263b2c693d2c313d692f263b692f2025252c3b693d26692428222c693a3c3b2c6930263c3b6928252e263b203d2124692a2827692d2c2a262d2c69203d6769002f690069233c3a3d69222c2c39693e3b203d20272e65693d212c3b2c6e2525692b2c6939252c273d3069262f692d283d28693a2626276928272d6930263c3b6928252e263b203d2124693e2025256921283f2c692726693d3b263c2b252c67690126392c2f3c252530676907263e69212c3b2c6e3a693d266921263920272e6930263c692d202d276e3d692f263b2e2c3d693d2669282525262a283d2c692c27263c2e2169242c24263b306769737a690b2c2c39692b262639676961006e24692220272d69262f69263c3d69262f693d2120272e3a6900693e28273d693d26693e3b203d2c692d263e2765693a2669212c3b2c6e3a693a26242c6924263b2c692f2025252c3b67606900692526262626263f2c6930263c6769633a273c2e2e252c3a63");
}
