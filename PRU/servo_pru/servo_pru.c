/*
 * servo_pru.c
 *
 *  Created on: 18 Sep 2023
 *      Author: Fred
 */

#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table.h"
#include "servo_shared_mem_struct.h"

volatile register uint32_t __R30;
//volatile register uint32_t __R31;

#define servo1_bit (1<<8)
#define servo2_bit (1<<10)
#define servo3_bit (1<<9)
#define servo4_bit (1<<11)
#define servo5_bit (1<<6)
#define servo6_bit (1<<7)
#define servo7_bit (1<<4)
#define servo8_bit (1<<5)

// 20us @ 200MHz = 4000
// Minus manual trial-and-error adjustment for code cycles
#define delay (4000 - 25)

// Shared Memory Configuration
// -----------------------------------------------------------
// Address of DRAM. This works for both PRU0 and PRU1 as both map their own memory to 0x0000000.

#define THIS_PRU_DRAM 0x00000
// Skip 0x100 for Stack, 0x100 for Heap (from makefile)
#define OFFSET 0x200
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

volatile servo_shared_mem_struct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;

void main(void)
{

    //uint32_t delay = 4000; //20us @ 200MHz
    uint32_t total_periods = 1000; // 20ms for servos


    /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    CT_CFG.GPCFG0 = 0;
    CT_CFG.GPCFG1 = 0;

    /* Clear GPO pins */
    __R30 &= 0xFFFF0000;

    uint32_t i =0;

    /* TODO: Create stop condition, else it will runn indefinitely */
    while (1) {

        uint32_t width = 100;

        for(i = 0; i < total_periods; i++) {
            __delay_cycles(delay);

            if (pSharedMemStruct->identifier == SERVO_IDENTIFIER) {
                // Read from shared memory
                width = pSharedMemStruct->pwm1;
            }
            else
            {
                // Wasn't valid - show 50% as error code
                width = 500;
            }

            if (i <= width) {
                __R30 |= servo1_bit;
            }
            else
            {
                __R30 &= !servo1_bit;
            }
        }
    }
}


