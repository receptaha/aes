#include <string.h>
#include <stdlib.h>
#include "../headers/cipher_functions.h"
#include "../headers/constants.h"
#include "../headers/file_functions.h"


void validate_base_key(const char* base_key) {
    unsigned long long len = strlen(base_key);
    u_int8_t valid = 0;

    for (int i = 0; i < sizeof(KEY_SIZES) / sizeof(KEY_SIZES[0]); i++){
        if (len * 8 == KEY_SIZES[i]) valid = 1;
    }

    if (valid == 0) {
        printf("Invalid key size");
        exit(-1);
    }
}

u_int8_t*** key_expension(const char* base_key) {
    unsigned long long base_key_len = strlen(base_key); // possible values : 16, 24, 32
    int word_count_per_round = base_key_len / sizeof(u_int32_t); // possible values : 4, 6, 8
    int round_count = 0;

    // Round count setting.
    for (int i = 0; i < sizeof(KEY_SIZES) / sizeof(KEY_SIZES[0]); i++) {
        if (base_key_len * 8 == KEY_SIZES[i]) round_count = ROUNDS[i];
    }

    if (round_count == 0) {
        printf("Invalid round count");
        exit(-1);
    }

    u_int8_t*** round_keys = malloc(sizeof(u_int8_t**) * (round_count + 1));
    if (round_keys == NULL) {
        printf("Round keys cannot allocated");
        exit(-1);
    }

    for (int i = 0; i < round_count + 1; i++) {
        round_keys[i] = malloc(sizeof(u_int8_t*) * 4);
        if (round_keys[i] == NULL) {
            printf("Round keys cannot allocated");
            exit(-1);
        }
        for (int j = 0; j < 4; j++) {
            round_keys[i][j] = malloc(sizeof(u_int8_t) * word_count_per_round);
            if (round_keys[i][j] == NULL) {
                printf("Round keys cannot allocated");
                exit(-1);
            }
        }
    }

    // Original key is placing
    for (int column = 0; column < word_count_per_round; column++) {
        for (int row = 0; row < 4; row++) {
            round_keys[0][row][column] = (u_int8_t) base_key[4 * column + row];
        }
    }
    // Other keys are placing to other rows of the rounds keys
    for (int round = 1; round <= round_count; round++) {
        for (int column = 0; column < word_count_per_round; column++) {
            for (int row = 0; row < 4; row++) {
                if (column == 0) {
                    u_int8_t ** g_ = g(round_keys[round-1], round);
                    round_keys[round][row][column] = round_keys[round-1][row][column] ^ g_[row][word_count_per_round - 1];
                    free(g_);
                }else {
                    round_keys[round][row][column] = round_keys[round-1][row][column] ^ round_keys[round][row][column-1];
                }
            }
        }
    }
    return round_keys;
}

u_int8_t rc(u_int8_t i) {
    if (i == 1) return 0x01;

    u_int8_t prev_rc = rc(i-1);
    u_int8_t result = prev_rc << 1;

    if (prev_rc & 0x80) { // if the last bit is 1
       result ^= irreducible_polynom;
    }

    return result;
}

u_int8_t** g(u_int8_t** state_array, u_int8_t round_number) {

    int word_count_per_round = strlen(BASE_KEY) / sizeof(u_int32_t); // possible values : 4, 6, 8

    u_int8_t** state_array_copy = malloc(sizeof(u_int8_t*) * 4);
    if (state_array_copy == NULL) exit(-1);
    for (int i = 0; i < 4; i++) {
        state_array_copy[i] = malloc(sizeof(u_int8_t) * word_count_per_round);
        if (state_array_copy[i] == NULL) exit(-1);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < word_count_per_round; j++) {
            state_array_copy[i][j] = state_array[i][j];
        }
    }

    int last_word_index = word_count_per_round - 1;
    for (int i = 0; i < 3; i++) {
        u_int8_t temp = state_array_copy[i][last_word_index];
        state_array_copy[i][last_word_index] = state_array_copy[(i-1) % 4][last_word_index];
        state_array_copy[(i-1) % 4][last_word_index] = temp;
    }

    u_int8_t round_constant = rc(round_number);

    for (int i = 0; i < 4; i++) {
        state_array_copy[i][last_word_index] = subByte(state_array_copy[i][last_word_index]);
        if (i == 0) state_array_copy[i][last_word_index] ^= round_constant;
        else state_array_copy[i][last_word_index] ^= 0x00;
    }

    return state_array_copy;
}

u_int8_t subByte(u_int8_t byte) {
    u_int8_t row = byte >> 4;
    u_int8_t column = byte << 4;
    column >>= 4;
    return s_box_encrypt[row][column];
}

void encrpyt_state_array(u_int8_t*** round_keys, u_int8_t** state_array, FILE* output_f) {
    // elimde bir state array var.

}

void encrypt(u_int8_t*** round_keys, char* input_file, char* output_file) {
    FILE* input_f = find_file_or_fail(input_file);
    rewind(input_f);

    FILE* output_f = fopen(output_file, "wb");
    rewind(output_f);
    u_int8_t readed_byte;
    u_int64_t counter = 0;

    u_int8_t** state_array = malloc(sizeof(u_int8_t*) * 4);
    if (state_array == NULL) {
        printf("\nState array cannot allocated");
        exit(-1);
    }

    for (int i = 0; i < 4; i++) {
        state_array[i] = malloc(sizeof(u_int8_t) * 4);
        if (state_array[i] == NULL) {
            printf("\nState array cannot allocated");
            exit(-1);
        }
    }

    while (fread(&readed_byte, sizeof(u_int8_t), 1, input_f) > 0) {
        state_array[counter % 4][counter / 4] = readed_byte;
        counter++;
        if (counter % 16 == 0) { // state array is ready to encrypt.
            counter = 0;
            encrpyt_state_array(round_keys, state_array, output_f);
        }
    }
}


