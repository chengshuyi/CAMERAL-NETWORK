#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

enum state{
    INIT_CAMERA = 0,
    READ_CAMERA,
    TRAN_FORMAT,
    SEND_PACKET,
    ERROR_STATE,
};

void to_state(int state);
void do_state();

#endif
