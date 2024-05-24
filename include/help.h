#ifndef help_H
#define help_H

#define SBOX_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

extern uint8_t* inverse_aes_box;

extern uint8_t aes_sbox[];

extern uint8_t mixing_matrix[][4];

extern uint8_t reverse_mixing_matrix[][4];

extern void printState(uint8_t** state);

extern void printBytes(uint8_t* bytes, int size);

extern uint8_t xTimes(uint8_t a, uint8_t b);

#endif