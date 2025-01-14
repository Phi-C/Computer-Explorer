// Copyright 2024 AI Inc. All rights reserved.
// Author: Chen Chen (chenxj@163.com)
// Print initialization info in process stack
// Compile: g++ -o explore_process_stack explore_process_stack.cpp

#include <elf.h>
#include <stdio.h>

int main(int argc, char* argv[], char* envp[]) {
    int i = 0;

    printf("Argument count: %d\n", argc);
    for (i = 0; i < argc; ++i) {
        printf("Argument %d: %s\n", i, argv[i]);
    }

    printf("Environment:\n");
    for (i = 0; envp[i] != NULL; ++i) {
        printf("envp[%d]: %s\n", i, envp[i]);
    }

    // Print ELF auxiliary vector
    printf("Auxiliary vector:\n");
    Elf32_auxv_t* aux =
        (Elf32_auxv_t*)(envp + i + 1); // Skip the NULL pointer of envp
    while (aux->a_type != AT_NULL) {
        printf("Type: %02d, Value: %x\n", aux->a_type, aux->a_un.a_val);
        aux++;
    }

    return 0;
}