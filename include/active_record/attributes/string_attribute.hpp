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
    requires std::same_as<typename Attr::value_type, active_record::string>
    [[nodiscard]] active_record::string to_string(const Attr& attr) {
        // require sanitize
        return static_cast<bool>(attr) ? concat_strings("\'", active_record::sanitize(attr.value()), "\'") : "null";
    }
    template<std::same_as<common_adaptor> Adaptor, Attribute Attr>
    requires std::same_as<typename Attr::value_type, active_record::string>
    void from_string(Attr& attr, const active_record::string_view str) {
        if(str != "null" && str != "NULL"){
            attr = active_record::string{ str };
        }
    }

    template<typename Model, typename Attribute>
    struct attribute<Model, Attribute, active_record::string> : public attribute_common<Model, Attribute, active_record::string> {
        using attribute_common<Model, Attribute, active_record::string>::attribute_common;

        template<std::convertible_to<active_record::string> StringType>
        requires (!std::convertible_to<StringType, std::optional<active_record::string>>)
        constexpr attribute(const StringType& default_value) : attribute_common<Model, Attribute, active_record::string>(active_record::string{ default_value }) {}

        struct constraint_length_impl {
            const std::size_t length;
            constexpr bool operator()(const std::optional<active_record::string>& t) {
                return static_cast<bool>(t) && t.value().length() <= length;
            }
        };
        [[nodiscard]] static const typename attribute_common<Model, Attribute, active_record::string>::constraint length(const std::size_t len) noexcept {
            return constraint_length_impl{ len };
        };

        template<std::convertible_to<active_record::string> StringType>
        [[nodiscard]] static constexpr query_condition<std::tuple<Attribute>> like(const StringType& value){
            query_condition<std::tuple<Attribute>> ret;
            ret.bind_attrs = std::make_tuple<Attribute>(active_record::string{ value });
            ret.condition.push_back(concat_strings(Attribute::column_full_name(), " LIKE "));
            ret.condition.push_back(static_cast<std::size_t>(0));
            return ret;
        }

        template<std::derived_from<adaptor> Adaptor = common_adaptor>
        [[nodiscard]] active_record::string to_string() const {
            return active_record::to_string<Adaptor>(*this);
        }
        template<std::derived_from<adaptor> Adaptor = common_adaptor>
        void from_string(const active_record::string_view str) {
            active_record::from_string<Adaptor>(*this, str);
        }
    };

    namespace attributes {
        template<typename Model, typename Attribute>
        struct string : public attribute<Model, Attribute, active_record::string>{
            using attribute<Model, Attribute, active_record::string>::attribute;
        };
    }
}