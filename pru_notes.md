# Notes on using the PRU

## Communicating between the ARM core and PRU
### Shared memory
This is now librobotcontrol works. Shared memory is allocated - see _rc_pru_shared_mem_ptr()_ in library/src/pru/pru.c - and can be accessed by both. Note that _rc_servo_init()_ lets the PRU zero this out and checks that it has done so.

Reading and writing from the ARM side seems simple enough. On the PRU side, here's only an asm example of using PRU_SHAREDMEM (defined in AM335x_PRU.cmd) so my have to look for a C version! It's probably simple when you see it.
This looks good - and is as simple as expected: https://catch22eu.github.io/website/beaglebone/beaglebone-pru-ipc/

Also, it seems that accessing shared memory requires running as root on the Linux side, so maybe it's not ideal.

On the Linux side, work out what the memory location is and mmap to it.
~~~
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

#define SERVO_IDENTIFIER 0x7974

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
~~~

On the PRU side, just read or write the memory. A struct is a useful way of structuring the memory.
~~~
#define THIS_PRU_DRAM 0x00000
// Skip 0x100 for Stack, 0x100 for Heap (from makefile)
#define OFFSET 0x200
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

#define SERVO_IDENTIFIER 0x7974

volatile servo_shared_mem_struct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;

void main(void)
{

    ...

    // Check it's not just some random bytes in memory
    if (pSharedMemStruct->identifier == SERVO_IDENTIFIER) {
        // Read from shared memory
        width = pSharedMemStruct->pwm1;
    }

    ...
}
~~~

## rpmsg
This is used in the PRU training examples. The echo code runs on the PRU, but does not seem to create the /dev/rpmsg_pru31 file that's used for communication. Maybe there's something else needed. See https://software-dl.ti.com/processor-sdk-linux/esd/docs/08_00_00_21/common/PRU-ICSS/PRU-Hands-on-Labs.html#lab-5-rpmsg-communication-between-arm-and-pru

To try: Downgrading from Debian 10.13 to the last official build of 10.3
