#ifndef PASSTHROUGH_H
# define PASSTHROUGH_H

#include "interface.h"

void * passthrough_init(cmnd_cb_t callback, void * dest_inst);

void passthrough_free(void * inst);

err_t passthrough_callback(void * inst, cmnd_t c, note_t n, vlct_t v);

#endif
