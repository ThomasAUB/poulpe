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

template<eTestSigType I>
struct RX_var {
    static bool value;
};

template<eTestSigType I>
bool RX_var<I>::value = false;

struct PingReceiver{

    void pReceive(PingSignal<eTestSigType::eRef>& s){
        s.r = true;
        RX_var<eTestSigType::eRef>::value = true;
    }

    void pReceive(PingSignal<eTestSigType::eCopy> s){
        s.r = true;
        RX_var<eTestSigType::eCopy>::value = true;
    }

    void pReceive(const PingSignal<eTestSigType::eConstRef>& s){
        RX_var<eTestSigType::eConstRef>::value = true;
    }
    
    void pReceive(const PingSignal<eTestSigType::eConstCopy> s){
        RX_var<eTestSigType::eConstCopy>::value = true;
    }

};

DEFINE_RECEIVERS(PingReceiver)

PingReceiver ping_rx;

CREATE_POULPE(ping_rx)
