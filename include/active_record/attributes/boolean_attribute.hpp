#pragma once
#include "../attribute.hpp"
#include "../query.hpp"

namespace active_record {
    template<typename Model, typename Attribute>
    struct attribute<Model, Attribute, bool> : public attribute_common<Model, Attribute, bool> {
        using attribute_common<Model, Attribute, bool>::attribute_common;
        
        [[nodiscard]] constexpr virtual active_record::string to_string() const override {
            return static_cast<bool>(*this) ? (this->value() ? "true" : "false") : "null";
        }
        virtual void from_string(const active_record::string& str) override {
            if(str != "null" && str != "NULL"){
                this->data = (str == "false" || str == "0") ? false : true;
            }
        }
    };

    namespace attributes {
        template<typename Model, typename Attribute>
        struct boolean : public attribute<Model, Attribute, bool>{
            using attribute<Model, Attribute, bool>::attribute;
        };
    }
}