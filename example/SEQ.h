#pragma once

#include "SEQ_signal.h"


template<typename Notif_t>
struct SEQ : private Notif_t{

	template<typename T>
	void receiveSignal(T& t){}

	void Test(){
		SEQ_signal s(9, 6);
		this->sendSignal(s);
	}


};
