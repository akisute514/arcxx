#pragma once
/*
 * ARCXX: https://github.com/akisute514/arcxx
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT License.
 */
#include <libpq-fe.h>
#include <any>
#include <bit>
#if !(defined(_WIN32) || defined(_WIN64))
#include <byteswap.h>
#endif
#include "arcxx/query_impl/query_relation.hpp"
#include "string_convertors.hpp"

namespace arcxx::PostgreSQL::detail {
    template<is_attribute Attr>
    constexpr std::size_t attribute_size([[maybe_unused]] const Attr&) noexcept { return sizeof(typename Attr::value_type); }
    template<is_attribute Attr>
    requires regarded_as_clock<typename Attr::value_type>
    constexpr std::size_t attribute_size(const Attr&){ return sizeof("YYYY-MM-DD hh:mm:ss"); }
    template<is_attribute Attr>
    requires std::invocable<typename Attr::value_type::size>
    constexpr std::size_t attribute_size(const Attr& attr){ return attr ? attr.value().size() : 0; }

    template<std::size_t Bytes> struct uint{};
    template<> struct uint<2> { using type = uint16_t; };
    template<> struct uint<4> { using type = uint32_t; };
    template<> struct uint<8> { using type = uint64_t; };

    template<std::integral T>
    [[nodiscard]] inline auto byte_swap(const T h) noexcept -> decltype(h) {
        if constexpr (std::endian::native == std::endian::little) {
            #if defined(_WIN32) || defined(_WIN64)
            if constexpr (sizeof(h) == sizeof(uint16_t)) return _byteswap_ushort(h);
            else if constexpr (sizeof(h) == sizeof(uint32_t)) return _byteswap_ulong(h);
            else if constexpr (sizeof(h) == sizeof(uint64_t)) return _byteswap_uint64(h);
            #else
            if constexpr (sizeof(h) == sizeof(uint16_t)) return bswap_16(h);
            else if constexpr (sizeof(h) == sizeof(uint32_t)) return bswap_32(h);
            else if constexpr (sizeof(h) == sizeof(uint64_t)) return bswap_64(h);
            #endif
        }
        else return h;
    }

    template<is_attribute Attr>
    requires std::same_as<typename Attr::value_type, arcxx::string>
    [[nodiscard]] inline auto get_value_ptr(const Attr& attr, [[maybe_unused]]std::any&) {
        if (!attr) return static_cast<const char*>(nullptr);
        return attr.value().c_str();
    }
    template<is_attribute Attr>
    requires std::integral<typename Attr::value_type>
    [[nodiscard]] inline auto get_value_ptr(const Attr& attr, [[maybe_unused]]std::any& tmp) {
        if (!attr) return static_cast<const char*>(nullptr);
        const auto tmp_value = byte_swap(attr.value());
        tmp = tmp_value;
        return reinterpret_cast<const char*>(std::any_cast<decltype(tmp_value)>(&tmp));
    }
    template<is_attribute Attr>
    requires std::floating_point<typename Attr::value_type>
    [[nodiscard]] inline auto get_value_ptr(const Attr& attr, [[maybe_unused]]std::any& tmp) {
        if (!attr) return static_cast<const char*>(nullptr);
        // PostgreSQL use IEE 754
        if constexpr(std::numeric_limits<typename Attr::value_type>::is_iec559){
            const auto tmp_value = byte_swap(std::bit_cast<typename uint<sizeof(typename Attr::value_type)>::type>(attr.value()));
            tmp = tmp_value;
            return reinterpret_cast<const char*>(std::any_cast<decltype(tmp_value)>(&tmp));
        }
        else{
            static_assert(std::bool_constant<(Attr{},false)>{}/*lazy instantiation*/, "Buy a machine that using IEE 754 as float format!");
        }
    }
    template<is_attribute Attr>
    requires regarded_as_clock<typename Attr::value_type>
    [[nodiscard]] inline auto get_value_ptr(const Attr& attr, [[maybe_unused]]std::any& tmp) {
        // YYYY-MM-DD hh:mm:ss (UTC or GMT)
        if (!attr) return static_cast<const char*>(nullptr);
        tmp = to_string<postgresql_connector>(attr);
        return reinterpret_cast<const char*>(std::any_cast<arcxx::string&>(tmp).c_str());
    }
}