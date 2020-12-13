#include <iostream>

#include "poulpe.h"

struct PingSignal{
    PingSignal(bool& _r):r(_r){}
    bool& r;
};




struct TX{
    void transmit(bool& r){
        PingSignal s(r);
        pEmit(s);
    }
    virtual void pEmit(PingSignal&) = 0;
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



int main() {

	bool ping = false;

	EmitterFactory<Emitter, TX, SignalTypes<PingSignal>> tx;

	tx.transmit(ping);

	if(ping){
		std::cout << "Ping OK" << std::endl;
	}else{
		std::cout << "Ping NOT OK !" << std::endl;
	}


	return 0;
}
