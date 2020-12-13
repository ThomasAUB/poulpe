#include <iostream>

#include "poulpe.h"

struct MySignal{};

struct RX{

	template<typename void_sig> void pReceive(void_sig){}

	void pReceive(MySignal& s){
		std::cout << "Signal received" << std::endl; // prints !!!Hello World!!!
	}

};



template<typename emitter_t>
struct TX : private emitter_t{

	void test(){
		MySignal s;
		this->pEmit(s);
	}

};

DEFINE_RECEIVERS(RX)

RX rx;

TX<Emitter> tx;

CREATE_POULPE(rx)



int main() {

	tx.test();

	return 0;
}
