#include "dram.h"
#include "register.h"
#include <stdint.h>
#include <stdio.h>

#define SRAM_CACHE_SETTING 0

uint64_t read64bits_dram(uint64_t paddr) {
    if (SRAM_CACHE_SETTING == 1) {
        return 0x0; // 此时开启 SRAM
    }
    uint64_t val = 0x0;
    val += (((uint64_t)mem[paddr + 0]) << 0);
    val += (((uint64_t)mem[paddr + 1]) << 8);
    val += (((uint64_t)mem[paddr + 2]) << 16);
    val += (((uint64_t)mem[paddr + 3]) << 24);
    val += (((uint64_t)mem[paddr + 4]) << 32);
    val += (((uint64_t)mem[paddr + 5]) << 40);
    val += (((uint64_t)mem[paddr + 6]) << 48);
    val += (((uint64_t)mem[paddr + 7]) << 56);
    return val;
}
void write64bits_dram(uint64_t paddr, uint64_t data) {
    if (SRAM_CACHE_SETTING == 1) {
        return;
    }
    mem[paddr + 0] = (data >> 0) & 0xff;
    mem[paddr + 1] = (data >> 8) & 0xff;
    mem[paddr + 2] = (data >> 16) & 0xff;
    mem[paddr + 3] = (data >> 24) & 0xff;
    mem[paddr + 4] = (data >> 32) & 0xff;
    mem[paddr + 5] = (data >> 40) & 0xff;
    mem[paddr + 6] = (data >> 48) & 0xff;
    mem[paddr + 7] = (data >> 56) & 0xff;
}

void print_register() {
    printf("rax = %16lx\trbx = %16lx\trcx = %16lx\trdx = %16lx\n",
           reg.rax, reg.rbx, reg.rcx, reg.rdx);
    printf("rsi = %16lx\trdi = %16lx\trbp = %16lx\trsp = %16lx\n",
           reg.rsi, reg.rdi, reg.rbp, reg.rsp);
    printf("rip = %16lx\n", reg.rip);
}

void print_stack() {
    int n = 10;

    uint64_t *high = (uint64_t *)&mem[va2pa(reg.rsp)]; // 栈顶
    high = &high[n];                                   // 相当于往前取了 10 位

    uint64_t rsp_start = reg.rsp + n * 8;

    for (int i = 0; i < 2 * n; ++i) {
        uint64_t *ptr = (uint64_t *)(high - i);
        printf("0x%016lx : %16lx", rsp_start, (uint64_t)*ptr);

        if (i == n) {
            printf(" <== rsp");
        }

        rsp_start = rsp_start - 8;

        printf("\n");
    }
}