typedef enum{
    ERR_NONE = 0,

    /* Command errors */
    ERR_CMND_UNSP,

    /* Note errors */
    ERR_NOTE_OOB,

    /* Velocity Errors */
    ERR_VLCT_OOB,

    /* Fatal error and error count must be last */
    ERR_FATAL,
    NUM_ERR
} err_t;

void err_print(err_t error);
