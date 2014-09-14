all: prob01 prob02 prob03

prob01:
	clang -o prob01.o -DRUNMAIN=1 prob01-base64.c utility/utility.c

prob02:
	clang -o prob02.o prob02-fixed_xor.c -lgmp

prob03:
	clang -o prob03.o -DRUNMAIN=1 prob03-xor_cipher.c utility/utility.c -lm

clean:
	rm *.o
