/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MIT License                                                                     *
 *                                                                                 *
 * Copyright (c) 2020 Thomas AUBERT                                                *
 *                                                                                 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy    *
 * of this software and associated documentation files (the "Software"), to deal   *
 * in the Software without restriction, including without limitation the rights    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is           *
 * furnished to do so, subject to the following conditions:                        *
 *                                                                                 *
 * The above copyright notice and this permission notice shall be included in all  *
 * copies or substantial portions of the Software.                                 *
 *                                                                                 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 * SOFTWARE.                                                                       *
 *                                                                                 *
 * github : https://github.com/ThomasAUB/poulpe                                    *
 *                                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <tuple>
#include <type_traits>




//! pass the typenames of receivers
#define DEFINE_RECEIVERS(...)                                                       \
struct Emitter {                                                                    \
    using poulpe_t = Poulpe<__VA_ARGS__>;                                           \
                                                                                    \
    /* sends the signal to every receivers that implement the right function */     \
    template<typename signal_t>                                                     \
    static void pEmit(signal_t&& s){ sP.pEmit(s); }                                 \
                                                                                    \
    /* returns the number of receivers that implement the right function */         \
    template<typename signal_t>                                                     \
    static constexpr std::size_t getReceiverCount() {                               \
        return poulpe_t::getReceiverCount<signal_t>();                              \
    }                                                                               \
                                                                                    \
private:                                                                            \
    static poulpe_t sP;                                                             \
};                                                                                  \




//! pass the instances of the receivers
#define CREATE_POULPE(...)                                                          \
Emitter::poulpe_t Emitter::sP(__VA_ARGS__);                                         \










template<typename... T>
struct Poulpe {

    constexpr Poulpe(T&...p) :
    mReceivers(std::tuple<T&...>(p...)) {}

    template<typename signal_t>
    void pEmit(signal_t &s) {

        static_assert(!std::is_fundamental<signal_t>::value,
                "Invalid signal type : can't be fundamental type");
        process_call(s, std::make_index_sequence<sizeof...(T)>{});
    }

    template<typename signal_t>
    static constexpr std::size_t getReceiverCount() {
    	std::size_t ioCount = 0;
    	countReceivers<signal_t>(ioCount);
    	return ioCount;
    }

private:

    template<typename signal_t, std::size_t I = 0>
    static constexpr void countReceivers(std::size_t& ioCount) {

    	if constexpr (
    			IsReceiver<type_at<(I%sizeof...(T))>, void(signal_t&)>::value
		) {
    		ioCount++;
    	}

    	if constexpr (I < sizeof...(T) - 1) {
    		countReceivers<signal_t, I + 1>(ioCount);
    	}
    }

    template<size_t I>
    using type_at = typename std::tuple_element<I, std::tuple<T...>>::type;

    const std::tuple<T&...> mReceivers;

    // === receiver sensor === //

    //! Primary template with a static assertion
    //! for a meaningful error message
    //! if it ever gets instantiated.
    //! We could leave it undefined if we didn't care.
    template<typename, typename R>
    struct IsReceiver {
        static_assert(
            std::integral_constant<R, false>::value,
            "Second template parameter needs to be of function type.");
    };

    //! specialization that does the checking
    template<typename C, typename Ret, typename... Args>
    struct IsReceiver<C, Ret(Args...)> {
    private:
        template<typename R>
        static constexpr auto check(R*)
        -> typename
            std::is_same<
                decltype( std::declval<R>().pReceive( std::declval<Args>()... ) ),
                Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            >::type;  // attempt to call it and see if the return type is correct

        template<typename>
        static constexpr std::false_type check(...);

        typedef decltype(check<C>(0)) type;

    public:
        static constexpr bool value = type::value;
    };

    // ======================= //

    //! call receiver I
    template<std::size_t I, typename signal_t>
    inline typename std::enable_if<
    IsReceiver<type_at<(I%sizeof...(T))>, void(signal_t&)>::value,
    void>::type
    call_receiver(signal_t &s) {
        std::get<I>(mReceivers).pReceive(s);
    }

    //! empty receiver : do nothing
    template<std::size_t I, typename signal_t>
    inline typename std::enable_if<
    !IsReceiver<type_at<(I%sizeof...(T))>, void(signal_t&)>::value,
    void>::type
    call_receiver(signal_t &s) {}

    //! call every receivers
    template<typename signal_t, std::size_t... Is>
    inline void process_call(signal_t &s, std::index_sequence<Is...>) {
        (call_receiver<Is>(s), ...);
    }


};
