#pragma once
/*
 * Active Record C++: https://github.com/akisute514/active_record_cpp
 * Copyright (c) 2021 akisute514
 * 
 * Released under the MIT License.
 */
#include <libpq-fe.h>
#include "postgresql/schema.hpp"
#include "postgresql/utils.hpp"

namespace active_record {
    namespace PostgreSQL {
        /*
         * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         * DO NOT HARD CORD SENSITIVE INFORMATION
         * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         */
        struct endpoint {
            const active_record::string server_name;
            const active_record::string port = "5432";
            const active_record::string db_name;
        };

        struct auth {
            const active_record::string user;
            const active_record::string password;
        };

        struct options {
            const active_record::string option;
            const active_record::string debug_of;
        };
    }

    class postgresql_connector : public connector {
    private:
        ::PGconn* conn = nullptr;
        std::optional<active_record::string> error_msg = std::nullopt;

        postgresql_connector() = delete;
        postgresql_connector(const PostgreSQL::endpoint& endpoint_info, const std::optional<PostgreSQL::auth>& auth_info, const std::optional<PostgreSQL::options> option);
        postgresql_connector(const active_record::string& info);

        template<typename Result, specialized_from<std::tuple> BindAttrs>
        PGresult* exec_sql(const query_relation<Result, BindAttrs>& query);

    public:
        bool has_error() const noexcept;
        const active_record::string& error_message() const;

        static postgresql_connector open(const PostgreSQL::endpoint endpoint_info, const std::optional<PostgreSQL::auth> auth_info = std::nullopt, const std::optional<PostgreSQL::options> option = std::nullopt);
        static postgresql_connector open(const active_record::string& connection_info);

        int protocol_version() const;
        int server_version() const;

        ~postgresql_connector();

        void close();

        static constexpr bool bindable = true;
        static active_record::string bind_variable_str(const std::size_t idx, active_record::string&& buff = {});

        template<is_model Mod>
        active_record::expected<void, active_record::string> create_table(decltype(abort_if_exists));
        template<is_model Mod>
        active_record::expected<void, active_record::string> create_table();
        template<is_model Mod>
        active_record::expected<void, active_record::string> drop_table();

        template<is_model Mod>
        bool exists_table();

        template<typename Result, specialized_from<std::tuple> BindAttrs>
        active_record::expected<Result, active_record::string> exec(const query_relation<Result, BindAttrs>& query);

        template<specialized_from<std::tuple> BindAttrs>
        active_record::expected<void, active_record::string> exec(const query_relation<void, BindAttrs>& query);

        active_record::expected<void, active_record::string> begin();
        active_record::expected<void, active_record::string> commit();
        active_record::expected<void, active_record::string> rollback();

        template<typename F>
        requires std::convertible_to<F, std::function<transaction::detail::commit_or_rollback_t()>>
        active_record::expected<void, active_record::string> transaction(F&& func);
        template<typename F>
        requires std::convertible_to<F, std::function<transaction::detail::commit_or_rollback_t(postgresql_connector&)>>
        active_record::expected<void, active_record::string> transaction(F&& func);

        template<is_attribute Attr>
        static active_record::string column_definition();
    };

    namespace PostgreSQL {
        using connector = postgresql_connector;
    }
}

#include "postgresql/connector.ipp"