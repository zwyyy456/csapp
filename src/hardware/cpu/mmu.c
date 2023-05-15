// Memory Management Unit
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cpu.h"
#include "memory.h"
#include "common.h"

uint64_t va2pa(uint64_t vaddr, core_t *cr) {
    return vaddr & (0xffffffffffffffff >> (64 - MAX_INDEX_PHYSICAL_PAGE));
}