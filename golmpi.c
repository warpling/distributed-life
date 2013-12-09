#include "golmpi.h"

int main(int argc, char **argv)
{
   MPI_Init(&argc,&argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   MPI_Comm_size(MPI_COMM_WORLD,&nprocs); 
   if (argc != 3) {
      if (rank == 0) printf("Usage: gol <num of generations>  <input file>\n");
      MPI_Abort(MPI_COMM_WORLD,1);
      exit(1);
   }

   int maxGen = atoi(argv[1]);

   int numElements = getDeclaredElementCount(argv[2]);

   int width = sqrt(numElements);
   int length = numElements / nprocs;
   uint8_t* myBoard; //length
   uint8_t* myTop;//width
   uint8_t* myBot;//width
   uint8_t* otherBot;// width
   uint8_t* otherTop;// width
   
   otherBot = (uint8_t *) calloc(width, sizeof(int));   
   otherTop = (uint8_t *) calloc(width, sizeof(int));   

   //read data here
   myBoard = getGameTile(argv[2]); 

   int err;
   int i;

   MPI_Status status;
   MPI_Request send_request0, send_request1, recv_request0, recv_request1;

   for(i = 0; i < maxGen; i++)
   {
      //set myTop and myBot
      myTop = myBoard;

      printf("\nrank %d gen %d myTop ", rank, i);
      printArray(myTop, width);

      myBot = myBoard + (length * sizeof(uint8_t)) - (width * sizeof(uint8_t));
      printf("\nrank %d gen %d myBot ", rank, i);
      printArray(myBot, width);

      //send other top and bottoms around, TOP and BOT relative to sender
      if(nprocs > 1) {
         if(rank == 0)
         {
            err = MPI_Isend(myBot, width, MPI_INT, rank + 1, BOT, MPI_COMM_WORLD, &send_request0); 
            err = MPI_Irecv(otherTop, width, MPI_INT, rank + 1, TOP, MPI_COMM_WORLD, &recv_request0);
         }
         else if(rank == nprocs - 1)
         {
            err = MPI_Isend(myTop, width, MPI_INT, rank - 1, TOP, MPI_COMM_WORLD, &send_request0); 
            err = MPI_Irecv(otherBot, width, MPI_INT, rank - 1, BOT, MPI_COMM_WORLD, &recv_request0);
         }
         else
         {
            err = MPI_Isend(myBot, width, MPI_INT, rank + 1, BOT, MPI_COMM_WORLD, &send_request0); 
            err = MPI_Isend(myTop, width, MPI_INT, rank - 1, TOP, MPI_COMM_WORLD, &send_request1); 
            err = MPI_Irecv(otherTop, width, MPI_INT, rank + 1, TOP, MPI_COMM_WORLD, &recv_request0);
            err = MPI_Irecv(otherBot, width, MPI_INT, rank - 1, BOT, MPI_COMM_WORLD, &recv_request1);
            err = MPI_Wait(&send_request1, &status);
            err = MPI_Wait(&recv_request1, &status);
         }
         err = MPI_Wait(&send_request0, &status);
         err = MPI_Wait(&recv_request0, &status);
      }
      
      //insert game/cuda logic here


      MPI_Barrier(MPI_COMM_WORLD);              
      printf("\nrank %d gen %d myBoard ", rank, i);
      printArray(myBoard, length);

      natural_select(otherBot, otherTop, myBoard, width, length / nprocs);

      //otherBot and otherTop are Bot and Top pieces coming from other nodes. 
      //Will need to use otherBot to calculate game at the top of the current node,
      // and use otherTop to calculate game at the bottom of current node.
      //ask Mike for clarification if this doesn't make sense
   }

   //mpi gather junk
   uint8_t* gameBoard;
   
//   if(rank == 0)
//   {
   gameBoard = (uint8_t*)calloc(numElements, sizeof(uint8_t));
//   }
   
   MPI_Barrier(MPI_COMM_WORLD);

   MPI_Gather(myBoard, length, MPI_INT, gameBoard, length,  MPI_INT, ROOT, MPI_COMM_WORLD);

   //output junk
   printf("made it to outputting!\n");
   saveFrame(gameBoard, numElements, "output.txt"); 
   MPI_Finalize();
   return 0;
}

// Takes a file of 'cells' (one byte ints) and reads them into a 1D array
// Assumes first four bytes contains an int representing the number of 'cells'
// Assumes rank is global to the node reading in the file
int8_t* getGameTile(char *filename) {
   FILE *fp = fopen((char *)filename, "r");

    if (fp) {
        // The first four bytes should contain the number of cells to be read in
        int32_t numElements;
        // lol what's error handling?
        fread(&numElements, 4, 1, fp);
        // Read the elements into a 1D array
        int numElementsInSubarray = numElements / nprocs;
        uint8_t *subArray = (uint8_t *) malloc((numElementsInSubarray + 1) * sizeof(uint8_t));
        fseek(fp, (numElementsInSubarray * rank), SEEK_CUR);
        fread(subArray, 1, numElementsInSubarray, fp);
        printf("Read in %d cells.\n", numElementsInSubarray);

        // Get rid of dat shit
        return subArray;
    }
    return 0;
}

int32_t getDeclaredElementCount(char * filename){
    FILE *fp = fopen((char *)filename, "r");

    if (fp) {
        // The first four bytes should contain the number of cells to be read in
        int32_t numElements;
        fread(&numElements, 4, 1, fp);
        fclose(fp);
        return numElements;
    }

     return 0;
}

/*
void saveFrame(uint8_t *array, int arraySize) {
    for (i = 0; i < arraySize; i++) {
      fwrite(&array[i], 1, 1, fp);
  }
}
*/
// Saves a 2D array of binary states out to a files of single byte unsigned ints
// This function can easily be broked. Please don't breaked it.
void saveFrame(uint8_t *array, int arraySize, char *filename) {
   FILE *fp = fopen(filename, "w");
   int i;
    if (fp) {
        for (i = 0; i < arraySize; i++) {
            fwrite(&array[i], 1, 1, fp);
        }
    }

    fclose(fp);
}

// Assumes the input array represents a square
void printArray(uint8_t *array, int arraySize)
{
  if(arraySize > 10000) {
    fprintf(stderr, "Are you kidding me? You want to print %d little squares?\nI just saved your ass and your buffer.", arraySize);
  }
  else {

    char EMPTY_SQUARE[]  = {0xe2, 0x96, 0xA2, '\0'};
    char FILLED_SQUARE[]  = {0xe2, 0x96, 0xA3, '\0'};

    int i;

    for(i = 0; i < arraySize; i++)
    {
        // Make dem line breaks
        // Make dem assumptions about square arrays
        if(i % ((int)sqrt(arraySize)) == 0) {
          printf("\n");
        }

        if(array[i] == 1) {
          printf("%s ", FILLED_SQUARE);
        }
        else {
          printf("%s ", EMPTY_SQUARE);
        }
    }

    printf("\n");
    fflush(stdout);
  }
}
