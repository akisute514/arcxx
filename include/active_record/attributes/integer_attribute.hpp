#pragma once
/*
 * Active Record C++: https://github.com/akisute514/active_record_cpp
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT Lisence.
 */
#include "attribute_common.hpp"

namespace active_record {
    template<std::same_as<common_adaptor> Adaptor, Attribute Attr>
    requires std::integral<typename Attr::value_type>
    [[nodiscard]] active_record::string to_string(const Attr& attr) {
        return static_cast<bool>(attr) ? std::to_string(attr.value()) : "null";
    }
    template<std::same_as<common_adaptor> Adaptor, Attribute Attr>
    requires std::integral<typename Attr::value_type>
    void from_string(Attr& attr, const active_record::string_view str) {
        if(str != "null" && str != "NULL"){
            typename Attr::value_type tmp = static_cast<typename Attr::value_type>(0);
            std::from_chars(&*str.begin(), &*str.end(), tmp);
            attr = tmp;
        }
    }

    template<typename Model, typename Attribute, std::integral Integer>
    struct attribute<Model, Attribute, Integer> : public attribute_common<Model, Attribute, Integer> {
        struct avg_attribute : public attribute_common<Model, avg_attribute, double>{
            inline static const auto column_name = Attribute::column_name;
            using attribute_common<Model, avg_attribute, double>::attribute_common;
        };
        using attribute_common<Model, Attribute, Integer>::attribute_common;

        inline static const typename attribute_common<Model, Attribute, Integer>::constraint auto_increment = [](const std::optional<Integer>&) constexpr { return false; };

        template<std::derived_from<adaptor> Adaptor = common_adaptor>
        [[nodiscard]] active_record::string to_string() const {
            return active_record::to_string<Adaptor>(*this);
        }
        template<std::derived_from<adaptor> Adaptor = common_adaptor>
        void from_string(const active_record::string_view str) {
            active_record::from_string<Adaptor>(*this, str);
        }

        template<std::convertible_to<Integer> ArgType1, std::convertible_to<Integer> ArgType2>
        [[nodiscard]] static auto between(const ArgType1 value1, const ArgType2 value2){
            query_condition<std::tuple<Attribute, Attribute>> ret;
            ret.bind_attrs =std::make_tuple(static_cast<Attribute>(value1), static_cast<Attribute>(value2));
            ret.condition.push_back(concat_strings(Attribute::column_full_name(), " BETWEEN "));
            ret.condition.push_back(0UL);
            ret.condition.push_back(" AND ");
            ret.condition.push_back(1UL);
            return ret;
        }

        struct sum : public attribute_aggregator<Model, Attribute, sum> {
            inline static decltype(auto) aggregation_func = "sum";
        };
        struct avg : public attribute_aggregator<Model, avg_attribute, avg> {
            inline static decltype(auto) aggregation_func = "avg";
        };
        struct max : public attribute_aggregator<Model, Attribute, max> {
            inline static decltype(auto) aggregation_func = "max";
        };
        struct min : public attribute_aggregator<Model, Attribute, min> {
            inline static decltype(auto) aggregation_func = "min";
        };
    };

    namespace attributes {
        template<typename Model, typename Attribute, std::integral Integer = int32_t>
        struct integer : public attribute<Model, Attribute, Integer>{
            using attribute<Model, Attribute, Integer>::attribute;
        };
    }
}