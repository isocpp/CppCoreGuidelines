namespace positive_cases_in_type_conversions
{
    enum enum_1
    {
        enum_value_1
    };

    enum class enum_class_1
    {
        enum_class_value_1
    };

    using alias_1 = int;

    class class_1
    {
    };

    class class_2 : class_1
    {
    };

    struct struct_1
    {
    };

    void function_1(void *arg_1, enum_1 arg_2, enum_class_1 arg_3, int (class_1::*arg_4))
    {
        arg_1 = reinterpret_cast<void *>(arg_1);                                    // @expected(type.1) for same type cast with pointer type.
        arg_2 = reinterpret_cast<enum_1>(arg_2);                                    // @expected(type.1) for same type cast with enum type.
        arg_3 = reinterpret_cast<enum_class_1>(arg_3);                              // @expected(type.1) for same type cast with enum class type.
        arg_4 = reinterpret_cast<int (class_1::*)>(arg_4);                          // @expected(type.1) for same type cast with pointer to memeber type.
    }

    void function_2(void *arg_1, int *arg_2, char **arg_3, enum_class_1 *arg_4, class_1 *arg_5, void (*arg_6)())
    {
        size_t local_1 = 0;
        local_1 = reinterpret_cast<size_t>(arg_1);                                  // @expected(type.1) for integral cast from void pointer.
        local_1 = reinterpret_cast<size_t>(arg_2);                                  // @expected(type.1) for integral cast from integral pointer.
        local_1 = reinterpret_cast<size_t>(arg_3);                                  // @expected(type.1) for integral cast from pointer to pointer.
        local_1 = reinterpret_cast<size_t>(arg_4);                                  // @expected(type.1) for integral cast from enum class pointer.
        local_1 = reinterpret_cast<size_t>(arg_5);                                  // @expected(type.1) for integral cast from class pointer.
        local_1 = reinterpret_cast<size_t>(arg_6);                                  // @expected(type.1) for integral cast from function pointer.
    }

    void function_3(int arg_1, enum_1 arg_2, enum_class_1 arg_3)
    {
        void *local_1 = nullptr;
        local_1 = reinterpret_cast<void *>(arg_1);                                  // @expected(type.1) for pointer cast from integral type.
        local_1 = reinterpret_cast<void *>(arg_2);                                  // @expected(type.1) for pointer cast from enum type.
        local_1 = reinterpret_cast<void *>(arg_3);                                  // @expected(type.1) for pointer cast from enum class type.
        local_1 = reinterpret_cast<void *>(0);                                      // @expected(type.1) for pointer cast from constant.
    }

    void function_4(std::nullptr_t arg_1)
    {
        int local_1 = 0;
        local_1 = reinterpret_cast<int>(arg_1);                                     // @expected(type.1) for integral cast from nullptr_t value.
        local_1 = reinterpret_cast<int>(nullptr);                                   // @expected(type.1) for integral cast from nullptr literal.
    }

    void function_5(class_1 *arg_1, class_2 *arg_2, struct_1 *arg_3)
    {
        arg_1 = reinterpret_cast<class_1 *>(arg_2);                                 // @expected(type.1) for base class cast.
        arg_2 = reinterpret_cast<class_2 *>(arg_1);                                 // @expected(type.1) for derived class cast.
        arg_1 = reinterpret_cast<class_1 *>(arg_3);                                 // @expected(type.1) for unrelated class cast.
    }

    void function_6(int &arg_1, int &&arg_2)
    {
        int *local_1 = 0;
        local_1 = reinterpret_cast<int *>(arg_1);                                   // @expected(type.1) for reference to pointer cast.
        local_1 = reinterpret_cast<int *>(arg_2);                                   // @expected(type.1) for rvalue reference to pointer cast.

        int &local_2 = reinterpret_cast<int &>(arg_2);                              // @expected(type.1) for rvalue reference to reference cast.
        int &&local_3 = reinterpret_cast<int &&>(arg_1);                            // @expected(type.1) for reference to rvalue reference cast.
    }

    void function_7(void (*arg_1)(), void* (*arg_2)(int))
    {
        int* (*local_1)(int) = nullptr;
        local_1 = reinterpret_cast<int* (*)(int)>(arg_1);                            // @expected(type.1) for unrelated function pointer cast.
        local_1 = reinterpret_cast<int* (*)(int)>(arg_2);                            // @expected(type.1) for compatible function pointer cast.
    }

    void function_8(void (*arg_1)(), void* arg_2, class_1 *arg_3)
    {
        arg_2 = reinterpret_cast<void *>(arg_1);                                    // @expected(type.1) for function pointer to void pointer cast.
        arg_3 = reinterpret_cast<class_1 *>(arg_1);                                 // @expected(type.1) for function pointer to class pointer cast.
        arg_1 = reinterpret_cast<void (*)()>(arg_2);                                // @expected(type.1) for void pointer to function pointer cast.
        arg_1 = reinterpret_cast<void (*)()>(arg_3);                                // @expected(type.1) for class pointer to function pointer cast.
    }

    void function_9(void (class_1::*arg_1)(), void (class_2::*arg_2)(), int (struct_1::*arg_3)(int))
    {
        arg_2 = reinterpret_cast<void (class_2::*)()>(arg_1);                       // @expected(type.1) for related member function pointer cast.
        arg_3 = reinterpret_cast<int (struct_1::*)(int)>(arg_1);                    // @expected(type.1) for unrelated member function pointer cast.
    }

    void function_10(alias_1 arg_1, unsigned int arg_2, int * const volatile arg_3)
    {
        int *local_1 = nullptr;
        local_1 = reinterpret_cast<int *>(arg_1);                                   // @expected(type.1) for cast from non-pointer alias.
        local_1 = reinterpret_cast<int *>(arg_2);                                   // @expected(type.1) for cast from non-pointer unsigned type.
        local_1 = reinterpret_cast<int *>(arg_3);                                   // @expected(type.1) for cast from pointer with modifiers.

        arg_1 = reinterpret_cast<alias_1>(local_1);                                 // @expected(type.1) for cast to non-pointer alias.
        arg_2 = reinterpret_cast<unsigned int>(local_1);                            // @expected(type.1) for cast to non-pointer unsigned type.

        int * const local_2 = reinterpret_cast<int * volatile>(local_1);            // @expected(type.1) for implicit and explicit modifiers cast.
    }

    void function_11(void *arg_1, int* arg_2, const char *arg_3)
    {
        int local_1[] = { 0 };

        arg_1 = reinterpret_cast<void *>(local_1);                                  // @expected(type.1) for array to unrelated pointer cast.
        arg_2 = reinterpret_cast<int *>(local_1);                                   // @expected(type.1) for array to pointer cast.
        arg_2 = reinterpret_cast<int[1]>(local_1);                                  // @expected(type.1) for array to array cast.

        arg_3 = reinterpret_cast<const char *>("");                                 // @expected(type.1) for string literal cast.
    }

    void function_12()
    {
        auto local_1 = 0;
        auto local_2 = reinterpret_cast<decltype(local_1)*>(local_1);               // @expected(type.1) for pointer cast with auto type resolution.
    }
}

namespace positive_cases_in_expressions
{
    struct struct_1
    {
        int field_1;
        void method_1();
    };

    void function_1(int arg_1, int *arg_2, struct_1 *arg_3)
    {
        reinterpret_cast<struct_1*>(arg_2)->field_1 = 0;                            // @expected(type.1) in arrow access.
        reinterpret_cast<struct_1&>(arg_2).method_1();                              // @expected(type.1) in dot access.

        arg_3->*reinterpret_cast<int (struct_1::*)>(0) = 0;                         // @expected(type.1) in field pointer access.
        ((*arg_3).*reinterpret_cast<void (struct_1::*)()>(0))();                    // @expected(type.1) in method pointer access.

        reinterpret_cast<int*>(arg_1)[arg_1] = 0;                                   // @expected(type.1) in array subcript target.
        arg_2[reinterpret_cast<int>(arg_2)] = 0;                                    // @expected(type.1) in array subcript index.

        reinterpret_cast<int*&>(arg_1)++;                                           // @expected(type.1) in postfix unary.
        --reinterpret_cast<int&>(arg_2);                                            // @expected(type.1) in prefix unary.

        typeid(reinterpret_cast<int*>(arg_1));                                      // @expected(type.1) in typeid.
        sizeof(reinterpret_cast<int*>(arg_1));                                      // @expected(type.1) in sizeof.

        !reinterpret_cast<int>(arg_2);                                              // @expected(type.1) in logical unary.
        -reinterpret_cast<int>(arg_2);                                              // @expected(type.1) in arithmetic unary.
        ~reinterpret_cast<int>(arg_2);                                              // @expected(type.1) in bitwise unary.

        reinterpret_cast<int>(arg_2) && arg_1;                                      // @expected(type.1) in logical binary.
        reinterpret_cast<int>(arg_2) + arg_1;                                       // @expected(type.1) in arithmetic binary.
        reinterpret_cast<int>(arg_2) & arg_1;                                       // @expected(type.1) in bitwise binary.

        &reinterpret_cast<int&>(arg_2);                                             // @expected(type.1) in address of.
        *reinterpret_cast<int*>(arg_1);                                             // @expected(type.1) in indirection.

        delete reinterpret_cast<int*>(arg_1);                                       // @expected(type.1) in delete.

        reinterpret_cast<int&>(arg_2) = 0;                                          // @expected(type.1) in assignment target.
        reinterpret_cast<int&>(arg_2) += 0;                                         // @expected(type.1) in combined assignment target.

        reinterpret_cast<bool>(arg_2) ?                                             // @expected(type.1) in ternary operator (condition).
            reinterpret_cast<void *>(arg_2) :                                       // @expected(type.1) in ternary operator (true branch).
            reinterpret_cast<void *>(arg_1);                                        // @expected(type.1) in ternary operator (false branch).

        arg_1 = reinterpret_cast<int>(arg_2),                                       // @expected(type.1) in comma (left side).
                    reinterpret_cast<void *>(arg_3);                                // @expected(type.1) in comma (right side).

        arg_1 = reinterpret_cast<int>(                                              // @expected(type.1) in nested cast (external).
                    reinterpret_cast<void *>(arg_3));                               // @expected(type.1) in nested cast (internal).

        arg_2 = reinterpret_cast<int*>(1 + arg_1);                                  // @expected(type.1) in cast of a complex expression result.

        throw reinterpret_cast<void *>(arg_3);                                      // @expected(type.1) in throw.
    }
}

namespace positive_cases_in_constexpr
{
    constexpr int function_1(int *arg_1)
    {
        return reinterpret_cast<int>(arg_1);                                        // @expected(type.1) in constexpr function.
    }

    void function_2()
    {
        constexpr int *local_1 = reinterpret_cast<int *>(function_1(nullptr));      // @expected(type.1) in constexpr expression.
    }
}

namespace positive_cases_in_function_calls
{
    void function_1(void* (*arg_1)(int))
    {
        int local_1 = 0;
        local_1 = reinterpret_cast<int>(                                            // @expected(type.1) in function call return.
                        arg_1(reinterpret_cast<int>(arg_1)));                       // @expected(type.1) in function call argument.
    }

    struct struct_1
    {
        struct_1(int);
    };

    struct struct_2 : struct_1
    {
        struct_2(int *arg_1) : struct_1(reinterpret_cast<int>(arg_1))               // @expected(type.1) in base class call.
        {
        }
        
        struct_2(void *arg_1) : struct_2(reinterpret_cast<int*>(arg_1))             // @expected(type.1) in forwarding ctor call.
        {
        }
    };
}

namespace positive_cases_in_statements
{
    int* function_1(int arg_1, void *arg_2)
    {
        if (reinterpret_cast<int*>(arg_1))                                          // @expected(type.1) in 'if' statement.
        {
        }

        switch (reinterpret_cast<int>(arg_2))                                       // @expected(type.1) in 'switch' statement (condition).
        {
        case reinterpret_cast<int>(nullptr):                                        // @expected(type.1) in 'switch' statement (case label).
            break;
        }

        do
        {
        } while (reinterpret_cast<int*>(arg_1));                                    // @expected(type.1) in 'do' statement.

        while (reinterpret_cast<int*>(arg_1))                                       // @expected(type.1) in 'while' statement.
        {
        }

        for (
            int *i = reinterpret_cast<int*>(arg_1);                                 // @expected(type.1) in 'for' statement (declaration).
            reinterpret_cast<bool>(i);                                              // @expected(type.1) in 'for' statement (condition).
            reinterpret_cast<int&>(i)++)                                            // @expected(type.1) in 'for' statement (increment).
        {
        }

        for (auto v : reinterpret_cast<int(&)[2]>(arg_2))                           // @expected(type.1) in range-based 'for' statement.
        {
        }

        return reinterpret_cast<int*>(arg_1);                                       // @expected(type.1) in 'return' statement.
    }
}

namespace positive_cases_in_initializers
{
    int *global_1 = reinterpret_cast<int *>(1);                                     // @expected(type.1) in static initializer for a global variable.
    int *global_2 = reinterpret_cast<int *>(&global_1);                             // @expected(type.1) in dynamic initializer for a global variable.

    struct struct_1
    {
        static int *static_field_1;
        static int *static_field_2;

        int *field_1;
        int *field_2;

        int *field_3 = reinterpret_cast<int *>(1);                                  // @expected(type.1) in static initializer for a field.
        int *field_4 = reinterpret_cast<int *>(&field_3);                           // @expected(type.1) in dynamic initializer for a field.

        struct_1() :
            field_1(reinterpret_cast<int *>(1)),                                    // @expected(type.1) in static initializer for a field in ctor.
            field_2(reinterpret_cast<int *>(&field_1))                              // @expected(type.1) in dynamic initializer for a field in ctor.
        {
        }
    };

    int *struct_1::static_field_1 = reinterpret_cast<int *>(1);                     // @expected(type.1) in static initializer for a static field.
    int *struct_1::static_field_2 = reinterpret_cast<int *>(&static_field_1);       // @expected(type.1) in dynamic initializer for a static field.

    void function_1(void *arg_1 = reinterpret_cast<void *>(0))                      // @expected(type.1) in default initializer for a parameter.
    {
        static int *static_1 = reinterpret_cast<int *>(0);                          // @expected(type.1) in static initializer for a static variable.
        static int *static_2 = reinterpret_cast<int *>(arg_1);                      // @expected(type.1) in dynamic initializer for a static variable.

        int *local_1 = reinterpret_cast<int *>(0);                                  // @expected(type.1) in static initializer for a local variable.
        int *local_2 = reinterpret_cast<int *>(arg_1);                              // @expected(type.1) in dynamic initializer for a local variable.
        int *local_3 { reinterpret_cast<int *>(&local_1) };                         // @expected(type.1) in direct initializer list for a local variable.
        int *local_4 = { reinterpret_cast<int *>(&local_1) };                       // @expected(type.1) in copy initializer list for a local variable.
        int *local_5 = new int { reinterpret_cast<int>(arg_1) };                    // @expected(type.1) in 'new' statement initializer list for a local variable.
    }
}

namespace positive_cases_in_templates
{
    template<int *P>
    struct struct_1
    {
        int field = reinterpret_cast<int>(P);                                       // @expected(type.1) for P == 1.
    };

    template<int *P = reinterpret_cast<int*>(1)>                                    // @expected(type.1) for default P.
    using alias_1 = int[reinterpret_cast<int>(P)];                                  // @expected(type.1) for default P and @expected(type.1) for P == 2.

    template<class T1, class T2>
    T1 function_1(T2 arg_1)
    {
        return reinterpret_cast<T1>(arg_1);                                         // @expected(type.1) for T1 == int and T2 == int[2].
    }

    void function_2()
    {
        struct_1<reinterpret_cast<int*>(1)>().field = 0;                            // @expected(type.1) in struct template instantiation.

        alias_1<> local_1;

        alias_1<reinterpret_cast<int*>(2)> local_2;                                 // @expected(type.1) in alias template instantiation.

        int local_3 = function_1<int>(local_2);
    }
}

namespace negative_cases_in_casts
{
    struct struct_1
    {
        virtual void method_1() = 0;
    };

    struct struct_2
    {
    };

    void function_1(struct_1 *arg_1)
    {
        struct_1 * const local_1 = arg_1;                                           // no type.1 warning in implicit cast.
        void *local_2 = static_cast<void*>(arg_1);                                  // no type.1 warning in static cast.
        struct_2 *local_3 = dynamic_cast<struct_2*>(arg_1);                         // no type.1 warning in dynamic cast.

        arg_1 = const_cast<struct_1*>(local_1);                                     // no type.1 warning in const cast.
        arg_1 = (struct_1*)(local_2);                                               // no type.1 warning in C-style cast.
    }
}

namespace negative_cases_in_tokens
{
    void function_1()
    {
        int *local_1 = nullptr; /* = reinterpret_cast<int *>(0); */                 // no type.1 warning in a comment.

        auto local_2 = "reinterpret_cast<int *>(0);";                               // no type.1 warning in a string literal.

        #define reinterpret_cast static_cast                                        // no type.1 warning in a macro definition.
        void *local_3 = reinterpret_cast<void*>(0);                                 // no type.1 warning in a macro expansion.
        #undef reinterpret_cast
    }
}