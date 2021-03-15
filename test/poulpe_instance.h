#pragma once


#include "../include/poulpe/poulpe.h"

struct PingSignal{
    PingSignal(bool& _r):r(_r){}
    bool& r;
};

struct PingReceiver{

    template<typename void_sig> void pReceive(void_sig){}

    void pReceive(PingSignal& s){
        s.r = true;
    }
    
};

DEFINE_RECEIVERS(PingReceiver)

PingReceiver ping_rx;

CREATE_POULPE(ping_rx)
