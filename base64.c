#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FIRST_CHAR((x)) *(x)
#define SECOND_CHAR((x)) *((x) + 1)
#define THIRD_CHAR((x)) *((x) + 2)

void base64_convert(char a, char b, char c)
{
    printf("%d", (a & 0xFC) >> 2);
    printf("%d", (a & 0x02) << 4 | (b & 0xF0 >> 4));
    printf("%d", (b & 0x0F) << 2 | (c & 0xC0) >> 2);
    printf("%d", c & 0x3F);
}


int main(void)
{
    /* TODO: eventually read general input from stdin */
    char *hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";

    int hexlen = strlen(hex);
    int[] padding = {-1, -1};
    int i;

    
    for (i = 0; i < hexlen - hexlen%3; i += 3, hex = *(hex + 3))
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
    }
}
