#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utility/utility.h"

void base64_encode_convert(char *base64_str, char a, char b, char c)
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

char* base64_encode(const char *bytestr, size_t len)
{
    size_t remainder = len % 3;
    size_t base64_len = (len + (remainder == 0 ? 0 : 3 - remainder)) * 4/3 + 1;
    
    char *base64 = malloc(base64_len);
    char *current_pos = base64;

    for (int i = 0; i < len - remainder; i += 3, bytestr += 3, current_pos += 4)
    {
        base64_encode_convert(current_pos, bytestr[0], bytestr[1], bytestr[2]);
    }

    switch(remainder)
    {
        case 2:
            base64_encode_convert(current_pos, bytestr[0], bytestr[1], 0);
            break;
        case 1:
            base64_encode_convert(current_pos, bytestr[0], 0, 0);
            break;
    }

    base64[base64_len] = '\0';

    return base64;
}

void base64_decode_convert(char *decoded_str, char a, char b, char c, char d)
{
    int input[4];

    input[0] = a;
    input[1] = b;
    input[2] = c;
    input[3] = d;

    for (int i = 0; i < 4; i++)
    {
        switch(input[i])
        {
            case 'A' ... 'Z':
                input[i] -= 'A';
                break;
            case 'a' ... 'z':
                input[i] = input[i] - 'a' + 26;
                break;
            case '0' ... '9':
                input[i] = input[i] - '0' + 52;
                break;
            case '+':
                input[i] = 62;
                break;
            case '/':
                input[i] = 63;
                break;
            case '=':
                input[i] = 0;
                break;
        }
    }

    decoded_str[0] = ((input[0] & 0x3F) << 2) | ((input[1] & 0x30) >> 4);
    if (input[2])
    {
        decoded_str[1] = ((input[1] & 0x0F) << 4) | ((input[2] & 0x3C) >> 2);
    }
    if (input[3])
    {
        decoded_str[2] = ((input[2] & 0x03) << 6) | (input[3] & 0x3F);
    }
}



char* base64_decode(const char *base64_str)
{
    size_t base64_len = strlen(base64_str);
    size_t decoded_len = strlen(base64_str) * 3/4;

    char *decoded_str = malloc(decoded_len + 1);
    char *current_pos = decoded_str;

    if (!decoded_str) utility_malloc_error();
    
    for (int i = 0; i < base64_len; i += 4, base64_str += 4, current_pos += 3)
    {
        base64_decode_convert(current_pos, base64_str[0], base64_str[1],
                base64_str[2], base64_str[3]);
    }
    
    decoded_str[decoded_len] = '\0'; 
    return decoded_str;
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

        char *decoded_result = base64_decode(result);
        if (!decoded_result) utility_malloc_error();
        printf("%s\n", decoded_result);

        free(large_bytestr);
        free(result);
        free(decoded_result);
    }
#endif
