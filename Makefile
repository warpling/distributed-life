CC=mpicc
NVCC=nvcc

DEBUG=-g
OBJ_FLAGS=-O -c
LIBS=-lmpi

CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

all: golmpi

golmpi: golmpi.o life.o
		$(CC) $(DEBUG) $(CUDA_INC) $(CUDA_LIBS) $(LIBS) golmpi.o life.o -o golmpi

golmpi.o: golmpi.c golmpi.h
		$(CC) $(DEBUG) $(OBJ_FLAGS) golmpi.c

life.o: life.cu
		$(NVCC) $(DEBUG) $(OBJ_FLAGS) life.cu

clean:
		rm -rf golmpi *.o

test40:
	make
	time mpirun -mca btl_tcp_if_include eth0 -n 4 --hostfile my_hostfile golmpi 5 10000.in 

test90:
	make
	time mpirun -mca btl_tcp_if_include eth0 -n 4 --hostfile my_hostfile golmpi 5 90.in 
