#include <iostream>

#include "poulpe.h"

// signal definition
struct PingSignal{
    PingSignal(bool& _r):
    r(_r){}
    bool& r;
};



template<typename emitter_t>
struct PingEmitter {

    PingEmitter(bool& r){

        // instantiate signal
        PingSignal s(r);

        // emit signal
        emitter_t::pEmit(s);
    }

};



struct PingReceiver{

    // receive signal
    void pReceive(PingSignal& s){
        // update ping state
        s.r = true;
    }

};

DEFINE_RECEIVERS(PingReceiver)

PingReceiver rx;

CREATE_POULPE(rx)


int main() {

    // instantiate ping state
    bool ping = false;

    // instantiate ping emitter
    PingEmitter<Emitter> tx(ping);

    // test updated ping state
    if(ping){
        std::cout << "Ping OK" << std::endl;
    }else{
        std::cout << "Ping NOT OK !" << std::endl;
    }

    return 0;
}
