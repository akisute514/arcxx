=======================================
arcxx::postgresql_connector
=======================================

.. cpp:class:: postgresql_connector

    .. code-block:: cpp

        class postgresql_connector : public connector;

    This class is database connector class for PostgreSQL.
    "libpq-dev" is required to use this class.


    .. cpp:function:: open()

        .. code-block:: cpp

            static postgresql_connector open(
                const PostgreSQL::endpoint endpoint_info,
                const std::optional<PostgreSQL::auth> auth_info = std::nullopt,
                const std::optional<PostgreSQL::options> option = std::nullopt
            )
            static postgresql_connector open(const arcxx::string& connection_info)

    .. cpp:function:: close()


    .. cpp:function:: protocol_version()

        Return PQprotocolVersion.

        .. code-block:: cpp

            int protocol_version() const;

    .. cpp:function:: server_version()

        Return PQserverVersion.

        .. code-block:: cpp

            int server_version() const;

    .. cpp:function:: exec()

        .. code-block:: cpp
            
            template<typename Result, specialized_from<std::tuple> BindAttrs>
            auto exec(const query_relation<Result, BindAttrs>& query) -> arcxx::expected<Result, arcxx::string>;

    .. cpp:function:: create_table()

        .. code-block:: cpp

            template<is_model Mod>
            auto create_table(decltype(abort_if_exist)) -> arcxx::expected<void, arcxx::string>;
            
            template<is_model Mod>
            auto create_table() -> arcxx::expected<void, arcxx::string>;

    .. cpp:function:: drop_table()

        .. code-block:: cpp

            template<is_model Mod>
            auto drop_table() -> -> arcxx::expected<void, arcxx::string>;

    .. cpp:function:: transaction()

        .. code-block:: cpp

            template<typename F>
            requires std::convertible_to<F, std::function<transaction::detail::commit_or_rollback_t()>>
            auto transaction(F&& func) -> arcxx::expected<void, arcxx::string>;
            
            template<typename F>
            requires std::convertible_to<F, std::function<transaction::detail::commit_or_rollback_t(postgresql_connector&)>>
            auto transaction(F&& func) -> arcxx::expected<void, arcxx::string>;
