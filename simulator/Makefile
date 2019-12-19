
all:
	python3 setup.py build_ext --inplace 

main: libsim.o sim.o method.o
	gcc -o main main.c libsim.o sim.o method.o

libsim.o: 
	gcc -c libsim.c

sim.o:
	gcc -c sim.c

method.o:
	gcc -c method.c
