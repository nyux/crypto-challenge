#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "utility/utility.h"
#include "utility/list.h"
#include "prob03-xor_cipher.h"

int main(void)
{
    list_t *hexstr_list = list_new();
    char *hexstr, *candidate, *best = malloc(1), *temp;
    int score, best_score = INT_MAX;
    size_t hexstr_len;

    while (!feof(stdin))
    {
        list_add(hexstr_list, utility_readline());
    }


    for (int i = 0; i < list_size(hexstr_list); i++)
    {
        hexstr = list_get(hexstr_list, i);
        hexstr_len = utility_ascii_len(hexstr);
        candidate = xor_cipher_decode(hexstr);
        score = xor_cipher_heuristic(candidate, hexstr_len);

        if (score < best_score)
        {
            temp = candidate;
            candidate = best;
            best = temp;
            best_score = score;
        }

        free(candidate);
    }

    printf("%s\n", best);

    for (int i = 0; i < list_size(hexstr_list); i++)
    {
        free(list_get(hexstr_list, i));
    }

    free(best);
    list_free(&hexstr_list);
}
