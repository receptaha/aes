#include <stdio.h>

#include "headers/cipher_functions.h"
#include "headers/constants.h"

int main() {
    validate_base_key(BASE_KEY);
    u_int32_t** round_keys = key_expension(BASE_KEY);
    return 0;
}