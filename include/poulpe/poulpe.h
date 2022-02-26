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
namespace poulpe {                                                                  \
    struct Emitter {                                                                \
        using poulpe_t = poulpe::Poulpe<__VA_ARGS__>;                               \
                                                                                    \
        /* sends the signal to every receivers that implement the right function */ \
        template<typename signal_t>                                                 \
        static void pEmit(signal_t&& s){ sP.pEmit(s); }                             \
                                                                                    \
        /* returns the number of receivers that implement the right function */     \
        template<typename signal_t>                                                 \
        static constexpr std::size_t getReceiverCount() {                           \
            return poulpe_t::getReceiverCount<signal_t>();                          \
        }                                                                           \
                                                                                    \
    private:                                                                        \
        static poulpe_t sP;                                                         \
    };                                                                              \
}                                                                                   \



//! pass the instances of the receivers
#define CREATE_POULPE(...)                                                          \
poulpe::Emitter::poulpe_t poulpe::Emitter::sP(__VA_ARGS__);                         \








namespace poulpe {

    namespace utils { // compile-time utils

        // === receiver sensor === //

        // value is true if rx_t implements a pReceive
        // function able to receive a signal of type signal_t
        template<typename rx_t, typename signal_t>
        struct is_receiver {
        private:
            template<typename R>
            static constexpr auto check(R*)
            -> typename
                std::is_same<
                    decltype(
                            std::declval<R>().pReceive( std::declval<signal_t&>() )
                            ),
                    void
                >::type;  // attempt to call pReceive and check the result

            template<typename>
            static constexpr std::false_type check(...);

            typedef decltype(check<rx_t>(0)) type;

        public:
            static constexpr bool value = type::value;
        };

        // ======================= //

    }



    template<typename... T>
    struct Poulpe {

        constexpr Poulpe(T&...p) :
        mReceivers(std::tuple<T&...>(p...))
        {}

        // dispatch signal_t to receivers
        template<typename signal_t>
        constexpr void pEmit(signal_t &s) {

            static_assert(!std::is_fundamental<signal_t>::value,
                    "Invalid signal type : can't be fundamental type");

            if constexpr (getReceiverCount<signal_t>() > 0) {
                unfoldCall(s, std::make_index_sequence<sizeof...(T)>{});
            }

        }

        // returns the number of receiever for signal_t
        template<typename signal_t>
        static constexpr std::size_t getReceiverCount() {
            return

            (
                (
                    ( poulpe::utils::is_receiver<T, signal_t>::value ) ?
                    1 :
                    0
                ) + ...
            );

        }

    private:

        // call pReceive function if rx_t implements
        // the pReceive function able to receive the type signal_t
        template<std::size_t I, typename signal_t>
        void call(signal_t& s) {

            using rx_t = typename std::tuple_element<I, std::tuple<T...>>::type;

            if constexpr (poulpe::utils::is_receiver<rx_t, signal_t>::value) {
                std::get<I>(mReceivers).pReceive(s);
            }

        }

        template<std::size_t... Is, typename signal_t>
        void unfoldCall(signal_t& s, const std::index_sequence<Is...>&) {
            (call<Is>(s), ...);
        }

        const std::tuple<T&...> mReceivers;

    };
}
