
all:
	python3 setup.py build_ext --inplace 

clr: clr.c syn.c shmctl.c
	gcc -o clr clr.c syn.c shmctl.c
