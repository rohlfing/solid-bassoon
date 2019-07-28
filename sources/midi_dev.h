#ifndef MIDI_DEV_H
# define MIDI_DEV_H

#include "interface.h"

void * midi_dev_init(char const * dev_filename, cmnd_cb_t callback, void * dest_inst);

void midi_dev_free(void * inst);

err_t midi_dev_run(void * inst);

#endif
