#include "error.h"

/* Typdef convient names for message components */
typedef uint8_t cmnd_t;
typedef uint8_t note_t;
typedef uint8_t vlct_t;

/* Command id definitions */
#define CMND_DIR_DOWN   0x90
#define CMND_DIR_UP     0x80

/* Note number definitions */
#define NOTE_A0         0x15
#define NOTE_C0         0x18
#define NOTE_C8         0x6c
#define NOTE_LOWEST     NOTE_A0
#define NOTE_HIGHEST    NOTE_C8

/* Velocity Definitions */
#define VLCT_MIN        0x01
#define VLCT_MAX        0x7F

/* Callback type used between nodes */
typedef err_t (*cmnd_cb_t)(cmnd_t , note_t , vlct_t );
