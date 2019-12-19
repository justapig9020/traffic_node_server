HEADER = libsim.h debug.h conf.h

all: sim.o

sim.o: libsim.o method.o
	gcc -c sim.c libsim.o method.o

main: main.c libsim.o method.o
	gcc -g -o main main.c libsim.o method.o

libsim.o: libsim.c $(HEADER)
	gcc -c libsim.c

method.o: method.c $(HEADER)
	gcc -c method.c
