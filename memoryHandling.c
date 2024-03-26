#include "help.c"

uint8_t* allocateList(int size);

uint8_t** initializeMatrix(int n, int m);

void clear(uint8_t** A, int width);

long bytesToTotalChunks(long bytes);

uint8_t* allocateList(int size){
    uint8_t* A = (uint8_t*)malloc(size * sizeof(uint8_t));
    return A;
}

uint8_t** initializeMatrix(int n, int m){
    uint8_t** state = (uint8_t**)malloc(n * sizeof(uint8_t*));
    
    for(int i = 0; i < n; i++){
        state[i] = (uint8_t*)malloc(m * sizeof(uint8_t));
    }

    return state;
}

void empty(uint8_t* A){
    free(A);
}

void clear(uint8_t** A, int width){
    for(int i = 0; i < width; i++){
        free(A[i]);
    }
    free(A);
}

long bytesToTotalChunks(long bytes){
    int num_chunks = bytes / 16;
    if(bytes % 16 != 0){ num_chunks++; }
    return num_chunks;
}