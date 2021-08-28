#pragma once
#include "../query_relation.hpp"
#include "../query_utils.hpp"
#include "../query_condition_impl.hpp"

namespace active_record {
    /*
     * return type == Scalar
     */

    template<typename Result, Tuple BindAttrs>
    template<Attribute Attr>
    query_relation<Result, active_record::tuple_cat_t<BindAttrs, std::tuple<const Attr*>>> query_relation<Result, BindAttrs>::where(const Attr&& attr) && {
        return std::move(*this).where(Attr::cmp == attr);
    }
    template<typename Result, Tuple BindAttrs>
    template<Attribute Attr>
    query_relation<Result, active_record::tuple_cat_t<BindAttrs, std::tuple<const Attr*>>> query_relation<Result, BindAttrs>::where(const Attr&& attr) const& {
        return this->where(Attr::cmp == attr);
    }

    template<typename Result, Tuple BindAttrs>
    template<Tuple SrcBindAttrs>
    query_relation<Result, active_record::tuple_cat_t<BindAttrs, SrcBindAttrs>> query_relation<Result, BindAttrs>::where(query_condition<SrcBindAttrs>&& cond) &&{
        query_relation<Result, active_record::tuple_cat_t<BindAttrs, SrcBindAttrs>> ret;

        ret.operation = this->operation;
        ret.query_op_arg = std::move(this->query_op_arg);
        ret.query_table = std::move(this->query_table);

        if(!this->query_condition.empty()){
            ret.query_condition = std::move(this->query_condition);
            ret.query_condition.push_back(" AND ");
        }
        const struct {
            std::variant<active_record::string, std::size_t> operator()(active_record::string&& str) const { return std::move(str); }
            std::variant<active_record::string, std::size_t> operator()(std::size_t idx) const noexcept { return idx + std::tuple_size_v<BindAttrs>; }
        } visitor;
        for(auto& cond : cond.condition){
            ret.query_condition.push_back(
                std::visit(visitor, std::move(cond))
            );
        }

        ret.query_options = std::move(this->query_options);
        ret.temporary_attrs = std::move(this->temporary_attrs);
        ret.temporary_attrs.insert(
            ret.temporary_attrs.end(),
            std::make_move_iterator(cond.temporary_attrs.begin()),
            std::make_move_iterator(cond.temporary_attrs.end())
        );
        detail::set_bind_attrs_ptr<0>(ret.bind_attrs, ret.temporary_attrs);
        return ret;
    }

    template<typename Result, Tuple BindAttrs>
    template<Tuple SrcBindAttrs>
    query_relation<Result, active_record::tuple_cat_t<BindAttrs, SrcBindAttrs>> query_relation<Result, BindAttrs>::where(query_condition<SrcBindAttrs>&& cond) const&{
        query_relation<Result, active_record::tuple_cat_t<BindAttrs, SrcBindAttrs>> ret;

        ret.operation = this->operation;
        ret.query_op_arg = this->query_op_arg;
        ret.query_table = this->query_table;

        if(!this->query_condition.empty()){
            ret.query_condition = this->query_condition;
            ret.query_condition.push_back(" AND ");
        }

        const struct {
            std::variant<active_record::string, std::size_t> operator()(active_record::string&& str) const { return std::move(str); }
            std::variant<active_record::string, std::size_t> operator()(std::size_t idx) const noexcept { return idx + std::tuple_size_v<BindAttrs>; }
        } visitor;
        for(auto& cond : cond.condition){
            ret.query_condition.push_back(
                std::visit(visitor, std::move(cond))
            );
        }

        ret.query_options = this->query_options;
        ret.temporary_attrs = this->temporary_attrs;
        ret.temporary_attrs.insert(
            ret.temporary_attrs.end(),
            std::make_move_iterator(cond.temporary_attrs.begin()),
            std::make_move_iterator(cond.temporary_attrs.end())
        );
        detail::set_bind_attrs_ptr<0>(ret.bind_attrs, ret.temporary_attrs);
        return ret;
    }
}