#ifndef encryption_H
#define encryption_H

#include <time.h>
#include <stdint.h>

void encryptFile(char* filepath, char* keypath, char* exportpath);

void decryptFile(char* filepath, char* keypath, char* exportpath);

#endif