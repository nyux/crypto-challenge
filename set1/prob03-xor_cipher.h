#ifndef PROB04

#define PROB04

char* xor_cipher_translate(char *xor_bytestr, size_t bytestr_len, char byte_to_xor);
int xor_cipher_get_score(char c);
int xor_cipher_heuristic(const char *bytestr, size_t bytestr_len);
char* xor_cipher_decode(const char *hexstr);

#endif
