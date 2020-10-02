#pragma once

#include "SEQ_signal.h"

#include "OPT.h"

struct STEP_signal{};

struct SEQ{

	SEQ(uint8_t i) : mVal(i)
	{}

	// mandatory receiver function
	template<typename T> void receiveSignal(T& t) {}

	void Test(){
		sendSignal(SEQ_signal(9, 6));
	}

	virtual void sendSignal(const SEQ_signal& s) = 0;
	virtual void sendSignal(const OPT_signal& s) = 0;
	virtual void sendSignal(const HUI_signal& s) = 0;

	const uint8_t mVal;

};
