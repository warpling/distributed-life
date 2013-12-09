#include <float.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdint.h>
#include "mpi.h"

#define BUF_SIZE 10000
#define TOP 0
#define BOT 1
#define ROOT 0

int rank;
int nprocs;

int8_t* getGameTile(char *filename);
int32_t getDeclaredElementCount(char * filename);
void saveFrame(uint8_t *array, int arraySize, char *filename); 
void printArray(uint8_t *array, int arraySize);

