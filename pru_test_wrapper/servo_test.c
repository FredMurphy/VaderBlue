#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "servo_shared_mem_struct.h"

#define PRU_ADDR 0x4A300000 // Start of PRU memory Page 184 am335x TRM
#define PRU_LEN 0x80000 // Length of PRU memory
#define PRU0_DRAM 0x00000 // Offset to DRAM
#define PRU1_DRAM 0x02000
#define PRU_SHAREDMEM 0x10000 // Offset to shared memory
#define PRU_MEM_RESERVED 0x200 // Amount used by stack and heap

// Convert base address to each memory section
#define PRU0_MEM_FROM_BASE(base) ( (base) + PRU0_DRAM + PRU_MEM_RESERVED)
#define PRU1_MEM_FROM_BASE(base) ( (base) + PRU1_DRAM + PRU_MEM_RESERVED)
#define PRUSHARED_MEM_FROM_BASE(base) ( (base) + PRU_SHAREDMEM)

// Return the address of the PRU's base memory
volatile void* getPruMmapAddr(void) {
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }
    // Points to start of PRU memory.
    volatile void* pPruBase = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);
    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase){
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]){
    
    printf("Sharing memory with PRU 1\n");
    // Get access to shared memory for my uses
    volatile void *pPruBase = getPruMmapAddr();
    volatile servo_shared_mem_struct_t *pSharedPru0 = (servo_shared_mem_struct_t *)PRU1_MEM_FROM_BASE(pPruBase);

    printf("Setting duty cycle to 20 percent\n");
    // Set to 20% duty cycle
    pSharedPru0->identifier = SERVO_IDENTIFIER;
    pSharedPru0->pwm1 = 200;
    pSharedPru0->pwm2 = 300;

    printf("Press enter to quit\n");  
    getchar();  

    // Cleanup
    freePruMmapAddr(pPruBase);
    printf("Done\n");
}