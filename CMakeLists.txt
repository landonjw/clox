cmake_minimum_required(VERSION 3.24)
project(clox C)

set(CMAKE_C_STANDARD 11)

add_executable(clox main.c common.h chunk.h chunk.c memory.h memory.c debug.h debug.c value.h value.c vm.h int_conversion.h int_conversion.c line_tracker.h line_tracker.c vm.c compiler.h compiler.c scanner.h scanner.c)
