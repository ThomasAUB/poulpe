#include <iostream>

#include "poulpe.h"

struct PingSignal{
    PingSignal(bool& _r):r(_r){}
    bool& r;
};



template<typename emitter_t>
struct TX : private emitter_t{
    TX(bool& r){
        PingSignal s(r);
        this->pEmit(s);
    }
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

	TX<Emitter> tx(ping);

	if(ping){
		std::cout << "Ping OK" << std::endl;
	}else{
		std::cout << "Ping NOT OK !" << std::endl;
	}


	return 0;
}
