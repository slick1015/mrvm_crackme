#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include "vm.h"

// flag == n3at0vm!

uint8_t sample_bytecode[] = {
    OP_RIN, R1,
    LOADC_REG(R3, '\n'),
    OP_JE, R1, R3, I(26),
    LOAD_REG(R4, 8),
    OP_SHL, R2, R4,
    OP_ADD, R2, R1,
    OP_JMP, I(-41),
    LOAD_REG(R4, 0x0B4DBAB3D3ADB3EF),
    OP_XOR, R2, R4,
    LOAD_REG(R4, 0x657edbc7e3dbdece),
    OP_JE, R2, R4, I(1),
    OP_EXIT,
    LOAD_REG(R0, 1),
    OP_EXIT
};

int main(int argc, char* argv[])
{
    VM vm;
    vm_init(&vm, sample_bytecode, sizeof(sample_bytecode));

    printf("> ");
    fgets(vm.input_buf, VM_INPUT_SIZE-1, stdin);

    vm_run(&vm);

    if(vm.registers.r0)
        printf("\nsuccess\n");

    vm_free(&vm);

    return 0;
}