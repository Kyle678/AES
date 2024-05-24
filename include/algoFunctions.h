#ifndef algoFunctions_H
#define algoFunctions_H

#include <stdint.h>

void cipher(uint8_t** state, int Nr, uint8_t** w);

void invCipher(uint8_t** state, int Nr, uint8_t** w);

void mixColumns(uint8_t** state);

void invMixColumns(uint8_t** state);

void shiftRows(uint8_t** state);

void invShiftRows(uint8_t** state);

void subBytes(uint8_t* byte);

void subWord(uint8_t* bytes);

void subState(uint8_t** state);

void invSubState(uint8_t** state);

void addRoundKey(uint8_t** state, uint8_t** round_key);

void generateRoundConstants(uint8_t* Rcon, int Nr);

void generateInverseSBox();

void generateKey(uint8_t* key, int Nc);

void keyExpansion(uint8_t* key, uint8_t** round_keys, int Nk, int Nr);

void exportNewKey(int key_length, char* filepath);

long removePadding(uint8_t* bytes, int bytes_in_file);

uint8_t* getNewKey(int key_length);

#endif