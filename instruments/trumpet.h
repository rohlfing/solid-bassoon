#ifndef TRUMPET_H
# define TRUMPET_H

#include "interface.h"

void * trumpet_init();

void trumpet_free(void * inst);

err_t trumpet_callback(void * inst, cmnd_t c, note_t n, vlct_t v);

#endif
