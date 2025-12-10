cmake_minimum_required(VERSION 3.10)
project(aes)

set(CMAKE_C_STANDARD 11)

include_directories(headers)

add_executable(main
        main.c
        functions/cipher_functions.c
        functions/file_functions.c
        functions/constants.c
)