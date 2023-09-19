# Vader Blue PRU notes

## Setup

## Copying output
On PC
~~~
> cd VaderBlue
.../Debug> scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null PRU\servo_pru\Debug\*.out debian@beagleblue.local:/home/debian/vader
~~~

On Beaglebone
~~~
debian@beagleblue:~$ sudo -i
root@beagleblue:~# cp /home/debian/vader/servo_pru.out /lib/firmware/servo_pru
root@beagleblue:~#  cd /sys/class/remoteproc/remoteproc1
root@beagleblue:/sys/class/remoteproc/remoteproc1# echo 'servo_pru' > firmware
root@beagleblue:/sys/class/remoteproc/remoteproc1# echo 'start' > state
cat state
~~~

## PRU Pins
Servo pins are:
| Servo | Sitara pin | PRU no. | Register | Bit |
| ----- | ---------- | ------- | -------- | --- |
| 1     | U5         | 1       | R30      |  8  |
| 2     | V5         | 1       | R30      | 10  |
| 3     | R5         | 1       | R30      |  9  |
| 4     | R6         | 1       | R30      | 11  |
| 5     | T3         | 1       | R30      |  6  |
| 6     | T4         | 1       | R30      |  7  |
| 7     | T1         | 1       | R30      |  4  |
| 8     | T2         | 1       | R30      |  5  |

Full list of PRU pins are in datasheet https://www.ti.com/lit/ds/symlink/am3359.pdf
See 4.3.4.2 for PRU1 - e.g. pr1_pru1_pru_r30_8 connects to ZCZ ball U5

## LEDs
These can't be connected to the PRU output
User LEDs are:
| LED   | Sitara pin |
| ----- | ---------- |
| G     | T7         |
| R     | R7         |
| 0     | V15        |
| 1     | U15        |
| 2     | T15        |
| 3     | V16        |

Battery LEDs are:
| LED   | Sitara Pin |
| BATT1 | U12        |
| BATT2 | T5         |
| BATT3 | V6         |
| BATT4 | T11        |