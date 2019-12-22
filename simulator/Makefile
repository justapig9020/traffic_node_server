
all:
	python3 setup.py build_ext --inplace 

main: libsim.o sim.o method.o main.c debug.h
	gcc -o main main.c libsim.o sim.o method.o

libsim.o: libsim.c libsim.h debug.h
	gcc -c libsim.c

sim.o: sim.c sim.h debug.h
	gcc -c sim.c

method.o: method.c method.h debug.h
	gcc -c method.c
