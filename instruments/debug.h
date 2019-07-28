#ifndef DEBUG_H
# define DEBUG_H

#include "interface.h"

void * debug_init();

void debug_free(void * inst);

err_t debug_callback(void * inst, cmnd_t c, note_t n, vlct_t v);

#endif
