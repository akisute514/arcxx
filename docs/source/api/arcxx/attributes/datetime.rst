===================================
arcxx::attributes::datetime
===================================

.. cpp:struct:: template<typename Model, typename Attribute> \
                datetime

    datetime attribute.

    .. list-table:: Member functions

        * - :ref:`(constructor) <datetime_constructors>`
          - constructs the datetime attribute
        * - :cpp:func:`operator=`
          - 
        * - :ref:`(destructor) <datetime_destructors>`
          - destroy the attribute

    .. list-table:: Member variables

        * - :cpp:var:`data`
          - :code:`std::optional<arcxx::datetime>`

    .. list-table:: Member types

        * - :cpp:type:`model_type`
          - :code:`Model`
        * - :cpp:type:`attribute_type`
          - :code:`Attribute`
        * - :cpp:type:`value_type`
          - :code:`arcxx::datetime`
        * - :cpp:type:`constraint`
          - :code:`std::function<bool(const std::optional<arcxx::datetime>&)>`

    .. list-table:: Observers

        * - :cpp:func:`operator bool`
          - check whether null
        * - :cpp:func:`value`
          - returns the contained value
        
    .. list-table:: Condition Queries
        
        * - :cpp:func:`in`
          - generate in condition
        * - :cpp:func:`between`
          - generate between condition
        * - :cpp:func:`operator&&`
          - 
        * - :cpp:func:`operator||`
          -

    .. list-table:: Constraints

        * - :cpp:func:`default_value`
          - generate a default value constraint
        * - :cpp:var:`not_null`
          -
        * - :cpp:var:`unique`
          -
        * - :cpp:var:`primary_key`
          -

    .. list-table:: String mutual converts

        * - :cpp:func:`to_string`
          - converts to string
        * - :cpp:func:`from_string`
          - converts from string

    .. _datetime_constructors:
    .. cpp:function:: datetime()

        .. code-block:: cpp

            constexpr datetime();
            constexpr datetime(const std::optional<arcxx::datetime>&);
            constexpr datetime(const std::optional<arcxx::datetime>&&);
            constexpr datetime(std::nullopt_t);

            constexpr datetime(const arcxx::datetime&);
            constexpr datetime(const arcxx::datetime&&);
        
    .. cpp:function:: operator=()
    
        .. code-block:: cpp

            Attribute& operator=(const Attribute&);
            Attribute& operator=(Attribute&&);

            Attribute& operator=(const std::optional<arcxx::datetime>&);
            Attribute& operator=(const std::optional<arcxx::datetime>&&);
            Attribute& operator=(std::nullopt_t);

            Attribute& operator=(const arcxx::datetime&);
            Attribute& operator=(const arcxx::datetime&&);

    .. _datetime_destructors:
    .. cpp:function:: ~datetime()
        
        .. code-block:: cpp

            constexpr virtual ~datetime();

    .. cpp:function:: operator bool()

        .. code-block:: cpp

            constexpr operator bool() const noexcept;
        
        Return false if attribute value is null.


    .. cpp:function:: value()

        .. code-block:: cpp

            [[nodiscard]] arcxx::datetime& value() &;
            [[nodiscard]] const arcxx::datetime& value() const&;
            [[nodiscard]] arcxx::datetime&& value() &&;

        Return attribute value.
        If the value is null, throw :code:`std::bad_optional_access`.

    .. cpp:function:: in()

        .. code-block:: cpp

            template<typename... Attrs>
            query_condition in(const Attrs&&... args);

        The returned object will generate
        :code:`Attribute::column_name IN (args...)`
    

    .. cpp:function:: between()
    
        .. code-block:: cpp

            query_condition between(std::convertible_to<arcxx::datetime> arg1, std::convertible_to<arcxx::datetime> arg2);

        The returned object will generate
        :code:`Attribute::column_name BETWEEN arg1 AND arg2`

    .. cpp:function:: operator&&()
    
        .. code-block:: cpp

            query_condition operator&&(const query_condition& condition); // (1)
            
            template<typename Arg>
            query_condition operator&&(const Arg& arg); // (2)

        The returned object will generate "AND condition SQL".
        
        .. code-block:: sql
            
            (Attribute::column_name = this AND condition) -- (1)
            (Attribute::column_name = this AND Attr::column_name = arg) -- (2)

    .. cpp:function:: operator||()
    
        .. code-block:: cpp

            query_condition operator||(const query_condition& condition); // (1)
            
            template<typename Arg>
            query_condition operator||(const Arg& arg); // (2)

        The returned object will generate "OR condition SQL".
        
        .. code-block:: sql
            
            (Attribute::column_name = this OR condition) -- (1)
            (Attribute::column_name = this OR Attr::column_name = arg) -- (2)

        
    .. cpp:function:: to_string()

        .. code-block:: cpp

            [[nodiscard]] arcxx::string to_string() const;

        Converts attribute value to string.
        
    .. cpp:function:: from_string()

        .. code-block:: cpp

            void from_string(const arcxx::string_view str);

        Converts string to attribute value.
        