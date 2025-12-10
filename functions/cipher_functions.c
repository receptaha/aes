#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../headers/cipher_functions.h"
#include "../headers/constants.h"
#include "../headers/string_functions.h"


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

u_int32_t** key_expension(const char* base_key) {
    unsigned long long base_key_len = strlen(base_key); // possible values : 16, 24, 32
    int word_count_per_round = base_key_len / sizeof(u_int32_t); // possible values : 4, 6, 8
    int round_count = 0;

    // Round count setting.
    for (int i = 0; i < 3; i++) {
        if (base_key_len * 8 == KEY_SIZES[i]) round_count = ROUNDS[i];
    }

    if (round_count == 0) {
        printf("Invalid round count");
        exit(-1);
    }

    u_int32_t** round_keys = malloc(sizeof(u_int32_t*) * round_count);
    if (round_keys == NULL) {
        printf("Round keys cannot allocated");
        exit(-1);
    }
    for (int i = 0; i < round_count; i++) {
        round_keys[i] = malloc(sizeof(u_int32_t) * word_count_per_round);
        if (round_keys[i] == NULL) {
            printf("Round keys cannot allocated");
            exit(-1);
        }
    }

    // Original key is placing to first row of the round keys
    for (int i = 0; i < word_count_per_round; i++) {
        round_keys[0][i] = str_to_u_int32_t(sizeof(u_int32_t) * i, base_key);
    }

    // Other keys are placing to other rows of the rounds keys
    // for (int i = 1; i <= round_count; i++) {
    //     for (int j = 0; j < word_count_per_round; j++) {
    //         if (j == 0) {
    //             round_keys[i][j] = round_keys[i-1][0] | g(round_keys[i-1][3]);
    //         }else {
    //             round_keys[i][j] = round_keys[i][j-1] | round_keys[i-1][j];
    //         }
    //     }
    // }
    return round_keys;
}

u_int8_t subByte(u_int8_t byte) {
    u_int8_t row = byte >> 4;
    u_int8_t column = byte << 4;
    return s_box_encrypt[row][column >> 4];
}


