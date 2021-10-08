#pragma once
/*
 * Copyright 2021 akisute514
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "query_relation_impl/result_vector.hpp"
#include "query_relation_impl/result_scalar.hpp"
#include "query_relation_impl/result_map.hpp"

namespace active_record {
    template<typename Result, typename... Args>
    requires ((Attribute<Args> || std::convertible_to<Args, active_record::string_view>) && ...)
    auto raw_query(Args&&... args){
        using namespace tuptup::type_placeholders;
        using bind_attrs_t = tuptup::tuple_filter_t<is_attribute<_1>, std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...>>;
        
        query_relation<Result, bind_attrs_t> ret{ query_operation::unspecified };
        std::size_t index = 0;
        ret.op_args.reserve(sizeof...(Args));
        ret.op_args = {
            [&index]<typename Arg>(const Arg& arg) -> decltype(ret)::str_or_bind {
                if constexpr (is_attribute<Arg>::value) {
                    ++index;
                    return index;
                }
                else return active_record::string{ arg };
            }(args)...
        };
        ret.bind_attrs = tuptup::tuple_filter<is_attribute<_1>>(std::make_tuple(std::forward<Args>(args)...));

        return ret;
    }

    template<Tuple BindAttrs>
    template<std::derived_from<adaptor> Adaptor>
    struct query_relation_common<BindAttrs>::sob_to_string_impl {
        struct visitor_impl;
        visitor_impl visitor;

        struct lazy_string_view {
            const active_record::string& str;
            const active_record::string::difference_type begin; // from begin(str)
            const active_record::string::difference_type end; // from begin(str)
            operator active_record::string_view() const {
                return active_record::string_view{ str.begin() + begin, str.begin() + end };
            }
        };

        lazy_string_view to_string(const std::vector<str_or_bind>& sobs) {
            const auto begin = std::distance(visitor.buff.cbegin(), visitor.buff.cend());
            for(const auto& sob : sobs) {
                std::visit(visitor, sob);
            }
            return lazy_string_view {
                .str = visitor.buff,
                .begin = begin,
                .end = std::distance(visitor.buff.cbegin(), visitor.buff.cend())
            };
        }
    };

    template<Tuple BindAttrs>
    template<std::derived_from<adaptor> Adaptor>
    struct query_relation_common<BindAttrs>::sob_to_string_impl<Adaptor>::visitor_impl {
        const std::array<active_record::string, std::tuple_size_v<BindAttrs>> attr_strings;
        active_record::string buff;

        visitor_impl(const BindAttrs& bind_attr) :
            attr_strings([&bind_attr](){
                if constexpr (Adaptor::bindable){
                    std::array<active_record::string, std::tuple_size_v<BindAttrs>> ret;
                    for(std::size_t i = 0; i < std::tuple_size_v<BindAttrs>; ++i){
                        ret[i] = Adaptor::bind_variable_str(i);
                    }
                    return ret;
                }
                else {
                    return std::apply([]<typename... Attrs>(const Attrs&... attrs) {
                        return std::array<active_record::string, std::tuple_size_v<BindAttrs>>{ active_record::to_string<Adaptor>(attrs)... };
                    }, bind_attr);
                }
            }()){
            const auto bit_ceil = [](std::size_t v) -> std::size_t{
                if (v <= 4) return 4; // minimum reserve size
                return std::bit_ceil(v);
            };
            buff.reserve(bit_ceil(std::transform_reduce(
                attr_strings.begin(), attr_strings.end(), static_cast<std::size_t>(0),
                [](auto acc, const auto len){ return acc += len; },
                [](const auto& str){ return str.length(); }
            )) * 2);
        }

        void operator()(const active_record::string& str) {
            buff += str;
        }
        void operator()(const std::size_t idx) {
            buff += attr_strings[idx];
        }
    };
}