/*
 * servo_pru.c
 *
 *  Created on: 18 Sep 2023
 *      Author: Fred
 */

#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table.h"

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

void main(void)
{

    /* Clear SYSCFG[STANDBY_INIT] to enable OCP master port */
    CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

    CT_CFG.GPCFG0 = 0;
    CT_CFG.GPCFG1 = 0;

    /* Clear GPO pins */
    __R30 &= 0xFFFF0000;

    /* TODO: Create stop condition, else it will toggle indefinitely */
    while (1) {
        /* Toggle GPO pin */
        __R30 ^= servo1_bit;
        __delay_cycles(1000000);
    }
}


