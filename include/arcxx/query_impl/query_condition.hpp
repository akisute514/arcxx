#pragma once
/*
 * ARCXX: https://github.com/akisute514/arcxx
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT License.
 */
#include "../query.hpp"

namespace arcxx {
    template<specialized_from<std::tuple> BindAttrs>
    struct query_condition {
    private:
        enum class conjunction{
            AND,
            OR
        };

        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] query_condition<tuptup::tuple_cat_t<BindAttrs, SrcBindAttrs>> concat_conditions(query_condition<SrcBindAttrs>&&, const conjunction);
    public:
        using str_or_bind = std::variant<arcxx::string, std::size_t>;
        std::vector<str_or_bind> condition;
        BindAttrs bind_attrs;

        [[nodiscard]] static consteval std::size_t bind_attrs_count() noexcept {
            return std::tuple_size_v<BindAttrs>;
        }

        template<specialized_from<std::tuple> DestBindAttrs>
        [[nodiscard]] auto operator&&(query_condition<DestBindAttrs>&& cond) && {
            return concat_conditions(std::move(cond), conjunction::AND);
        }
        template<specialized_from<std::tuple> DestBindAttrs>
        [[nodiscard]] auto operator||(query_condition<DestBindAttrs>&& cond) && {
            return concat_conditions(std::move(cond), conjunction::OR);
        }
    };
}

#include "query_condition_impl.ipp"