#pragma once
#include <utility>
#include <type_traits>
/*
 * tuptup: https://github.com/akisute514/tuptup/
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT Lisence.
 */

namespace tuptup {
#if (defined(__cplusplus) && __cplusplus < 201402L)  || (defined(_MSC_VER) && _MSVC_LANG < 201402L)
    template<typename Int, Int... I>
    struct integer_sequence {
        using value_type = Int;
        static constexpr std::size_t size() noexcept { return sizeof...(I); }
    };
#else
    template<typename Int, Int... I>
    using integer_sequence = std::integer_sequence<Int, I...>;
#endif

    namespace detail {
        template<typename IntSeq>
        struct integer_sequence_impl;
        template<typename Int, Int... Values>
        struct integer_sequence_impl<integer_sequence<Int, Values...>>{
            using type = integer_sequence<Int, Values...>;

            template<Int Last>
            struct push_back {
                using type = integer_sequence<Int, Values..., Last>;
            };
            template<Int First>
            struct push_front {
                using type = integer_sequence<Int, First, Values...>;
            };
            
            template<typename T>
            struct concat;
            template<Int... RightValues>
            struct concat<integer_sequence<Int, RightValues...>>{
                using type = integer_sequence<Int, Values..., RightValues...>;
            };

            template<typename F>
            struct for_each {
                using type = integer_sequence<Int, F{}(Values)...>;
            };
        };

        template<typename IntSeq, typename Next, typename Last, typename = void>
        struct integer_range_impl;
        template<typename Int, Int Next, Int Last, Int... Values>
        struct integer_range_impl<integer_sequence<Int, Values...>, std::integral_constant<Int, Next>, std::integral_constant<Int, Last>, typename std::enable_if<(Next >= Last)>::type>{
            using type = integer_sequence<Int, Values...>;
        };
        template<typename Int, Int Next, Int Last, Int... Values>
        struct integer_range_impl<integer_sequence<Int, Values...>, std::integral_constant<Int, Next>, std::integral_constant<Int, Last>, typename std::enable_if<(Next < Last)>::type>{
            using type = typename integer_range_impl<integer_sequence<Int, Values..., Next>, std::integral_constant<Int, Next+1>, std::integral_constant<Int, Last>>::type;
        };
    }
    template<typename IntSeq, typename IntSeq::value_type Last>
    using integer_sequence_push_back = typename detail::integer_sequence_impl<IntSeq>::template push_back<Last>::type;
    template<typename IntSeq, typename IntSeq::value_type First>
    using integer_sequence_push_front = typename detail::integer_sequence_impl<IntSeq>::template push_front<First>::type;

    template<typename IntSeqLeft, typename IntSeqRight>
    using integer_sequence_cat = typename detail::integer_sequence_impl<IntSeqLeft>::template concat<IntSeqRight>::type;


    template<typename Int, Int First, Int Last>
    using make_integer_range = typename detail::integer_range_impl<integer_sequence<Int>, std::integral_constant<Int, First>, std::integral_constant<Int, Last>>::type;

    template<typename Int, Int N>
    using make_integer_sequence = typename detail::integer_range_impl<integer_sequence<Int>, std::integral_constant<Int, 0>, std::integral_constant<Int, N>>::type;

    template<typename F, typename IntSeq>
    using integer_sequence_for_each = typename detail::integer_sequence_impl<IntSeq>::template for_each<F>::type;

    template<std::size_t... I>
    using index_sequence = integer_sequence<std::size_t, I...>;
    template<std::size_t N>
    using make_index_sequence = make_integer_sequence<std::size_t, N>;
    template<std::size_t First, std::size_t Last>
    using make_index_range = make_integer_range<std::size_t, First, Last>;

    template<typename... T>
    using index_sequence_for = make_index_sequence<sizeof...(T)>;
}