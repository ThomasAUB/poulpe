#pragma once


#include "HUI_signal.h"
#include "SEQ_signal.h"


struct HUI{

	// mandatory receiver function
	template<typename T> void receiveSignal(T& t) {}

	void Test(){
		sendSignal(HUI_signal("yo"));
	}

	void receiveSignal(const SEQ_signal& s){
		std::cout << "seq signal received in HUI" << std::endl;
		this->sendSignal(HUI_signal("wesh"));
	}

	virtual void sendSignal(const HUI_signal& s) = 0;

};
