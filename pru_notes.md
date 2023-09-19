# Notes on using te PRU

## Communicating between the ARM core and PRU
### Shared memory
This is now librobotcontrol works. Shared memory is allocated - see _rc_pru_shared_mem_ptr()_ in library/src/pru/pru.c - and can be accessed by both. Note that _rc_servo_init()_ lets the PRU zero this out and checks that it has done so.

Reading and writing from the ARM side seems simple enough. On the PRU side, here's only an asm example of using PRU_SHAREDMEM (defined in AM335x_PRU.cmd) so my have to look for a C version! It's probably simple when you see it.
This looks good - and is as simple as expected: https://catch22eu.github.io/website/beaglebone/beaglebone-pru-ipc/

Also, it seems that accessing shared memory requires running as root on the Linux side, so maybe it's not ideal.

## rpmsg
This is used in the PRU training examples. The echo code runs on the PRU, but does not seem to create the /dev/rpmsg_pru31 file that's used for communication. Maybe there's something else needed. See https://software-dl.ti.com/processor-sdk-linux/esd/docs/08_00_00_21/common/PRU-ICSS/PRU-Hands-on-Labs.html#lab-5-rpmsg-communication-between-arm-and-pru

To try: Downgrading from Debian 10.13 to the last official build of 10.3
