#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "hw_trumpet.h"

/* Using BCM GPIOs 22, 27, and 17 */

#define BASE     0x7E200000
#define GPFSEL1 ((0x7E200004 - BASE)/4)
#define GPFSEL2 ((0x7E200008 - BASE)/4)

#define GPFSEL1_MASK  (0x7 << 21)
#define GPFSEL2_MASK ((0x7 << 21) + (0x7 << 6))

#define GPSET0  ((0x7E20001C - BASE)/4)
#define GPCLR0  ((0x7E200028 - BASE)/4)

#define ALL_GPIO_MASK ((0x1 << 22) + (0x1 << 27) + (0x1 << 17))

static volatile uint32_t* gpio_regs;

void hw_trumpet_init(){
    int fd;

    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if(fd < 0){
        fprintf(stderr, "Cannot open mem\n");
    }

    gpio_regs = mmap(NULL, 0xB0, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20200000);

    close(fd);

    if(MAP_FAILED == gpio_regs){
        fprintf(stderr, "Cannot map mem\n");
    }

    /* Set GPIOs to output and clear */
    *(gpio_regs + GPFSEL1) &= ~GPFSEL1_MASK; 
    *(gpio_regs + GPFSEL2) &= ~GPFSEL2_MASK;
    *(gpio_regs + GPFSEL1) |= (1 << 21);
    *(gpio_regs + GPFSEL2) |= (1 << 21) + (1 << 6);
    *(gpio_regs + GPCLR0)  |= ALL_GPIO_MASK;
}

void hw_trumpet_play(uint8_t valves){
    static int pins[3] = {22, 27, 17};
    int i;

    for(i = 0; i < 3; ++i)
        *(gpio_regs + (valves & (1<<i) ? GPSET0 : GPCLR0)) = (1 << pins[i]);
}
