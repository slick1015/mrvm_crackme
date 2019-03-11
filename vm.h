#pragma once

#include <stdint.h>
#include "opcodes.h"

#define VM_MEMORY_SIZE 8 * 1024
#define VM_INPUT_SIZE 8 * 1024

struct _VM;
typedef void (*opcode_handler)(struct _VM* vm);

typedef struct _VM {
    union {
        struct {
            uint64_t r0;
            uint64_t r1;
            uint64_t r2;
            uint64_t r3;
            uint64_t r4;
            uint64_t in;
            uint64_t* sp;
            uint8_t* ip;
        };
        uint64_t raw[8];
    } registers;
    uint8_t* memory;
    char* input_buf;
    uint32_t bytecode_size;
    opcode_handler opcode_handlers[256];
    uint32_t is_running;
} VM;

VM* vm_init(VM* vm, uint8_t bytecode[], uint32_t bytecode_size);
void vm_free(VM* vm);
uint32_t vm_run(VM* vm);