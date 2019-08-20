#include <stdio.h>
#include <stdlib.h>

#include "trumpet.h"
#include "hw_drivers/hw_trumpet.h"

typedef struct{
    int id; 
} trmpt_inst_t;

static int num_trmpt_insts;

#define NOTE_TRMPT_LOW  0x37 /* G3 */
#define NOTE_TRMPT_HIGH 0x54 /* C6 */
#define NUM_NOTES       (NOTE_TRMPT_HIGH - NOTE_TRMPT_LOW + 1)

/*
 * Bits 0 through 2 for valves
 * 0x0 =>   O O O
 * 0x1 =>   O O X
 * ...
 * 0x7 =>   X X X
 */
static uint8_t fingers[NUM_NOTES] = {
    /* G3 -> B3 */
                                       0x5, 0x3, 0x6, 0x4, 0x2,
    /* C4 -> B4 */
    0x0, 0x7, 0x5, 0x3, 0x6, 0x4, 0x2, 0x0, 0x3, 0x6, 0x5, 0x2,
    /* C5 -> B5 */
    0x0, 0x6, 0x4, 0x2, 0x0, 0x4, 0x2, 0x0, 0x3, 0x6, 0x4, 0x2,
    /* C6 */
    0x0
};

void * trumpet_init(){
    trmpt_inst_t * inst;
    
    inst = (trmpt_inst_t*) malloc(sizeof(trmpt_inst_t));

    /* Assign a unique id to each trumpet instance */
    inst->id = num_trmpt_insts;
    ++num_trmpt_insts;

    hw_trumpet_init();

    return inst;
}

void trumpet_free(void * inst){
    free(inst);
}

/* This function matches the cmnd_cb_t type */
err_t trumpet_callback(void * inst, cmnd_t c, note_t n, vlct_t v){
    trmpt_inst_t * this;
    uint8_t f;
    char rep[4]; /* String representation of valves */

    /* End program on lowest piano note */
    if(NOTE_LOWEST == n){
        hw_trumpet_play(0xff);
        return ERR_DONE;
    }

    if(n > NOTE_TRMPT_HIGH || n < NOTE_TRMPT_LOW)
        return ERR_NOTE_OOB;

    f = fingers[n - NOTE_TRMPT_LOW];

    rep[0] = f & 0x4 ? 'X' : 'O';
    rep[1] = f & 0x2 ? 'X' : 'O';
    rep[2] = f & 0x1 ? 'X' : 'O';
    rep[3] = '\0';

    if(c == CMND_DIR_DOWN)
        hw_trumpet_play(f);
        //printf("%s\n", rep);

    this = (trmpt_inst_t*)inst;

    return ERR_NONE;
}
