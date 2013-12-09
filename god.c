/*
   Creator of worlds and really stupid lifeforms
             
                  .-'';'-.
                ,'   <_,-.`.
               /)   ,--,_>\_\
              |'   (      \_ |
              |_    `-.    / |
               \`-.   ;  _(`/
                `.(    \/ ,'
                  `-....-'
 
              ascii credit: hjw
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char const *argv[])
{
    if(argc < 3) {
        fprintf(stderr, "Usage: god <totalNumberOfCells> <filename> <optional density>\n");
        return -1;
    }

    // Make some binary files full o' rando shit
    int density = 40;
    if(argc == 4) {
        density = atoi(argv[3]);
        if (density < 0 || density > 100) {
            fprintf(stderr, "Bad density value (please use something between 0 and 100)\nDefaulting to 40%%\n");
            density = 40;            
        }
    }

    FILE *fp = fopen((char *)argv[2], "w");
    int32_t numElements = atoi(argv[1]);

    // Write out the number of elements
    fwrite(&numElements, 4, 1, fp);

    // Generate some shiiiitte
    uint8_t cell = 0;
    srand(12345);
    int i;
    for (i = 0; i < numElements; i++)
    {
        if((rand() % 100) <= density)
            cell = 1;
        else
            cell = 0;

        fwrite(&cell, 1, 1, fp);
    }

    printf("File \"%s\" created with %d cells using a %d%% density.\n", argv[2], numElements, density);
    fclose(fp);
}
