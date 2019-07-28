#include <stdlib.h>

#include "passthrough.h"

typedef struct{
    cmnd_cb_t callback;
    void * dest_inst;     
} pt_inst_t;

void * passthrough_init(cmnd_cb_t callback, void * dest_inst){
    pt_inst_t * this;
    
    this = malloc(sizeof(pt_inst_t));

    this->callback  = callback;
    this->dest_inst = dest_inst;

    return this;
}

void passthrough_free(void * inst){
    free(inst);
}

/* This function matches the cmnd_cb_t type */
err_t passthrough_callback(void * inst, cmnd_t c, note_t n, vlct_t v){
    pt_inst_t * this;

    this = (pt_inst_t*)inst;

    if(!this->callback)
        return ERR_NO_CB;

    return this->callback(this->dest_inst, c, n, v);
}
