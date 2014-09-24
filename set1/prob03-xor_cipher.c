#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#include "utility/utility.h"

/* NOTE: lots of a previous version's variable names referred to things as
 * ascii strings (ascii_str, xor_ascii, ascii_len), which isn't necessarily
 * a valid assumption. (for example, the bytestring decoded from hex may
 * contain characters > 127 or contain '\0' bytes anywhere in the string (which
 * violates the assumption that strings are null-terminated arrays of chars.)
 * thus the refactoring. */

/* mallocs out a new chunk of memory with the size of the xor'd byte sequence.
 * halts the program if malloc fails. copies over the xor'd bytes into the
 * malloc'd memory and does an xor transformation on it. if one of the
 * resulting translated characters isn't in the range of ascii characters,
 * it returns NULL as an error condition. otherwise, it returns a translated
 * sequence of bytes, which may or may not be an english ascii string. */
char* xor_cipher_translate(char *xor_bytestr, size_t bytestr_len, char byte_to_xor)
{
    char *translated_bytes = malloc(bytestr_len + 1);

    if (!translated_bytes) utility_malloc_error();

    memcpy(translated_bytes, xor_bytestr, bytestr_len);
    translated_bytes[bytestr_len] = '\0';

    for (int i = 0; i < bytestr_len; i++)
    {
        translated_bytes[i] ^= byte_to_xor;

        /* any char below 10 is either > 127 when unsigned, or really unlikely
         * to be part of a valid byte string */
        if (translated_bytes[i] < 10)
        {
            free(translated_bytes);
            return NULL;
        }
    }

    return translated_bytes;
}

/* New heuristic copied over from girlfriend's code at
 * https://github.com/irene-h/cryptochallenge/blob/master/set1/challenge1-3.rb
 * with consent. */
/* gives a score to a particular character based on its frequency in the
 * english alphabet. the more common the letter, the lower the score. this
 * function is meant to be iterated over a bytestring to grade it -- the lower
 * the bytestring's grade, the higher the chance that it is english. this
 * sort of heuristic discriminates against strings with lots of numbers or
 * other ascii symbols, which is a shame. */
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


/* accumulates the score for a bytestring to assess the chance that it is
 * an actual english phrase. */
int xor_cipher_heuristic(const char *bytestr, size_t bytestr_len)
{
    int score = 0;

    for (int i = 0; i < bytestr_len; i++)
    {
        score += xor_cipher_get_score(bytestr[i]);
    }
    return score;
}

/* takes in a hex representation of a bytestring and transforms it to the
 * hexstring it represents. for every valid string (that is, passes the
 * requirements of translate(char *, size_t, char) it grades it. the best
 * string is preserved and the rest are freed after being passed through
 * the heuristic. it frees the decoded bytestring and returns the best
 * candidate for being an ascii string. */
char* xor_cipher_decode(char *hex_str)
{
    /* as per the note above, the names of these two functions is problematic
     * have to figure out something nice to refactor them into. */
    char *xor_bytestr = utility_hex_to_ascii(hex_str);
    size_t bytestr_len = utility_ascii_len(hex_str);

    char *translated_bytestr, *best = malloc(1), *temp;
    int score, best_score = INT_MAX;

    if (!xor_bytestr) utility_malloc_error();

    for (int i = 0; i < 255; i++)
    {
        translated_bytestr = xor_cipher_translate(xor_bytestr, bytestr_len, i);

        if (!translated_bytestr) continue;

        score = xor_cipher_heuristic(translated_bytestr, bytestr_len);

        if (score < best_score)
        {
            temp = translated_bytestr;
            translated_bytestr = best;
            best = temp;
            best_score = score;
        }

        free(translated_bytestr);
    }

    free(xor_bytestr);
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
