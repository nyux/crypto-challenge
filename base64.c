#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FIRST_CHAR(x) *(x)
#define SECOND_CHAR(x) *((x) + 1)
#define THIRD_CHAR(x) *((x) + 2)

void base64_convert(char a, char b, char c)
{
    int i;
    int output[4];

    output[0] = (a & 0xFC) >> 2;
    output[1] = (a & 0x03) << 4 | (b & 0xF0) >> 4;
    output[2] = (b & 0x0F) << 2 | (c & 0xC0) >> 6;
    output[3] = c & 0x3F;

    for (i = 0; i < 4; i++)
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

void base64_copy_hex_byte(const char *src, char dest[static 3])
{
    int i;

    for (i = 0; i < 3; i++) dest[i] = *(src + i);
    dest[2] = '\0';
}


int main(void)
{
    /* TODO: eventually read general input from stdin */
#ifdef DEBUG
    char *hex_str = "49276d";
#else
    char *hex_str = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
#endif

    char hex_to_convert[3]; /* two chars + '\0' */
    char ascii_values[3];

    int hexlen = strlen(hex_str);
    int padding[] = {-1, -1};
    int i;

    for (i = 0; i < hexlen - hexlen%6; i += 2, hex_str += 2)
    {
        
    }

    /* previous version of the code, as a straight ascii -> base64 converter */
    /* for (i = 0; i < hexlen - hexlen%3; i += 3, hex += 3)
    {
       base64_convert(FIRST_CHAR(hex), SECOND_CHAR(hex), THIRD_CHAR(hex));
    }

    switch (hexlen % 3)
    {
        case 2:
            base64_convert(FIRST_CHAR(hex), 0, 0);
            break;
        case 1:
            base64_convert(FIRST_CHAR(hex), SECOND_CHAR(hex), 0);
            break;
    }*/

}
