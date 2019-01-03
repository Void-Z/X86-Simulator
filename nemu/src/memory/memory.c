#include "nemu.h"
int is_mmio(paddr_t addr);
uint32_t mmio_read(paddr_t addr, int len, int map_NO);
void mmio_write(paddr_t addr, int len, uint32_t data, int map_NO);

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
  int map_no = is_mmio(addr);
  if(map_no == -1) {
    return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));  
  } else {
    return mmio_read(addr,len,map_no);
  }
  
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
  int map_no = is_mmio(addr);
  if(map_no == -1) {
    memcpy(guest_to_host(addr), &data, len);
  } else {
    mmio_write(addr,len,data,map_no);
  }
  
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}
