#include "../headers/string_functions.h"

#include <sys/_types/_u_int32_t.h>

u_int32_t str_to_u_int32_t(int start, const char* string) {
    u_int32_t result = 0;
    for (int i = 0; i < sizeof(u_int32_t); i++) {
        int index = start + i;
        result |= ((int) string[index]) << (8 * (sizeof(u_int32_t) - i - 1));
    }
    return result;
}
