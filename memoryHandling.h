#ifndef memoryHandling_H
#define memoryHandling_H

#include <stdint.h>

void clear(uint8_t** A, int width);

long bytesToTotalChunks(long bytes);

uint8_t* allocateList(int size);

uint8_t** initializeMatrix(int n, int m);

void clearChunks(uint8_t*** chunks, long num_chunks);

#endif