CC=mpicc
NVCC=nvcc

OBJ_FLAGS=-O -c
LIBS=-lmpi

CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

all: golmpi

golmpi: golmpi.o tiler.o life.o
		$(CC) $(CUDA_INC) $(CUDA_LIBS) $(LIBS) golmpi.o tiler.o life.o -o golmpi

golmpi.o: golmpi.c
		$(CC) $(OBJ_FLAGS) golmpi.c

tiler.o: tiler.c
		$(CC) $(OBJ_FLAGS) tiler.c

life.o: life.cu
		$(NVCC) $(OBJ_FLAGS) life.cu

clean:
		rm -rf golmpi *.o
