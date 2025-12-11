#include <stdio.h>

#include "headers/cipher_functions.h"
#include "headers/constants.h"

int main() {
    validate_base_key(BASE_KEY);
    u_int8_t*** round_keys = key_expension(BASE_KEY);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%c\t", round_keys[0][i][j]);
        }
        printf("\n");
    }
    return 0;
}