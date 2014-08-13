#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void fixed_xor_print_hex(unsigned long val, char hex_array[])
{
    if (val > 1) fixed_xor_print_hex(val / 16, hex_array);
    printf("%c", hex_array[val % 16]);
}

int main(void)
{
    char hex_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
        'b', 'c', 'd', 'e', 'f'};

    char *hex1 = "0x1c0111001f010100061a024b53535009181c";
    char *hex2 = "0x686974207468652062756c6c277320657965";

    unsigned long long hex_int1 = strtoull(hex1, NULL, 0);
    unsigned long long hex_int2 = strtoull(hex2, NULL, 0);
    unsigned long long result = hex_int1 ^ hex_int2;

    fixed_xor_print_hex(result, hex_array);
} 
