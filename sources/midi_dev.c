#include <stdio.h>
#include <stdlib.h>

#include "midi_dev.h"

typedef struct{
    char const * dev_filename;
    cmnd_cb_t    callback;
    void *       dest_inst;
} midi_inst_t;

void * midi_dev_init(char const * dev_filename, cmnd_cb_t callback, void * dest_inst){
    midi_inst_t * this;
    
    this = malloc(sizeof(midi_inst_t));

    this->dev_filename = dev_filename;
    this->callback     = callback;
    this->dest_inst    = dest_inst;

    return this;
}

void midi_dev_free(void * inst){
    free(inst);
}

/* This function matches the inst_run_t type */
err_t midi_dev_run(void * inst){
    FILE * midi_in;
    midi_inst_t * this;
    err_t error;
    cmnd_t cmnd;
    note_t note;
    vlct_t vlct;

    error = ERR_NONE;
    this = (midi_inst_t*) inst;

    if(!this->callback){
        error = ERR_NO_CB;
        err_print(error);
        return error;
    }

    /* TODO ADR open device file */
    //midi_in = fopen(this->dev_filename, "r");
    /* Current workaround is to pipe in 'cat' output */
    midi_in = stdin;

    while(1){
        cmnd = fgetc(midi_in);
        note = fgetc(midi_in);
        vlct = fgetc(midi_in);
       
        error = this->callback(this->dest_inst, cmnd, note, vlct);
        
        if(error != ERR_NONE)
            err_print(error);
        if(ERR_DONE == error || ERR_FATAL == error)
            break;
    }

    return error;
}
