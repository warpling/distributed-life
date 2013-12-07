CC=mpicc
NVCC=nvcc

OBJ_FLAGS=-O -c
LIBS=-lmpi

CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

all: golmpi

golmpi: golmpi.o life.o
		$(CC) $(CUDA_INC) $(CUDA_LIBS) $(LIBS) golmpi.o life.o -o golmpi

golmpi.o: golmpi.c golmpi.h
		$(CC) $(OBJ_FLAGS) golmpi.c

life.o: life.cu
		$(NVCC) $(OBJ_FLAGS) life.cu

clean:
		rm -rf golmpi *.o
