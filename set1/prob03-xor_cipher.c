#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#include "utility/utility.h"

char* xor_cipher_translate(char *xor_ascii, size_t ascii_len, char byte_to_xor)
{
    char *translated_ascii = malloc(ascii_len + 1);

    if (!translated_ascii) utility_malloc_error();

    memcpy(translated_ascii, xor_ascii, ascii_len);
    translated_ascii[ascii_len] = '\0';

    for (int i = 0; i < ascii_len; i++)
    {
        translated_ascii[i] ^= byte_to_xor;

        /* any char below 10 is either > 127 when unsigned, or really unlikely
         * to be part of a valid ascii string */
        if (translated_ascii[i] < 10)
        {
            free(translated_ascii);
            return NULL;
        }
    }

    return translated_ascii;
}

/* New heuristic copied over from girlfriend's code at
 * https://github.com/irene-h/cryptochallenge/blob/master/set1/challenge1-3.rb
 * with consent. */
int xor_cipher_get_score(char c)
{
    char *lower = "etaoinshrdlcumwfgypbvkjxqz";
    char *upper = "ETAOINSHRDLCUMWFGYPBVKJXQZ";

    /* there are 26 bloody letters in the english alphabet */
    size_t num_letters = 26;

    for (int i = 0; *lower; i++, lower++, upper++)
    {
        if (c == *lower || c == *upper) return i;
    }

    return num_letters;
}

int xor_cipher_heuristic(const char *str_to_examine, size_t str_len)
{
    int score = 0;

    for (int i = 0; i < str_len; i++)
    {
        score += xor_cipher_get_score(str_to_examine[i]);
    }
    return score;
}

char* xor_cipher_decode(char *hex_str)
{
    char *xor_ascii = utility_hex_to_ascii(hex_str);
    size_t ascii_len = utility_ascii_len(hex_str);

    char *translated_str, *best = malloc(1), *temp;
    int score, best_score = INT_MAX;

    if (!xor_ascii) utility_malloc_error();

    for (int i = 0; i < 255; i++)
    {
        translated_str = xor_cipher_translate(xor_ascii, ascii_len, i);

        if (!translated_str) continue;

        score = xor_cipher_heuristic(translated_str, ascii_len);

        if (score < best_score)
        {
            temp = translated_str;
            translated_str = best;
            best = temp;
            best_score = score;
        }

        free(translated_str);
    }

    free(xor_ascii);
    return best;
}

#ifdef RUNMAIN

    int main(void)
    {
        char *a = xor_cipher_decode("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736");
        char *b = xor_cipher_decode("773e7271687b3e67716b3e222d");
        char *c = xor_cipher_decode("6b10263c6e3b2c69282b3a26253c3d2c2530693e26272d2c3b2f3c256928272d69006e24692a262439252c3d2c25306920276925263f2c693e203d216930263c6769757a6b69756464691d21283d693e283a693d212c693a2c273d2c272a2c6900693e283a693d3b3020272e693d26692e2c3d6930263c693d26692d2c2a262d2c65692b2c2a283c3a2c69006925262626263f2c6930263c69286925263d676907263e69212c3b2c6e3a69233c3a3d693a26242c6924263b2c693d2c313d692f263b692f2025252c3b693d26692428222c693a3c3b2c6930263c3b6928252e263b203d2124692a2827692d2c2a262d2c69203d6769002f690069233c3a3d69222c2c39693e3b203d20272e65693d212c3b2c6e2525692b2c6939252c273d3069262f692d283d28693a2626276928272d6930263c3b6928252e263b203d2124693e2025256921283f2c692726693d3b263c2b252c67690126392c2f3c252530676907263e69212c3b2c6e3a693d266921263920272e6930263c692d202d276e3d692f263b2e2c3d693d2669282525262a283d2c692c27263c2e2169242c24263b306769737a690b2c2c39692b262639676961006e24692220272d69262f69263c3d69262f693d2120272e3a6900693e28273d693d26693e3b203d2c692d263e2765693a2669212c3b2c6e3a693a26242c6924263b2c692f2025252c3b67606900692526262626263f2c6930263c6769633a273c2e2e252c3a63");

        printf("%s\n", a); free(a);
        printf("%s\n", b); free(b);
        printf("%s\n", c); free(c);
    }
#endif
