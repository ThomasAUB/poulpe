#pragma once

#include <tuple>


////////////////////////////////////////////////////////////////////////////////

template<typename...signal_types>
struct SignalTypes{ using signals = std::tuple<signal_types...>; };

template<
typename notifier_t, 
typename T, 
typename signal_types_t, 
typename...args_t>
struct NotifierFactory_M : public T {

	NotifierFactory_M(args_t...a):T(a...){}

private:

	template<size_t FT> struct FakeType{};
	
	static constexpr size_t kSignalCount = 
	std::tuple_size<typename signal_types_t::signals>::value;

	template<size_t I>
	using sig_t = 
	typename std::tuple_element<I, typename signal_types_t::signals>::type;

	template<size_t I>
	using sigC_t = 
	typename std::conditional_t<
	I < kSignalCount, 
	sig_t<I%kSignalCount>, FakeType<I> >;
	
	void sendSignal(sigC_t<0> s) final { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<1> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<2> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<3> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<4> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<5> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<6> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<7> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<8> s) { notifier_t::sP.sendSignal(s); }
	void sendSignal(sigC_t<9> s) { notifier_t::sP.sendSignal(s); }
	
};
////////////////////////////////////////////////////////////////////////////////


/*
template<typename notifier_t, typename T, typename signal_t, typename...args_t>
struct NotifierFactory : public T{
	NotifierFactory(args_t...a):T(a...){}
	void sendSignal(signal_t s) final {
		notifier_t::sP.sendSignal(s);
	}
};*/




#define DEFINE_LISTENERS(T...)                                                  \
struct Notifier{                                                                \
	using poulpe_t = Poulpe<T>;                                             	\
	template<typename signal_t>                                             	\
	void sendSignal(signal_t& s){                                           	\
		sP.sendSignal(s);                                               		\
	}                                                                       	\
	static poulpe_t& sP;                                                    	\
};


#define CREATE_POULPE(p...)                                                     \
Notifier::poulpe_t gInterDispatcher(p);                                      	\
Notifier::poulpe_t& Notifier::sP = gInterDispatcher;                   			\


template<typename...T>
struct Poulpe{

	constexpr Poulpe(T&...p) :
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
	typename std::enable_if_t<I == kLisSize,void> 
	process_caller(signal_t& s){}

	template <size_t I = 0, typename signal_t>
	typename std::enable_if_t<(I < kLisSize), void> 
	process_caller(signal_t& s){
		std::get<I>(mListeners).receiveSignal(s);
		process_caller<I+1>(s);
	}

};



