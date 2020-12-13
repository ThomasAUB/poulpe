#include <iostream>

#include "poulpe.h"

struct MySignal{};

struct RX{

	template<typename void_sig> void pReceive(void_sig){}

	void pReceive(MySignal& s){
		std::cout << "Signal received" << std::endl;
	}

};


struct TX{

	void test(){
		MySignal s;
		this->pEmit(s);
	}

	virtual void pEmit(MySignal&) = 0;
};





DEFINE_RECEIVERS(RX)

RX rx;

EmitterFactory<Emitter, TX, SignalTypes<MySignal>> tx;

CREATE_POULPE(rx)



int main() {

	tx.test();

	return 0;
}
