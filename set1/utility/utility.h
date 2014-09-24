#include <stdlib.h>

#ifndef UTILITY

#define UTILITY

char* utility_hex_to_ascii(const char *hex_str);
void utility_malloc_error();
void utility_file_error(const char *filename);
size_t utility_ascii_len(const char *hex_str);
char* utility_readline();
char* utility_resize_str(char *str, size_t newsize);

#endif
