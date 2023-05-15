#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct CPU_STRUCT {
    union {
        struct {
            uint8_t al;
            uint8_t ah;
        };
        uint16_t ax;
        uint32_t eax;
        uint64_t rax;
    };
    union {
        struct {
            uint8_t bl;
            uint8_t bh;
        };
        uint16_t bx;
        uint32_t ebx;
        uint64_t rbx;
    };

    uint64_t rcx;
    uint64_t rdx; // 这里其实表示了寄存器里的值
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;

    uint64_t rip; // 指向 program counter
} reg_t;

extern reg_t reg;