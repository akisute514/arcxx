==================================
arcxx::attributes::boolean
==================================

.. cpp:struct:: template<typename Model, typename Attribute> \
                boolean

    boolean attribute.

    .. list-table:: Member functions

        * - :ref:`(constructor) <boolean_constructors>`
          - constructs the boolean attribute
        * - :cpp:func:`operator=`
          - 
        * - :ref:`(destructor) <boolean_destructors>`
          - destroy the attribute

    .. list-table:: Member variables

        * - :cpp:var:`data`
          - :code:`std::optional<bool>`

    .. list-table:: Member types

        * - :cpp:type:`model_type`
          - :code:`Model`
        * - :cpp:type:`attribute_type`
          - :code:`Attribute`
        * - :cpp:type:`value_type`
          - :code:`bool`
        * - :cpp:type:`constraint`
          - :code:`std::function<bool(const std::optional<bool>&)>`


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

    .. _boolean_constructors:
    .. cpp:function:: boolean()

        .. code-block:: cpp

            constexpr boolean();
            constexpr boolean(const std::optional<bool>&);
            constexpr boolean(const std::optional<bool>&&);
            constexpr boolean(std::nullopt_t);

            constexpr boolean(const bool&);
            constexpr boolean(const bool&&);
        
    .. cpp:function:: operator=()
    
        .. code-block:: cpp

            Attribute& operator=(const Attribute&);
            Attribute& operator=(Attribute&&);

            Attribute& operator=(const std::optional<bool>&);
            Attribute& operator=(const std::optional<bool>&&);
            Attribute& operator=(std::nullopt_t);

            Attribute& operator=(const bool&);
            Attribute& operator=(const bool&&);

    .. _boolean_destructors:
    .. cpp:function:: ~boolean()
        
        .. code-block:: cpp

            constexpr virtual ~boolean();

    .. cpp:function:: operator bool()

        .. code-block:: cpp

            constexpr operator bool() const noexcept;
        
        Return false if attribute value is null.


    .. cpp:function:: value()

        .. code-block:: cpp

            [[nodiscard]] bool& value() &;
            [[nodiscard]] const bool& value() const&;
            [[nodiscard]] bool&& value() &&;

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

            query_condition between(std::convertible_to<bool> arg1, std::convertible_to<bool> arg2);

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
        