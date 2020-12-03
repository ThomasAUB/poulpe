#pragma once


#include <tuple>


#define DEFINE_RECEIVERS(T...)                                                  \
struct Emitter{                                                                	\
	using poulpe_t = Poulpe<T>;						\
	template<typename signal_t>                                             \
	void pEmit(signal_t& s){ sP.pEmit(s); }                       		\
	static poulpe_t& sP;                                                    \
};										\


#define CREATE_POULPE(p...)                                                     \
Emitter::poulpe_t	gPoulpe(p);              		                \
Emitter::poulpe_t&	Emitter::sP = gPoulpe;                   		\


////////////////////////////////////////////////////////////////////////////////

template<typename...signal_types>
struct SignalTypes{ 
    
    using signals = std::tuple<signal_types...>; 
    
    template<size_t I>
    using signal = std::tuple_element<I, signals>::type;
    
    static constexpr size_t kSignalCount =
    std::tuple_size<signals>::value;
};

template<
typename emitter_t,
typename T,
typename signal_types_t,
typename...args_t>
struct EmitterFactory final : public T {

    EmitterFactory(args_t...a):T(a...){}

private:

    template<size_t I> struct DummyType{};
	
    template<size_t I>
    using cond_sig_t =
    typename std::conditional_t<
        I < signal_types_t::kSignalCount,
        typename signal_types_t::signal<I%signal_types_t::kSignalCount>, 
        DummyType<I> 
    >;
	
#define P_REPEAT_FUNC_GEN_1		void pEmit(cond_sig_t<__COUNTER__>& s) { emitter_t::sP.pEmit(s); }
#define P_REPEAT_FUNC_GEN_2		P_REPEAT_FUNC_GEN_1		P_REPEAT_FUNC_GEN_1
#define P_REPEAT_FUNC_GEN_4		P_REPEAT_FUNC_GEN_2		P_REPEAT_FUNC_GEN_2
#define P_REPEAT_FUNC_GEN_8		P_REPEAT_FUNC_GEN_4		P_REPEAT_FUNC_GEN_4
#define P_REPEAT_FUNC_GEN_16	P_REPEAT_FUNC_GEN_8		P_REPEAT_FUNC_GEN_8
#define P_REPEAT_FUNC_GEN_32	P_REPEAT_FUNC_GEN_16	P_REPEAT_FUNC_GEN_16
#define P_REPEAT_FUNC_GEN_64	P_REPEAT_FUNC_GEN_32	P_REPEAT_FUNC_GEN_32
#define P_REPEAT_FUNC_GEN_128	P_REPEAT_FUNC_GEN_64	P_REPEAT_FUNC_GEN_64

// base class interface is allowed for 128 different signals
P_REPEAT_FUNC_GEN_128

};
////////////////////////////////////////////////////////////////////////////////






template<typename...T>
struct Poulpe{

    constexpr Poulpe(T&...p) :
    mReceivers(std::tuple<T&...>(p...))
    {}

    template<typename signal_t>
    void pEmit(signal_t& s){
        static_assert(!std::is_fundamental<signal_t>::value,
            "Invalid signal type : can't be fundamental type");        
        process_caller(s);
    }

private:

    using receivers_t = std::tuple<T&...>;

    static constexpr size_t kListSize = std::tuple_size<receivers_t>::value;

    const receivers_t mReceivers;

    template <size_t I = 0, typename signal_t>
    typename std::enable_if<I == kListSize, void>::type
    process_caller(signal_t& s){ static_cast<void>(s); }

    template <size_t I = 0, typename signal_t>
    typename std::enable_if<(I < kListSize), void>::type
    process_caller(signal_t& s){
        std::get<I>(mReceivers).pReceive(s);
        process_caller<I+1>(s);
    }

};





