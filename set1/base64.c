#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utility.h"

#define FIRST_CHAR(x) *(x)
#define SECOND_CHAR(x) *((x) + 1)
#define THIRD_CHAR(x) *((x) + 2)

void base64_convert(char a, char b, char c)
{
    int output[4];

    output[0] = (a & 0xFC) >> 2;
    output[1] = (a & 0x03) << 4 | (b & 0xF0) >> 4;
    output[2] = (b & 0x0F) << 2 | (c & 0xC0) >> 6;
    output[3] = c & 0x3F;

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
        }

        #ifdef DEBUG
            printf(" %d\n", output[i]);
        #endif
    }
}

/*
void base64_copy_hex_byte(const char *src, char dest[static 3])
{
    for (int i = 0; i < 3; i++) dest[i] = *(src + i);
    dest[2] = '\0';
}

void base64_hex_to_ascii(char *hex_str, int bytes_to_get, 
        char hex_to_convert[static 3], char ascii_values[static 3])
{
    for (int i = 0; i < bytes_to_get; i++, hex_str+= 2)
    {
        base64_copy_hex_byte(hex_str, hex_to_convert);
        ascii_values[i] = (char) strtol(hex_to_convert, NULL, 16);
        #ifdef DEBUG
            printf("%d ", ascii_values[i]);
        #endif
    }

}*/

void base64_encode(const char* hex_str)
{
    char *ascii_str = utility_hex_to_ascii(hex_str);
    char *ascii_copy = ascii_str;

    if (!ascii_str) exit(-1);

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

    free(ascii_copy);
}

int main(void)
{
    /* TODO: eventually read general input from stdin */

    #ifdef DEBUG
        base64_encode("49276d");
    #else
        base64_encode("49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d");
    #endif
}
