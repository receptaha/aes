#include "../headers/file_functions.h"

#include <stdio.h>
#include <stdlib.h>

FILE* find_file_or_fail(char* file) {
    FILE* f = fopen(file, "rb");
    if (f == NULL) {
        printf("\nFile cannot opened");
        exit(-1);
    }
    return f;
}
