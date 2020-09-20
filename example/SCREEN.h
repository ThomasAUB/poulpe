#pragma once

#include "HUI_signal.h"


struct SCREEN{

	SCREEN(){}

	template<typename T>
	void receiveSignal(T& t){}

	void receiveSignal(HUI_signal& qs){
		std::cout << "hui signal received in screen" << std::endl;
	}

};
