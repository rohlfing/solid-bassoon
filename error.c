#include <stdio.h>
#include <stdlib.h>

#include "error.h"

char const * error_strings[NUM_ERR] = {
    [ERR_NONE]      = "No Error",
    [ERR_DONE]      = "Done",

    [ERR_CMND_UNSP] = "Command unsupported",
    [ERR_NOTE_OOB]  = "Note out of bounds",
    [ERR_VLCT_OOB]  = "Velocity out of bounds",
    [ERR_NO_CB]     = "No callback defined",

    [ERR_FATAL]     = "Fatal error. Program terminated"
};

void err_print(err_t error){
    fprintf(stdout, "Error: %s\n", error_strings[error]);
}
