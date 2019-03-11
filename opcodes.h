#pragma once

#define I64(num) num & 0xff, (num >> 8) & 0xff, (num >> 16) & 0xff, (num >> 24) & 0xff, (num >> 32) & 0xff, (num >> 40) & 0xff, (num >> 48) & 0xff, (num >> 56) & 0xff
#define I(num) I64(num ## ll)
#define IC(c) c, 00, 00, 00, 00, 00, 00, 00

// both of these are 11 bytes total
#define LOAD_REG(reg, val) OP_PUSHI, I(val), OP_POP, reg
#define LOADC_REG(reg, val) OP_PUSHI, IC(val), OP_POP, reg

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define IN 5
#define SP 6
#define IP 7

#define OPCODE_BASE 0x42 // not used

#define OP_EXIT   0xff // stops the VM and returns from vm_run()
#define OP_PUSHI  0x01 // push a 64 bit immediate onto the stack
#define OP_PUSHR  0x02 // push a register onto the stack
#define OP_POP    0x03 // pop a value off the stack into a register
#define OP_MOV    0x04 // move a register value into another register
#define OP_RIN    0x05 // move a byte from the input buffer into a register, inc in
#define OP_PRTC   0x06 // print a character
#define OP_INC    0x07 // increment a register
#define OP_DEC    0x08 // decrement a register
#define OP_SUB    0x09 // subtract two registers
#define OP_ADD    0x0a // add two registers
#define OP_MUL    0x0b // multiply two registers
#define OP_JL     0x0c // jump if reg1 < reg2
#define OP_JG     0x0d // jump if reg1 > reg2
#define OP_JE     0x0e // jump if reg1 == reg2
#define OP_JMP    0x0f // jump unconditionally
#define OP_XOR    0x10 // xor
#define OP_AND    0x11 // and
#define OP_SHL    0x12
#define OP_SHR    0x13