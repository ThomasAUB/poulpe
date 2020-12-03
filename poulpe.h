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
struct SignalTypes{ using signals = std::tuple<signal_types...>; };

template<
typename emitter_t,
typename T,
typename signal_types_t,
typename...args_t>
struct EmitterFactory final : public T {

    EmitterFactory(args_t...a):T(a...){}

private:

    static constexpr size_t kSignalCount =
    std::tuple_size<typename signal_types_t::signals>::value;

    template<size_t I> struct DummyType{};

    template<size_t I>
    using sig_t =
    typename std::tuple_element<I, typename signal_types_t::signals>::type;
	
    template<size_t I>
    using cond_sig_t =
    typename std::conditional_t<
    I < kSignalCount,
    sig_t<I%kSignalCount>, DummyType<I> >;
	
#define REPEAT_1	void pEmit(cond_sig_t<__COUNTER__>& s) { emitter_t::sP.pEmit(s); }
#define REPEAT_2	REPEAT_1	REPEAT_1
#define REPEAT_4	REPEAT_2	REPEAT_2
#define REPEAT_8	REPEAT_4	REPEAT_4
#define REPEAT_16	REPEAT_8	REPEAT_8
#define REPEAT_32	REPEAT_16	REPEAT_16
#define REPEAT_64	REPEAT_32	REPEAT_32
#define REPEAT_128	REPEAT_64	REPEAT_64

// base class interface is allowed for 128 different signals
REPEAT_128

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





