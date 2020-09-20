#pragma once

#include <tuple>

#define DEFINE_LISTENERS(T...)                                                  \
struct Notifier{                                                                \
	using poulpe_t = Poulpe<T>;                                                 \
	template<typename signal_t>                                                 \
	void sendSignal(signal_t& s){                                               \
		sP.sendSignal(s);                                                       \
	}                                                                           \
private :                                                                       \
	static poulpe_t& sP;                                                        \
};                                                                              \


#define CREATE_POULPE(p...)                                                     \
Notifier::poulpe_t gInternalDispatcher(p);                                      \
Notifier::poulpe_t& Notifier::sP = gInternalDispatcher;                         \


template<typename ...T>
struct Poulpe{

	constexpr Poulpe(T& ...p) :
	mListeners(std::tuple<T&...>(p...))
	{}

	template<typename signal_t>
	void sendSignal(signal_t& s){
		static_assert(!std::is_fundamental<signal_t>::value, 
			"Invalid signal type : can't be fundamental type");
		process_caller(s);
	}

private:

	using listeners_t = std::tuple<T&...>;

	static constexpr size_t kLisSize = std::tuple_size<listeners_t>::value;

	const listeners_t mListeners;

	template <size_t I = 0, typename signal_t>
	typename std::enable_if<I == kLisSize,void>::type 
	process_caller(signal_t& s){}

	template <size_t I = 0, typename signal_t>
	typename std::enable_if<(I < kLisSize), void>::type 
	process_caller(signal_t& s){
		std::get<I>(mListeners).receiveSignal(s);
		process_caller<I+1>(s);
	}

};



