#ifndef CONSTANTS_H
#define CONSTANTS_H

const int BLOCK_SIZE;
const int KEY_SIZES[3];
const int ROUNDS[3];
const char* BASE_KEY;
const u_int8_t irreducible_polynom = 0x1B;
const u_int8_t s_box_encrypt[16][16];
#endif