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



        template<typename ... T>
        struct unique_types;

        template<typename T>
        struct unique_types<T> {
            static constexpr bool value = true;
        };

        template<typename T1, typename T2>
        struct unique_types<T1, T2> {
            static constexpr bool value = !std::is_same<T1, T2>::value;
        };

        template<typename T1, typename T2, typename ... TRest>
        struct unique_types<T1, T2, TRest...> {
            static constexpr bool value =
                unique_types<T1, T2>::value         &&
                unique_types<T1, TRest...>::value   &&
                unique_types<T2, TRest...>::value;
        };

        static_assert(
                unique_types<char, int, float, double>::value,
                "Error : types are not unique");

        ///////////////////////////////////////////////////////////////////

        // === receiver sensor === //

        //! Primary template with a static assertion
        //! for a meaningful error message
        //! if it ever gets instantiated.
        //! We could leave it undefined if we didn't care.

        template<typename, typename R>
        struct is_receiver {
            static_assert(
                std::integral_constant<R, false>::value,
                "Second template parameter needs to be of function type.");
        };

        //! specialization that does the checking
        template<typename C, typename Ret, typename... Args>
        struct is_receiver<C, Ret(Args...)> {
        private:
            template<typename R>
            static constexpr auto check(R*)
            -> typename
                std::is_same<
                    decltype(
                            std::declval<R>().pReceive( std::declval<Args>()... )
                            ),
                    Ret
                >::type;  // attempt to call pReceive and check the result

            template<typename>
            static constexpr std::false_type check(...);

            typedef decltype(check<C>(0)) type;

        public:
            static constexpr bool value = type::value;
        };

        // ======================= //

    }



    template<typename... T>
    struct Poulpe {

        static_assert(poulpe::utils::unique_types<T...>::value == true, "Types must be unique");

        constexpr Poulpe(T&...p) :
        mReceivers(std::tuple<T&...>(p...)) {}

        template<typename signal_t>
        constexpr void pEmit(signal_t &s) {

            static_assert(!std::is_fundamental<signal_t>::value,
                    "Invalid signal type : can't be fundamental type");

            (call<T>(s), ...);
        }

        template<typename signal_t>
        static constexpr std::size_t getReceiverCount() {
            return (((poulpe::utils::is_receiver<T, void(signal_t&)>::value) ? 1 : 0) + ...);
        }

    private:

        template<typename rx_t, typename signal_t>
        typename std::enable_if<!poulpe::utils::is_receiver<rx_t, void(signal_t&)>::value, void>::type
        call(signal_t& s) {}

        template<typename rx_t, typename signal_t>
        typename std::enable_if<poulpe::utils::is_receiver<rx_t, void(signal_t&)>::value, void>::type
        call(signal_t& s) {
            std::get<rx_t&>(mReceivers).pReceive(s);
        }

        const std::tuple<T&...> mReceivers;

    };
}
