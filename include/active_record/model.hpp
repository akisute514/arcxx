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
#include "query.hpp"
#include "attribute.hpp"

namespace active_record {
    template<typename Derived>
    class model {
    private:
        struct has_table_name_impl {
            template<typename S>
            static decltype(S::table_name, std::true_type{}) check(S);
            static std::false_type check(...);
            static constexpr bool value = decltype(check(std::declval<Derived>()))::value;
        };

        [[nodiscard]] static constexpr active_record::string insert_column_names_to_string();

    public:
        struct schema {
            template<std::derived_from<adaptor> Adaptor>
            [[nodiscard]] static active_record::string to_sql(bool abort_if_exist = true);
        };

        static constexpr bool has_table_name = has_table_name_impl::value;
        [[nodiscard]] static constexpr auto column_names() noexcept;

        auto attributes_as_tuple() noexcept {
            using namespace tuptup::type_placeholders;
            auto attributes_tuple = tuptup::struct_binder<Derived>{}(*reinterpret_cast<Derived*>(this));
            return tuptup::tuple_filter<is_attribute<defer<std::remove_reference<_1>>>>(attributes_tuple);
        }
        auto attributes_as_tuple() const noexcept {
            using namespace tuptup::type_placeholders;
            const auto attributes_tuple = tuptup::struct_binder<Derived>{}(*reinterpret_cast<const Derived*>(this));
            return tuptup::tuple_filter<is_attribute<defer<std::remove_reference<_1>>>>(attributes_tuple);
        }

        [[nodiscard]] static auto insert(const Derived& model);
        [[nodiscard]] static auto insert(Derived&& model);

        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<>> all();

        template<Attribute... Attrs>
        [[nodiscard]] static query_relation<std::vector<std::tuple<Attrs...>>, std::tuple<>> select();

        template<AttributeAggregator... Aggregators>
        [[nodiscard]] static query_relation<std::tuple<typename Aggregators::attribute_type...>, std::tuple<>> select();

        template<Attribute Attr>
        [[nodiscard]] static query_relation<std::vector<Attr>, std::tuple<>> pluck();

        // delete is identifier word
        template<Attribute Attr>
        [[nodiscard]] static query_relation<bool, std::tuple<Attr>> destroy(const Attr&&);
        template<Tuple SrcBindAttrs>
        [[nodiscard]] static query_relation<bool, SrcBindAttrs> destroy(query_condition<SrcBindAttrs>&&);

        template<Attribute Attr>
        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<Attr>> where(const Attr&);
        template<Tuple SrcBindAttrs>
        [[nodiscard]] static query_relation<std::vector<Derived>, SrcBindAttrs> where(query_condition<SrcBindAttrs>&&);

        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<>> limit(const std::size_t);

        template<Attribute Attr>
        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<>> order_by(const active_record::order = active_record::order::asc);

        template<typename ReferModel>
        requires std::derived_from<ReferModel, model<ReferModel>>
        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<>> join();

        template<typename ReferModel>
        requires std::derived_from<ReferModel, model<ReferModel>>
        [[nodiscard]] static query_relation<std::vector<Derived>, std::tuple<>> left_join();

        template<Attribute Attr>
        [[nodiscard]] static query_relation<std::unordered_map<Attr, std::tuple<>>, std::tuple<>> group_by();

        [[nodiscard]] static query_relation<std::size_t, std::tuple<>> count();

        template<Attribute Attr>
        requires requires{ typename Attr::sum; }
        [[nodiscard]] static query_relation<typename Attr::sum::attribute_type, std::tuple<>> sum();

        template<Attribute Attr>
        requires requires{ typename Attr::avg; }
        [[nodiscard]] static query_relation<typename Attr::avg::attribute_type, std::tuple<>> avg();

        template<Attribute Attr>
        requires requires{ typename Attr::max; }
        [[nodiscard]] static query_relation<typename Attr::max::attribute_type, std::tuple<>> max();

        template<Attribute Attr>
        requires requires{ typename Attr::min; }
        [[nodiscard]] static query_relation<typename Attr::min::attribute_type, std::tuple<>> min();
    };

    template<typename T>
    concept Model = requires {
        std::derived_from<T, model<T>>;
        T::has_table_name;
    };
}