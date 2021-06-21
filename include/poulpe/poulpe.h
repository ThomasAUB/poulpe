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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <tuple>
#include <type_traits>
    



// pass the typenames of receivers
#define DEFINE_RECEIVERS(...)                                                       \
struct Emitter {                                                                    \
	using poulpe_t = Poulpe<__VA_ARGS__>;                                           \
	template<typename signal_t>                                                     \
	static void pEmit(signal_t& s){ sP.pEmit(s); }                                  \
	static poulpe_t& sP;                                                            \
};                                                                                  \




// pass the instances of the receivers
#define CREATE_POULPE(...)                                                          \
Emitter::poulpe_t	gPoulpe(__VA_ARGS__);                                           \
Emitter::poulpe_t&	Emitter::sP = gPoulpe;                                          \





// Primary template with a static assertion
// for a meaningful error message
// if it ever gets instantiated.
// We could leave it undefined if we didn't care.

template<typename, typename T>
struct IsReceiver {
    static_assert(
        std::integral_constant<T, false>::value,
        "Second template parameter needs to be of function type.");
};

// specialization that does the checking

template<typename C, typename Ret, typename... Args>
struct IsReceiver<C, Ret(Args...)> {
private:
    template<typename T>
    static constexpr auto check(T*)
    -> typename
        std::is_same<
            decltype( std::declval<T>().pReceive( std::declval<Args>()... ) ),
            Ret    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        >::type;  // attempt to call it and see if the return type is correct

    template<typename>
    static constexpr std::false_type check(...);

    typedef decltype(check<C>(0)) type;

public:
    static constexpr bool value = type::value;
};




template<typename... T>
struct Poulpe {

    constexpr Poulpe(T&...p) :
    mReceivers(std::tuple<T&...>(p...)) {}

    template<typename signal_t>
    void pEmit(signal_t &s) {

        static_assert(!std::is_fundamental<signal_t>::value,
                "Invalid signal type : can't be fundamental type");

        process_caller(s);
    }

private:

    using receivers_t = std::tuple<T&...>;

    template<size_t I>
    using type_at = typename std::tuple_element<I, std::tuple<T...>>::type;

    static constexpr size_t kListSize = std::tuple_size<receivers_t>::value;

    const receivers_t mReceivers;

    // last call
    template<size_t I = 0, typename signal_t>
    typename std::enable_if<I == kListSize, void>::type
    process_caller(signal_t&) {}

    // real receiver caller
    template<size_t I = 0, typename signal_t>
    typename std::enable_if<
        (I < kListSize) &&
        IsReceiver<type_at<(I%sizeof...(T))>, void(signal_t&)>::value,
        void>::type
    process_caller(signal_t &s) {
        std::get<I>(mReceivers).pReceive(s);
        process_caller<I + 1>(s);
    }

    // receiver not found
    template<size_t I = 0, typename signal_t>
    typename std::enable_if<
        (I < kListSize) &&
        !IsReceiver<type_at<(I%sizeof...(T))>, void(signal_t&)>::value,
        void>::type
    process_caller(signal_t &s) {
        process_caller<I + 1>(s);
    }

};



