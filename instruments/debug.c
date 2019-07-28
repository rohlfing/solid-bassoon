#include <stdio.h>
#include <stdlib.h>

#include "debug.h"

typedef struct{
    int id; 
} dbg_inst_t;

static int num_dbg_insts;

void * debug_init(){
    dbg_inst_t * inst;
    
    inst = (dbg_inst_t*) malloc(sizeof(dbg_inst_t));

    /* Assign a unique id to each debug instance */
    inst->id = num_dbg_insts;
    ++num_dbg_insts;

    return inst;
}

void debug_free(void * inst){
    free(inst);
}

/* This function matches the cmnd_cb_t type */
err_t debug_callback(void * inst, cmnd_t c, note_t n, vlct_t v){
    dbg_inst_t * this;

    this = (dbg_inst_t*)inst;

    printf("Debug inst: %#4x\n"
           "Command:    %#4x\n"
           "Note:       %#4x\n"
           "Velocity:   %#4x\n"
           "\n");

    return ERR_NONE;
}
