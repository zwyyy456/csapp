#include "mmu.h"
#include "dram.h"

uint64_t va2pa(uint64_t vadrr) {
    return vadrr % MM_LEN;
}