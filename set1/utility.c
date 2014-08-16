#include <stdlib.h>
#include <string.h>

char* utility_hex_to_ascii(char *hexstr)
{   
    size_t hexlen = strlen(hexstr);

    /* every two characters in a hex string => one byte of ascii, + '\0' */
    size_t ascii_len = hexlen / 2;

    char *ascii = malloc(sizeof(char) * ascii_len + 1);

    if (ascii != NULL)
    {
        for (int i = 0; i < hexlen; i++)
        {
            for (int j = 0; j < 5; j += 4)
            {
                switch (*hexstr++)
                {
                    case '0' ... '9':
                        ascii[i] |= (*hexstr - '0') << j;
                        break;
                    case 'a' ... 'f':
                        ascii[i] |= (*hexstr - 'a') << j;
                        break;
                    case 'A' ... 'F':
                        ascii[i] |= (*hexstr - 'A') << j;
                        break;
                }
            }
        }

        ascii[ascii_len] = '\0';
    }

    return ascii;
}

#ifdef DEBUG
    #include <stdio.h>

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
