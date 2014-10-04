all: prob01 prob02 prob03 prob04 prob05

prob01:
	clang -o prob01.o -DRUNMAIN=1 prob01-base64.c utility/utility.c

prob02:
	clang -o prob02.o prob02-fixed_xor.c -lgmp

prob03:
	clang -o prob03.o -DRUNMAIN=1 prob03-xor_cipher.c utility/utility.c

prob04:
	clang -o prob04.o prob04-find_xor_string.c utility/{utility,list}.c prob03-xor_cipher.c

prob05:
	clang -o prob05.o prob05-repeating_key.c utility/utility.c

clean:
	rm *.o
