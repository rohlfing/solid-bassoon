#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
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
static uint8_t glbl_valves;

void * pwm_bitbang_main(void * arg);

void hw_trumpet_init(){
    pthread_t pwm_thread;
    int fd;

    /* Open up memory access to gpio registers */
    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if(fd < 0) fprintf(stderr, "Cannot open mem\n");
    gpio_regs = mmap(NULL, 0xB0, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x20200000);
    close(fd);
    if(MAP_FAILED == gpio_regs) fprintf(stderr, "Cannot map mem\n");

    /* Set GPIOs to output and clear */
    *(gpio_regs + GPFSEL1) &= ~GPFSEL1_MASK; 
    *(gpio_regs + GPFSEL2) &= ~GPFSEL2_MASK;
    *(gpio_regs + GPFSEL1) |= (1 << 21);
    *(gpio_regs + GPFSEL2) |= (1 << 21) + (1 << 6);
    *(gpio_regs + GPCLR0)   = ALL_GPIO_MASK;

    /* Start up PWM bit-bang thread */
    pthread_create(&pwm_thread, NULL, pwm_bitbang_main, NULL); 
}

void hw_trumpet_play(uint8_t valves){
    glbl_valves = valves;
}

/* TODO ADR ensure 'pwm' pins aren't left in a bad state */
void * pwm_bitbang_main(void * arg){
#   define usleep(x) usleep((x) - 90) // correction factor; emperically found
    static int pins[3] = {17, 27, 22};
    uint32_t clr_reg;
    uint8_t lcl_valves;
    int i;

    (void)arg;

    while(1){
        /* Set all pins high; wait 1.5ms */
        *(gpio_regs + GPSET0) = ALL_GPIO_MASK;
        usleep(1500);

        /* Clear any pins corresponding to non-active valves; wait 0.9ms */
        clr_reg = 0;
        lcl_valves = glbl_valves;
        for(i = 0; i < 3; ++i)
            clr_reg |= (lcl_valves & (1<<i) ? 0 : 1) << (pins[i]);
        *(gpio_regs + GPCLR0) = clr_reg;
        usleep(900);

        /* Clear all pins; wait 0.6ms */
        *(gpio_regs + GPCLR0) = ALL_GPIO_MASK;
        usleep(600);
    }
#   undef usleep
}
