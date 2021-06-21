#pragma once


#include "../include/poulpe/poulpe.h"

enum class eTestSigType{
    eRef,
    eConstRef,
    eCopy,
    eConstCopy
};

template<eTestSigType I>
struct PingSignal{
    PingSignal(bool& _r):r(_r){}
    bool& r;
};


struct PingReceiver{

    void pReceive(PingSignal<eTestSigType::eRef>& s){
        s.r = true;
        sRX = true;
    }

    void pReceive(const PingSignal<eTestSigType::eConstRef>& s){
        sRX = true;
    }

    void pReceive(PingSignal<eTestSigType::eCopy> s){
        s.r = true;
        sRX = true;
    }

    void pReceive(const PingSignal<eTestSigType::eConstCopy> s){
        sRX = true;
    }

    static bool sRX;

};

DEFINE_RECEIVERS(PingReceiver)

PingReceiver ping_rx;

CREATE_POULPE(ping_rx)
