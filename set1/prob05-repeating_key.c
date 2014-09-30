#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility/utility.h"

char* repeating_key_encode(char *bytestr, size_t bytestr_len, char *key,
        size_t keylen)
{
    char *xor_bytestr = malloc(bytestr_len);

    if (!xor_bytestr) utility_malloc_error();

    memcpy(xor_bytestr, bytestr,  bytestr_len);

    for (int i = 0; i < bytestr_len; i++)
    {
        xor_bytestr[i] ^= key[i % keylen];
    }

    return xor_bytestr;
}



int main(void)
{
    char *teststr = "Burning 'em, if you ain't quick and nimble", *key = "ICE";
    char *raw_result = repeating_key_encode(teststr, strlen(teststr), key, 3);
    char *result;

    if (raw_result)
    {
        result = utility_bytestr_to_hexstr(raw_result, strlen(teststr));

        if (result)
        {
            printf("%s\n", result);
            free(result);
        }

        free(raw_result);
    }
}
