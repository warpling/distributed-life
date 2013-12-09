#include <stdint.h>

#include "cutil.h"
#include "life.h"

__global__ void life(uint8_t *grid, uint8_t *output_grid, int width,
      int height);

extern "C" void natural_select(uint8_t *top, uint8_t *bot, uint8_t *grid,
      int width, int height) {
   uint8_t *dev_input, *dev_output;

   CUDA_SAFE_CALL(cudaMalloc((void **)&dev_input,
            ((2 * width) + (width * height)) * sizeof(uint8_t)));
   CUDA_SAFE_CALL(cudaMalloc((void **)&dev_output,
            ((2 * width) + (width * height)) * sizeof(uint8_t)));

   CUDA_SAFE_CALL(cudaMemcpy(dev_input, top, width * sizeof(uint8_t), TO_DEV));
   CUDA_SAFE_CALL(cudaMemcpy(dev_input + width, grid,
            width * height * sizeof(uint8_t), TO_DEV));
   CUDA_SAFE_CALL(cudaMemcpy(dev_input + (width * height) + width, bot,
            width * sizeof(uint8_t), TO_DEV));

   life<<<MAX_BLOCKS, THREADS_PER_BLOCK>>>(dev_input, dev_output, width,
            height);

   CUDA_SAFE_CALL(cudaMemcpy(grid, dev_output + width,
            width * height * sizeof(uint8_t), TO_HOST));

   cudaFree(dev_input);
   cudaFree(dev_output);
}

__global__ void life(uint8_t *grid, uint8_t *output_grid, int width,
      int height) {
   // Find the focus cell, ignore the first row
   int cell_id = width + blockIdx.x * blockDim.x + threadIdx.x;
   uint8_t live_neighbors;
   int row_idx, col_idx, neighbor_id;

   while (cell_id < (width * height) + width) {

      // Iterate through neighboring blocks and count those which are alive
      live_neighbors = 0;
      for (row_idx = 0; row_idx < 3; row_idx++) {
         for (col_idx = 0; col_idx < 3; col_idx++) {

            /*
             * Find the current neighbor. Ignore if it is out of bounds or
             * the focus cell.
             */
            neighbor_id = (cell_id + col_idx - 1) + (width * (row_idx - 1));
            if (neighbor_id < 0 || neighbor_id == cell_id
                  || neighbor_id >= (width * height) + (2 * width)) {
               continue;
            }

            // Increment the count of living neighbors
            live_neighbors += grid[neighbor_id];
         }
      }

      // If 2 or 3 neighbors are alive, the focus cell lives, else, it dies
      output_grid[cell_id] = (live_neighbors == 2 || live_neighbors == 3) ? 1 : 0;

      // Move to the next focus cell.
      cell_id += blockDim.x + gridDim.x;
   }
}
