#ifndef fileHandling_H
#define fileHandling_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void writeTextToFile(uint8_t* text, int size, char* filepath);

void writeBytesToFile(uint8_t* bytes, size_t size, char* filename);

void fileToBytes(char* filepath, uint8_t* bytes);

void createFile(char* filepath, uint64_t size);

void checkFiles(char* file1, char* file2);

void exportKey(uint8_t* key, int key_length, char* filepath);

long getBytesInFile(char* filepath);

uint8_t* readKeyFromFile(char* keypath, int bytes_in_key);

void fileToChunks(char* filepath, uint8_t*** chunks);

void writeChunksToFile(uint8_t*** chunks, long num_bytes, char* filepath);

#endif