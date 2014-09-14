#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utility/utility.h"

#define FIRST_CHAR(x) *(x)
#define SECOND_CHAR(x) *((x) + 1)
#define THIRD_CHAR(x) *((x) + 2)

/* does an ascii -> base64 conversion, three characters at a time. the 
 * conversion involves turning three bytes into four, by taking six bits 
 * from these characters at a time and placing them into a byte. depending
 * on the value of the output bytes, a specific set of characters is printed.
 * returns nothing.
 */
void base64_convert(char a, char b, char c)
{
    int output[4];

    output[0] = (a & 0xFC) >> 2;
    output[1] = (a & 0x03) << 4 | (b & 0xF0) >> 4;
    output[2] = b == 0 ? 64 : (b & 0x0F) << 2 | (c & 0xC0) >> 6;
    output[3] = c == 0 ? 64 : c & 0x3F;

    for (int i = 0; i < 4; i++)
    {
        /** i know, gcc extension. still, was convenient at the time */
        switch(output[i])
        {
            case 0 ... 25:
                printf("%c", 'A' + output[i]);
                break;
            case 26 ... 51:
                printf("%c", 'a' + output[i] - 26);
                break;
            case 52 ... 61:
                printf("%c", '0' + output[i] - 52);
                break;
            case 62:
                printf("+");
                break;
            case 63:
                printf("/");
                break;
            case 64:
                printf("=");
                break;
        }

        #ifdef DEBUG
            printf(" %d\n", output[i]);
        #endif
    }
}

/* turns the initial hex string into ascii. the ascii string is decomposed
 * three characters at a time. if the ascii string's length isn't divisible
 * by three, it passes in 0 bytes. at the end, it frees the ascii string
 */
void base64_encode(const char* hex_str)
{
    char *ascii_str = utility_hex_to_ascii(hex_str);
    char *ascii_copy = ascii_str;

    if (!ascii_str) utility_malloc_error();

    size_t ascii_len = strlen(ascii_str);

    for (int i = 0; i < ascii_len - ascii_len%3; i += 3, ascii_str += 3)
    {
        base64_convert(FIRST_CHAR(ascii_str), SECOND_CHAR(ascii_str),
                THIRD_CHAR(ascii_str));
    }

    switch(ascii_len % 3)
    {
        case 2:
            base64_convert(FIRST_CHAR(ascii_str), SECOND_CHAR(ascii_str), 0);
            break;
        case 1:
            base64_convert(FIRST_CHAR(ascii_str), 0, 0);
            break;
    }

    putchar('\n');

    free(ascii_copy);
}

#ifdef RUNMAIN
    int main(void)
    {
        /* TODO: eventually read general input from stdin */
        base64_encode("49276d");
        base64_encode("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    }
#endif
