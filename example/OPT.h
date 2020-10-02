#pragma once

struct OPT_signal{};

template<typename notifier_t>
struct OPT : private notifier_t{
	
	OPT(uint8_t _m, uint8_t _p, uint8_t _y):
	m(_m), p(_p), y(_y)
	{}
	
	// mandatory receiver function
	template<typename signal_t>	void receiveSignal(signal_t& s) {}

	uint8_t m, p, y;
};
