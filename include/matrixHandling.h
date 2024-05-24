#ifndef matrixHandling_H
#define matrixHandling_H

#include <stdint.h>
#include <stdlib.h>

void stateToChunk(uint8_t** state, uint8_t* chunk);

void bytesToChunks(uint8_t* bytes, uint8_t** chunks, long num_chunks);

void chunkToState(uint8_t* chunk, uint8_t** state);

void rotWord(uint8_t* A);

uint8_t* chunksToBytes(uint8_t** chunks, long num_chunks);

#endif