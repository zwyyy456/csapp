#include "instruction.h"
#include "register.h"
#include "dram.h"
#include <stdint.h>
#include <stdio.h>

// virtual memory
uint8_t mem[MM_LEN];
reg_t reg;
int main() {
    init_handler_table();
    // init
    reg.rax = 0x12340000;
    reg.rbx = 0x0;
    reg.rcx = 0x800660;
    reg.rdx = 0xabcd;
    reg.rsi = 0x7ffffffee2f8;
    reg.rdi = 0x1;
    reg.rbp = 0x7ffffffee210;
    reg.rsp = 0x7ffffffee1f0;

    reg.rip = (uint64_t)&program[11];

    write64bits_dram(va2pa(0x7ffffffee210), 0x08000660); // rbp
    write64bits_dram(va2pa(0x7ffffffee208), 0x0);
    write64bits_dram(va2pa(0x7ffffffee200), 0xabcd);
    write64bits_dram(va2pa(0x7ffffffee1f8), 0x12340000);
    write64bits_dram(va2pa(0x7ffffffee1f0), 0x08000660); // rsp

    print_register();
    print_stack();

    for (int i = 0; i < 15; ++i) {
        instruction_cycle();
        print_register();
        print_stack();
    }

    return 0;
}
