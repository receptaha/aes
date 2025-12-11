#ifndef CIPHER_FUNCTIONS_H
#define CIPHER_FUNCTIONS_H

#include <stdio.h>
#include <sys/_types/_u_int32_t.h>
#include <sys/_types/_u_int8_t.h>

void validate_base_key(const char* base_key);
u_int8_t*** key_expension(const char* base_key);
u_int32_t left_rotation(u_int32_t word, int byte_count);
u_int8_t rc(u_int8_t i);
u_int8_t** g(u_int8_t** state_array, u_int8_t round_number);
u_int8_t subByte(u_int8_t byte);
void encrpyt_state_array(u_int8_t*** round_keys, u_int8_t** state_array, FILE* output_f);
void encrypt(u_int8_t*** round_keys, char* input_file, char* output_file);


#endif
