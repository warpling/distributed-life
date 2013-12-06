#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 10000

// Takes a file of 'cells' (one byte ints) and reads them into a 1D array
// Assumes first four bytes contains an int representing the number of 'cells'
// Assumes rank is global to the node reading in the file
int8_t* getGameTile(void *filename) {
   FILE *fp = fopen((char *)filename, "r");

    if (fp) {
        // The first four bytes should contain the number of cells to be read in
        int32_t numElements;
        // lol what's error handling?
        fread(&numElements, 4, 1, fp);
        // Read the elements into a 1D array
        int numElementsInSubarray = numElements / n_procs;
        int8_t *subArray = (int8_t *) malloc((numElementsInSubarray + 1) * sizeof(int8_t));
        fseek(fp, (numElementsInSubarray * rank), SEEK_CUR);
        fread(subArray, 1, numElementsInSubarray, fp);
        printf("Read in %d cells.\n", numElements);

        // Get rid of dat shit
        return subArray;
    }
    // return -1;
}

int32_t getDeclaredElementCount(void * filename) {
    FILE *fp = fopen((char *)filename, "r");

    if (fp) {
        // The first four bytes should contain the number of cells to be read in
        int32_t numElements;
        fread(&numElements, 4, 1, fp);
        fclose(fp);
        return numElements;
    }

    // return -1;
}