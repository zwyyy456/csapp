#include "instruction.h"
#include "dram.h"
#include "mmu.h"
#include "register.h"
#include <stdint.h>

handler_t handler_table[NUM_INST_TYPE] = {
    &mov_reg_reg_handler,
    &add_reg_reg_handler};
static uint64_t decode_od(od_t od) {
    if (od.type == IMM) {
        return *((uint64_t *)&od.imm);
    } else if (od.type == REG) {
        return (uint64_t)od.reg1;
    } else {
        // mem
        uint64_t vaddr = 0;
        if (od.type == MM_IMM) {
            vaddr = od.imm;
        } else if (od.type == MM_REG) {
            vaddr = *(od.reg1);
        } else if (od.type == MM_IMM_REG) {
            vaddr = od.imm + *(od.reg1);
        } else if (od.type == MM_REG1_REG2) {
            vaddr = *(od.reg1) + *(od.reg2);
        } else if (od.type == MM_IMM_REG1_REG2) {
            vaddr = od.imm + *(od.reg1) + *(od.reg2);
        } else if (od.type == MM_REG2_S) {
            vaddr = *(od.reg2) * od.scale;
        } else if (od.type == MM_IMM_REG2_S) {
            vaddr = od.imm + *(od.reg2) * od.scale;
        } else if (od.type == MM_REG1_REG2_S) {
            vaddr = od.imm + *(od.reg1) + *(od.reg2) * od.scale;
        }
        return vaddr;
    }
}

void init_handler_table() {
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[call] = &call_handler;
    handler_table[add_reg_reg] = &add_reg_reg_handler;
    handler_table[push_reg] = &push_reg_handler;
    handler_table[pop_reg] = &pop_reg_handler;
    handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    handler_table[ret] = &ret_handler;
}
void instruction_cycle() {
    inst_t *inst = (inst_t *)reg.rip;

    uint64_t src = decode_od(inst->src);
    uint64_t dst = decode_od(inst->dst);

    printf("%d\n", inst->op);
    handler_t handler = handler_table[inst->op];
    handler(src, dst);

    printf("   %s\n", inst->code);
}

void add_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t *)dst = *(uint64_t *)dst + *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t); // 为什么加上 sizeof(inst_t)？
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t *)dst = *(uint64_t *)src;
    reg.rip = reg.rip + sizeof(inst_t);
}

void mov_reg_mem_handler(uint64_t src, uint64_t dst) {
    write64bits_dram(va2pa(dst), *(uint64_t *)src);
}

void mov_mem_reg_handler(uint64_t src, uint64_t dst) {
    *(uint64_t *)dst = read64bits_dram(va2pa(src));

    reg.rip = reg.rip + sizeof(inst_t);
}

void call_handler(uint64_t src, uint64_t dst) {
    // src: imm address of called funciton
    reg.rsp = reg.rsp - 8;
    write64bits_dram(va2pa(reg.rsp), reg.rip + sizeof(inst_t));
    reg.rip = src;
}

void push_reg_handler(uint64_t src, uint64_t dst) {
    reg.rsp = reg.rsp - 8;
    write64bits_dram(va2pa(reg.rsp), *(uint64_t *)src);
    reg.rip = reg.rip + sizeof(inst_t);
}

void pop_reg_handler(uint64_t src, uint64_t dst) {
    uint64_t old_val = read64bits_dram(va2pa(reg.rsp));
    reg.rsp += 8;
    reg.rip += sizeof(inst_t);
    *(uint64_t *)src = old_val;
}

void ret_handler(uint64_t src, uint64_t dst) {
    uint64_t ret_addr = read64bits_dram(va2pa(reg.rsp));
    reg.rip = ret_addr;
    reg.rsp += 8;
}