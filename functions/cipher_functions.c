#include "../headers/cipher_functions.h"
#include "../headers/constants.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void validate_base_key(const char* base_key) {
    unsigned long long len = strlen(base_key);
    u_int8_t valid = 0;

    for (int i = 0; i < 3; i++){
        if (len * 8 == KEY_SIZES[i]) valid = 1;
    }

    if (valid == 0) {
        printf("Invalid key size");
        exit(-1);
    }
}

u_int8_t subByte(u_int8_t byte) {
    u_int8_t row = byte >> 4;
    u_int8_t column = byte << 4;
    return s_box_encrypt[row][column >> 4];
}
