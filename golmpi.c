#include <float.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "mpi.h"

#define TOP 0
#define BOT 1

int rank;
int nprocs;

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

   int width = sqrt(numElements);
   int length = numElements / nprocs;
   int myBoard [length];
   int *myTop;//width
   int *myBot;//width
   int otherBot[width];
   int otherTop[width];
   int hasOtherTop = 1;
   int hasOtherBot = 1;
   

   //read data here

   if(rank == 0) 
   {
      hasOtherTop = 0;
   }
   else if(rank == nprocs - 1)
   {
      hasOtherBot = 0;
   }

   int err;
   int i;

   MPI_Status status;
   MPI_Request send_request0, send_request1, recv_request0, recv_request1;

   for(i = 0; i < maxGen; i++)
   {
      //send other top and bottoms around, TOP and BOT relative to sender
      if(rank == 0)
      {
         err = MPI_Isend(myBot, width, MPI_INT, rank + 1, BOT, MPI_COMM_WORLD, send_request0); 
         err = MPI_Irecv(otherTop, width, MPI_INT, rank + 1, TOP, MPI_COMM_WORLD, recv_request0);
      }
      else if(rank == nprocs - 1)
      {
         err = MPI_Isend(myTop, width, MPI_INT, rank - 1, TOP, MPI_COMM_WORLD, send_request0); 
         err = MPI_Irecv(otherBot, width, MPI_INT, rank - 1, BOT, MPI_COMM_WORLD, recv_request0);
      }
      else
      {
         err = MPI_Isend(myBot, width, MPI_INT, rank + 1, BOT, MPI_COMM_WORLD, send_request0); 
         err = MPI_Isend(myTop, width, MPI_INT, rank - 1, TOP, MPI_COMM_WORLD, send_request1); 
         err = MPI_Irecv(otherTop, width, MPI_INT, rank + 1, TOP, MPI_COMM_WORLD, recv_request0);
         err = MPI_Irecv(otherBot, width, MPI_INT, rank - 1, BOT, MPI_COMM_WORLD, recv_request1);
         err = MPI_Wait(&send_request1, &status);
         err = MPI_Wait(&recv_request1, &status);
      }
      err = MPI_Wait(&send_request0, &status);
      err = MPI_Wait(&recv_request0, &status);
      
      //insert game/cuda logic here

      natural_select(otherBot, otherTop, myBoard, width);

      //otherBot and otherTop are Bot and Top pieces coming from other nodes. 
      //Will need to use otherBot to calculate game at the top of the current node,
      // and use otherTop to calculate game at the bottom of current node.
      //ask Mike for clarification if this doesn't make sense
              
   }

   
   //mpi gather junk
   if(rank == 0)
   {
      int gameBoard[numElements];
      MPI_Gather(myBoard, length, MPI_INT, gameBoard, MPI_INT, rank, MPI_COMM_WORLD);
   }

   //output junk

}
