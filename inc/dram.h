
#define MM_LEN 1024

#include <stdint.h>
extern uint8_t mem[MM_LEN];
uint64_t va2pa(uint64_t vaddr);

uint64_t read64bits_dram(uint64_t paddr);
void write64bits_dram(uint64_t paddr, uint64_t data);

void print_register();

void print_stack();