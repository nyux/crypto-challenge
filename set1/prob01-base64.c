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
void base64_convert(char *base64_str, char a, char b, char c)
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
                *(base64_str + i) = 'A' + output[i];
                break;
            case 26 ... 51:
                *(base64_str + i) = 'a' + output[i] - 26;
                break;
            case 52 ... 61:
                *(base64_str + i) = '0' + output[i] - 52;
                break;
            case 62:
                *(base64_str + i) = '+';
                break;
            case 63:
                *(base64_str + i) = '/';
                break;
            case 64:
                *(base64_str + i) = '=';
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
char* base64_encode(const char* bytestr, size_t len)
{
    size_t remainder = len % 3;
    size_t base64_len = (len + (remainder == 0 ? 0 : 3 - remainder)) * 4/3 + 1;
    
    char *base64 = malloc(base64_len);
    char *current_pos = base64;

    for (int i = 0; i < len - remainder; i += 3, bytestr += 3, current_pos += 4)
    {
        base64_convert(current_pos, FIRST_CHAR(bytestr), SECOND_CHAR(bytestr),
                THIRD_CHAR(bytestr));
    }

    switch(remainder)
    {
        case 2:
            base64_convert(current_pos, FIRST_CHAR(bytestr),
                    SECOND_CHAR(bytestr), 0);
            break;
        case 1:
            base64_convert(current_pos, FIRST_CHAR(bytestr), 0, 0);
            break;
    }

    base64[base64_len] = '\0';

    return base64;
}

#ifdef RUNMAIN
    int main(void)
    {
        /* TODO: eventually read general input from stdin */
        char *small_test = "49276d";
        char *small_bytestr = utility_hex_to_ascii(small_test);
        size_t small_size = utility_ascii_len(small_test);
        
        if (!small_bytestr) utility_malloc_error();
        
        char *result = base64_encode(small_bytestr, small_size);
        if (!result) utility_malloc_error();
        printf("%s\n", result);

        free(small_bytestr);
        free(result);

        char *large_test = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
        char *large_bytestr = utility_hex_to_ascii(large_test);
        size_t large_size = utility_ascii_len(large_test);

        if (!large_bytestr) utility_malloc_error();
        
        result = base64_encode(large_bytestr, large_size);
        if (!result) utility_malloc_error();
        printf("%s\n", result);

        free(large_bytestr);
        free(result);
    }
#endif
