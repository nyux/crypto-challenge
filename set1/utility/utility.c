#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utility.h"

size_t utility_ascii_len(const char *hex_str)
{
    /* every two characters in a hex string => one byte of ascii */
    return strlen(hex_str) / 2;
}

char* utility_hex_to_ascii(const char *hex_str)
{   
    size_t hex_len = strlen(hex_str);
    size_t ascii_len = utility_ascii_len(hex_str);
    char *ascii = calloc(ascii_len + 1, sizeof(char)); /* length + '\0' */

    if (ascii != NULL)
    {
        for (int i = 0; i < ascii_len; i++)
        {
            for (int j = 4; j >= 0; j -= 4)
            {
                switch (*hex_str)
                {
                    case '0' ... '9':
                        ascii[i] |= (*hex_str++ - '0') << j;
                        break;
                    case 'a' ... 'f':
                        ascii[i] |= (*hex_str++ - 'a' + 10) << j;
                        break;
                    case 'A' ... 'F':
                        ascii[i] |= (*hex_str++ - 'A' + 10) << j;
                        break;
                }
            }
        }
    }

    return ascii;
}

char* utility_bytestr_to_hexstr(const char *bytestr, size_t byte_len)
{
    int high = 1;
    size_t hexstr_len = byte_len * 2;
    char *hexstr = malloc(hexstr_len + 1);
    char letters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'};

    if (!hexstr) utility_malloc_error();

    for (int i = 0; i < hexstr_len; i++)
    {
        hexstr[i] = letters[high ? (bytestr[i/2] & 0xF0) >> 4 : bytestr[i/2] & 0x0F];
        high = !high;
    }

    hexstr[hexstr_len] = '\0';
    return hexstr;
}


void utility_malloc_error()
{
    fprintf(stderr, "there was a problem with malloc, killing program\n");
    exit(-1);
}

void utility_file_error(const char *filename)
{
    fprintf(stderr, "there was a problem opening the file %s, killing program\n",
            filename);
    exit(-1);
}

char* utility_readline()
{
    size_t str_len = 10, tally = 0;
    char c, *input = malloc(str_len); /* ten chars + '\0' */

    if (!input) utility_malloc_error();

    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (tally == str_len)
        {
            str_len *= 2;
            input = utility_resize_str(input, str_len);
        }
        input[tally] = c;
        tally++;
    }

    if (tally == str_len) utility_resize_str(input, ++str_len);
    input[tally] = '\0';

    return input;
}

char* utility_resize_str(char *str, size_t newsize)
{
    str = realloc(str, newsize);
    if (!str) utility_malloc_error();
    return str;
}


#ifdef UTILITYDEBUG
    int main(void)
    {
        char *translation = utility_hex_to_ascii("61736466");
        
        if (translation) 
        {
            printf("%s\n", translation); /* should print asdf */
            free(translation);
        }

        for (int i = 0; i < 3; i++) {
        char *thing = utility_readline();
        if (thing) { printf("%s\n", thing); free(thing); }
        }

        char *hexstr = utility_bytestr_to_hexstr("asdf", 4);

        if (hexstr) { printf("%s\n", hexstr); free(hexstr); }

        return 0;
    }
#endif
