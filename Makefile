CC=mpicc
NVCC=nvcc

OBJ_FLAGS=-O -c
LIBS=-lmpi

CUDA_INC=-I/usr/local/cuda/common/inc
CUDA_LIBS=-L/usr/local/cuda/lib64 -lcudart

all: main

main: main.o tiler.o
		$(CC) $(CUDA_INC) $(CUDA_LIBS) $(LIBS) main.o tiler.o -o main

main.o: main.c
		$(CC) $(OBJ_FLAGS) main.c

tiler.o: tiler.c
		$(CC) $(OBJ_FLAGS) tiler.c

# vectoradd.o: vectoradd.cu
		# $(NVCC) $(OBJ_FLAGS) vectoradd.cu

clean:
		rm -rf main *.o