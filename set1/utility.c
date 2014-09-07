#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t utility_ascii_len(const char *hex_str)
{
    /* every two characters in a hex string => one byte of ascii */
    return strlen(hex_str) / 2;
}

char* utility_hex_to_ascii(char *hex_str)
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

#ifdef DEBUG

    int main(void)
    {
        char *translation = utility_hex_to_ascii("61736466");
        
        if (translation) 
        {
            printf("%s", translation); /* should print asdf */
            free(translation);
        }

        return 0;
    }
#endif
