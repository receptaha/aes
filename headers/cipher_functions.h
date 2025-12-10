#ifndef CIPHER_FUNCTIONS_H
#define CIPHER_FUNCTIONS_H
#include <sys/_types/_u_int32_t.h>
#include <sys/_types/_u_int8_t.h>

void validate_base_key(const char* base_key);
u_int8_t subByte(u_int8_t byte);
u_int32_t** key_expension(const char* base_key);


#endif
