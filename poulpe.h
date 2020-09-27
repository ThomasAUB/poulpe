#pragma once

#include <tuple>


////////////////////////////////////////////////////////////////////////////////
/*
template<typename notifier_t, typename signal_t> 
struct VirtGen{
	void sendSignal(signal_t s) override final{
		notifier_t::sP.sendSignal(s);
	}
};
*/
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
	
	static constexpr size_t kSignalCount = 
	std::tuple_size<typename signal_types_t::signals>::value;

	template<size_t I>
	using sig_t = 
	typename std::tuple_element<I, typename signal_types_t::signals>::type;
	
	void sendSignal(sig_t<0> s) override final {
		notifier_t::sP.sendSignal(s);
	}

	void sendSignal(sig_t<1> s) override final {
		notifier_t::sP.sendSignal(s);
	}

	void sendSignal(sig_t<2> s) override final {
		notifier_t::sP.sendSignal(s);
	}

	/*typename std::enable_if< 3 < kSignalCount, void>
	sendSignal(std::enable_if< 3 < kSignalCount, sig_t<3>> s){
		notifier_t::sP.sendSignal(s);
	}*/

	/*typename std::enable_if<3 < kSignalCount, void>
	signalSender(){
		
	}*/

	/*void sendSignal(sig_t<3> s) override final {
		notifier_t::sP.sendSignal(s);
	}*/

	/*void sendSignal(const SEQ_signal& s){
		notifier_t::sP.sendSignal(s);
	}*/

	//virtual void sendSignal(sig_t<1>& s){}

	//virtual void sendSignal(sig_t<2> s){}

	//typename std::enable_if_t<0 < kSignalCount, void>
	//sendSignal(sig_t<0> s) override final {}

	/*typename std::enable_if_t<1 < kSignalCount, void>
	sendSignal(sig_t<1> s) override final {}*/
	
};
////////////////////////////////////////////////////////////////////////////////



template<typename notifier_t, typename T, typename signal_t, typename...args_t>
struct NotifierFactory : public T{
	NotifierFactory(args_t...a):T(a...){}
	virtual void sendSignal(signal_t s) final override {
		notifier_t::sP.sendSignal(s);
	}
};




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



