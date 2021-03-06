#pragma once
/*
 * ARCXX: https://github.com/akisute514/arcxx
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT License.
 */
namespace arcxx {
    template<typename Result, specialized_from<std::tuple> BindAttrs>
    struct query_relation : public query_relation_common<BindAttrs> {
        using result_type = Result;
        using query_relation_common<BindAttrs>::query_relation_common;
        template<is_connector Connector>
        [[nodiscard]] auto exec(Connector& conn) const {
            return conn.exec(*this);
        }

        /* scalar */
        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) &&;
        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) const&;

        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) &&;
        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) const&;

        /* vector */
        template<is_attribute... Attrs>
        [[nodiscard]] auto select() && requires specialized_from<Result, std::vector>;
        template<is_attribute... Attrs>
        [[nodiscard]] auto select() const & requires specialized_from<Result, std::vector>;

        template<is_attribute_aggregator... Attrs>
        [[nodiscard]] auto select() && requires specialized_from<Result, std::vector>;
        template<is_attribute_aggregator... Attrs>
        [[nodiscard]] auto select() const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        [[nodiscard]] auto pluck() && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        [[nodiscard]] auto pluck() const& requires specialized_from<Result, std::vector>;
        template<is_attribute_aggregator Attr>
        [[nodiscard]] auto pluck() && requires specialized_from<Result, std::vector>;
        template<is_attribute_aggregator Attr>
        [[nodiscard]] auto pluck() const& requires specialized_from<Result, std::vector>;

        template<is_attribute... Attrs>
        requires is_model<typename Result::value_type>
        [[nodiscard]] auto update(const Attrs&...) && requires specialized_from<Result, std::vector>;
        template<is_attribute... Attrs>
        requires is_model<typename Result::value_type>
        [[nodiscard]] auto update(const Attrs&...) const& requires specialized_from<Result, std::vector>;


        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) const& requires specialized_from<Result, std::vector>;

        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) && requires specialized_from<Result, std::vector>;
        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) const& requires specialized_from<Result, std::vector>;

        [[nodiscard]] auto limit(const std::size_t) && requires specialized_from<Result, std::vector>;
        [[nodiscard]] auto limit(const std::size_t) const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        [[nodiscard]] auto order_by(const arcxx::order = arcxx::order::asc) && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        [[nodiscard]] auto order_by(const arcxx::order = arcxx::order::asc) const& requires specialized_from<Result, std::vector>;

        [[nodiscard]] auto count() && requires specialized_from<Result, std::vector>;
        [[nodiscard]] auto count() const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        requires requires{ typename Attr::sum; }
        [[nodiscard]] auto sum() && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        requires requires{ typename Attr::sum; }
        [[nodiscard]] auto sum() const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        requires requires{ typename Attr::avg; }
        [[nodiscard]] auto avg() && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        requires requires{ typename Attr::avg; }
        [[nodiscard]] auto avg() const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        requires requires{ typename Attr::max; }
        [[nodiscard]] auto max() && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        requires requires{ typename Attr::max; }
        [[nodiscard]] auto max() const& requires specialized_from<Result, std::vector>;

        template<is_attribute Attr>
        requires requires{ typename Attr::min; }
        [[nodiscard]] auto min() && requires specialized_from<Result, std::vector>;
        template<is_attribute Attr>
        requires requires{ typename Attr::min; }
        [[nodiscard]] auto min() const& requires specialized_from<Result, std::vector>;

        /* map */
        template<is_attribute_aggregator... Attrs>
        [[nodiscard]] auto select() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute_aggregator... Attrs>
        [[nodiscard]] auto select() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute_aggregator Attr>
        [[nodiscard]] auto pluck() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute_aggregator Attr>
        [[nodiscard]] auto pluck() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        [[nodiscard]] auto where(const Attr&) const& requires specialized_from<Result, std::unordered_map>;

        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) && requires specialized_from<Result, std::unordered_map>;
        template<specialized_from<std::tuple> SrcBindAttrs>
        [[nodiscard]] auto where(query_condition<SrcBindAttrs>&&) const& requires specialized_from<Result, std::unordered_map>;

        [[nodiscard]] auto limit(const std::size_t) && requires specialized_from<Result, std::unordered_map>;
        [[nodiscard]] auto limit(const std::size_t) const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        [[nodiscard]] auto order_by(const arcxx::order = arcxx::order::asc) && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        [[nodiscard]] auto order_by(const arcxx::order = arcxx::order::asc) const& requires specialized_from<Result, std::unordered_map>;

        [[nodiscard]] auto count() && requires specialized_from<Result, std::unordered_map>;
        [[nodiscard]] auto count() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        requires requires{ typename Attr::sum; }
        [[nodiscard]] auto sum() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        requires requires{ typename Attr::sum; }
        [[nodiscard]] auto sum() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        requires requires{ typename Attr::avg; }
        [[nodiscard]] auto avg() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        requires requires{ typename Attr::avg; }
        [[nodiscard]] auto avg() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        requires requires{ typename Attr::max; }
        [[nodiscard]] auto max() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        requires requires{ typename Attr::max; }
        [[nodiscard]] auto max() const& requires specialized_from<Result, std::unordered_map>;

        template<is_attribute Attr>
        requires requires{ typename Attr::min; }
        [[nodiscard]] auto min() && requires specialized_from<Result, std::unordered_map>;
        template<is_attribute Attr>
        requires requires{ typename Attr::min; }
        [[nodiscard]] auto min() const& requires specialized_from<Result, std::unordered_map>;
    };
}

#include "query_relation_impls/result_scalar.ipp"
#include "query_relation_impls/result_vector.ipp"
#include "query_relation_impls/result_map.ipp"