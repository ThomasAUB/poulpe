#pragma once

#include "HUI_signal.h"
#include "SEQ_signal.h"

struct GUI{

	GUI(){}

	// mandatory receiver function
	template<typename T> void receiveSignal(T& t) {}

	void receiveSignal(const HUI_signal& s){
		std::cout << "hui signal received in GUI " << s.n << std::endl;
	}

	void receiveSignal(const SEQ_signal& s){
		std::cout << "seq signal received in GUI" << std::endl;
	}

};
