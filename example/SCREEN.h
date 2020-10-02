#pragma once

#include "HUI_signal.h"


struct SCREEN{

	SCREEN(){}

	// mandatory receiver function
	template<typename T> void receiveSignal(T& t) {}

	void receiveSignal(const HUI_signal& qs){
		std::cout << "hui signal received in screen" << std::endl;
	}

};
