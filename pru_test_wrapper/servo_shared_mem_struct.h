#ifndef _SERVO_SHARED_MEM_STRUCT_H_
#define _SERVO_SHARED_MEM_STRUCT_H_

#include <stdint.h>

#define SERVO_IDENTIFIER 0x7974

// WARNING:
// Fields in the struct must be aligned to match ARM's alignment
// bool/char, uint8_t: byte aligned
// int/long, uint32_t: word (4 byte) aligned
// double, uint64_t: dword (8 byte) aligned
// Add padding fields (char _p1) to pad out to alignment.
// My Shared Memory Structure
// ----------------------------------------------------------------
typedef struct {
    uint32_t identifier;    // Should be 0x7974 or something's wrong
    uint32_t  pwm1;         // 0-999
    uint32_t  pwm2;         // 0-999
} servo_shared_mem_struct_t;

#endif
