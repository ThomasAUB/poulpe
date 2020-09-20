#pragma once


#include "HUI_signal.h"
#include "SEQ_signal.h"

template<typename Notif_t>
struct HUI : private Notif_t{

	template<typename T>
	void receiveSignal(T& t){}

	void Test(){
		HUI_signal sig("yo");
		this->sendSignal(sig);
	}

	void receiveSignal(SEQ_signal& s){
		std::cout << "seq signal received in HUI" << std::endl;
		HUI_signal sig("wesh");
		this->sendSignal(sig);
	}

};
