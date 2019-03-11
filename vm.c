#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

void default_opcode_handler(VM* vm)
{
    printf("Unknown opcode 0x%x\n", vm->registers.ip[0]);
    vm->registers.ip++;
}

void exit_handler(VM* vm)
{
    vm->is_running = 0;
}

void pushi_handler(VM* vm)
{
    uint64_t num = *(uint64_t*)&vm->registers.ip[1];
    vm->registers.sp--;
    vm->registers.sp[0] = num;
    vm->registers.ip += 9;
}

void pushr_handler(VM* vm)
{
    uint64_t reg = vm->registers.raw[vm->registers.ip[1]];
    vm->registers.sp--;
    vm->registers.sp[0] = reg;
    vm->registers.ip += 2;
}

void pop_handler(VM* vm)
{
    uint64_t* reg = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t num = vm->registers.sp[0];
    vm->registers.sp++;
    *reg = num;
    vm->registers.ip += 2;
}

void mov_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 = *reg2;
    vm->registers.ip += 3;
}

void rin_handler(VM* vm)
{
    uint64_t* reg = &vm->registers.raw[vm->registers.ip[1]];
    char in = vm->input_buf[vm->registers.in];
    vm->registers.in++;
    *reg = in;
    vm->registers.ip += 2;
}

void prtc_handler(VM* vm)
{
    uint64_t val = vm->registers.raw[vm->registers.ip[1]];
    putchar(val);
    vm->registers.ip += 2;
}

void inc_handler(VM* vm)
{
    vm->registers.raw[vm->registers.ip[1]]++;
    vm->registers.ip += 2;
}

void dec_handler(VM* vm)
{
    vm->registers.raw[vm->registers.ip[1]]--;
    vm->registers.ip += 2;
}

void sub_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 -= *reg2;
    vm->registers.ip += 3;
}

void add_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 += *reg2;
    vm->registers.ip += 3;
}

void mul_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 *= *reg2;
    vm->registers.ip += 3;
}

void conditional_jump_handlers(VM* vm)
{
    uint8_t opcode = vm->registers.ip[0];
    uint32_t result = 0;
    uint64_t reg1 = vm->registers.raw[vm->registers.ip[1]];
    uint64_t reg2 = vm->registers.raw[vm->registers.ip[2]];
    int64_t target = *(int64_t*)&vm->registers.ip[3];

    switch(opcode)
    {
        case OP_JL:
            result = reg1 < reg2;
            break;
        case OP_JG:
            result = reg1 > reg2;
            break;
        case OP_JE:
            result = reg1 == reg2;
            break;
    }

    if(result)
    {
        if(target >= 0)
            vm->registers.ip += 11;
        vm->registers.ip += target;
    }
    else
    {
        vm->registers.ip += 11;
    }
}

void jmp_handler(VM* vm)
{
    int64_t target = *(int64_t*)&vm->registers.ip[1];
    if(target >= 0)
        vm->registers.ip += 9;
    vm->registers.ip += target;
}

void and_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 &= *reg2;
    vm->registers.ip += 3;
}

void xor_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 ^= *reg2;
    vm->registers.ip += 3;
}

void shl_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 <<= *reg2;
    vm->registers.ip += 3;
}

void shr_handler(VM* vm)
{
    uint64_t* reg1 = &vm->registers.raw[vm->registers.ip[1]];
    uint64_t* reg2 = &vm->registers.raw[vm->registers.ip[2]];
    *reg1 >>= *reg2;
    vm->registers.ip += 3;
}

VM* vm_init(VM* vm, uint8_t bytecode[], uint32_t bytecode_size)
{
    memset(vm, 0, sizeof(VM));

    vm->memory = malloc(VM_MEMORY_SIZE);
    memset(vm->memory, 0, VM_MEMORY_SIZE);
    memcpy(vm->memory, bytecode, bytecode_size);

    vm->input_buf = malloc(VM_INPUT_SIZE);
    memset(vm->input_buf, 0, VM_INPUT_SIZE);

    vm->bytecode_size = bytecode_size;

    vm->registers.ip = vm->memory;
    vm->registers.sp = (uint64_t) vm->memory + VM_MEMORY_SIZE;

    for(uint32_t i = 0; i < 256; i++)
    {
        vm->opcode_handlers[i] = default_opcode_handler;
    }

    vm->opcode_handlers[OP_EXIT] = exit_handler;
    vm->opcode_handlers[OP_PUSHI] = pushi_handler;
    vm->opcode_handlers[OP_PUSHR] = pushr_handler;
    vm->opcode_handlers[OP_POP] = pop_handler;
    vm->opcode_handlers[OP_MOV] = mov_handler;
    vm->opcode_handlers[OP_RIN] = rin_handler;
    vm->opcode_handlers[OP_PRTC] = prtc_handler;
    vm->opcode_handlers[OP_INC] = inc_handler;
    vm->opcode_handlers[OP_DEC] = dec_handler;
    vm->opcode_handlers[OP_SUB] = sub_handler;
    vm->opcode_handlers[OP_ADD] = add_handler;
    vm->opcode_handlers[OP_MUL] = mul_handler;
    vm->opcode_handlers[OP_JL] = conditional_jump_handlers;
    vm->opcode_handlers[OP_JG] = conditional_jump_handlers;
    vm->opcode_handlers[OP_JE] = conditional_jump_handlers;
    vm->opcode_handlers[OP_JMP] = jmp_handler;
    vm->opcode_handlers[OP_AND] = and_handler;
    vm->opcode_handlers[OP_XOR] = xor_handler;
    vm->opcode_handlers[OP_SHL] = shl_handler;
    vm->opcode_handlers[OP_SHR] = shr_handler;

    return vm;
}

void vm_free(VM* vm)
{
    free(vm->memory);
    free(vm->input_buf);
}

uint32_t vm_run(VM* vm)
{
    uint32_t result = 1;
    uint8_t opcode;

    vm->is_running = 1;
    while(vm->is_running)
    {
        opcode = vm->registers.ip[0];
        vm->opcode_handlers[opcode](vm);
    }

    return result;
}
