CC=mpicc
NVCC=nvcc

DEBUG=-g
OBJ_FLAGS=-O -c
LIBS=-lmpi

CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

all: golmpi god

god: god.c
		$(CC) god.c -o god

golmpi: golmpi.o life.o
		$(CC) $(CUDA_INC) $(CUDA_LIBS) $(LIBS) golmpi.o life.o -o golmpi

golmpi.o: golmpi.c golmpi.h
		$(CC) $(OBJ_FLAGS) golmpi.c

life.o: life.cu
		$(NVCC) $(OBJ_FLAGS) life.cu

clean:
		rm -rf god golmpi *.o

test40:
	make
	time mpirun -mca btl_tcp_if_include eth0 -n 4 --hostfile my_hostfile golmpi 5 10000.in 

test90:
	make
	time mpirun -mca btl_tcp_if_include eth0 -n 4 --hostfile my_hostfile golmpi 5 90.in 

test50:
	make
	time mpirun -mca btl_tcp_if_include eth0 -n 8 --hostfile my_hostfile golmpi 10000 100000.in 
