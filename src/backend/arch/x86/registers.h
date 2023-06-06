#ifndef BACKEND_REGISTERS_H
#define BACKEND_REGISTERS_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

struct x86_16_registers {
    uint16_t ax;
    uint16_t bx;
    uint16_t cx;
    uint16_t dx;
    uint16_t sp;
    uint16_t bp;
    uint16_t si;
    uint16_t di;
    uint16_t cs;
    uint16_t ds;
    uint16_t ss;
    uint16_t es;
    uint16_t ip;
    uint16_t flags;
};


struct x86_32_registers {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
    uint32_t eflags;
    struct x86_16_registers i8086;
};

struct x86_64_registers {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    struct x86_32_registers i386;
};

/* vector extensions not added */
#endif /* BACKEND_REGISTERS_H */
