#include <stdio.h>

#include "sources/midi_dev.h"
#include "nodes/passthrough.h"
#include "instruments/debug.h"
#include "instruments/trumpet.h"

int main(int argc, char ** argv){
    void* midi_source;
    void* pt_node;
    void* debug_inst;
    void* trumpet_inst;

    /* MIDI source -> Passthrough node (NOP) -> Debug (print) instrument */
    debug_inst   = debug_init();
    trumpet_inst = trumpet_init();
    pt_node      = passthrough_init(trumpet_callback, trumpet_inst);
    //pt_node      = passthrough_init(debug_callback, debug_inst);
    midi_source  = midi_dev_init("/dev/midi1/", passthrough_callback, pt_node);

    midi_dev_run(midi_source);

    return 0;
}
