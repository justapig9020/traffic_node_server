HEADER = libsim.h debug.h conf.h

main: main.c libsim.o method.o
	gcc -g -o main main.c libsim.o method.o

libsim.o: libsim.c $(HEADER)
	gcc -c libsim.c

method.o: method.c $(HEADER)
	gcc -c method.c
