cmake_minimum_required(VERSION 3.24)
project(Maman14 C)

set(CMAKE_C_STANDARD 90)

add_executable(Maman14 error_print.c pre_assembler.c hash_table.c error_print.h hash_table.h ../constants.h input_validation.c input_validation.h functions.c functions.h validate_file.c validate_file.h)
