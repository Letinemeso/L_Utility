#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H

namespace LST
{

    template<typename Type>
    class __I_Function;

    template<typename Return_Type, typename... Args>
    class __I_Function<Return_Type(Args...)>
    {
    public:
        virtual ~__I_Function() { };
        virtual __I_Function* copy() = 0;
        virtual Return_Type invoke(Args... _args) = 0;

    };

    template<typename Object_Type, typename Return_Type, typename... Args>
    class __Function_Implementation : public __I_Function<Return_Type(Args...)>
    {
    private:
        Object_Type m_object_to_invoke;

    public:
        __Function_Implementation(const Object_Type& _obj) : m_object_to_invoke(_obj) { }
        ~__Function_Implementation() { }

        __I_Function<Return_Type(Args...)>* copy() override { return new __Function_Implementation(m_object_to_invoke); }

    public:
        Return_Type invoke(Args... _args) override { return m_object_to_invoke(_args...); }

    };


    template<typename Type>
    class Function;

    template<typename Return_Type, typename... Args>
    class Function<Return_Type(Args...)>
    {
    private:
        __I_Function<Return_Type(Args...)>* m_func = nullptr;

    public:
        Function()
        {
        }

        template<typename Type>
        Function(const Type& _f)
        {
            m_func = new __Function_Implementation<Type, Return_Type, Args...>(_f);
        }

        template<typename Type>
        void operator=(const Type& _f)
        {
            delete m_func;
            m_func = new __Function_Implementation<Type, Return_Type, Args...>(_f);
        }

        ~Function()
        {
            delete m_func;
        }

    public:
        Return_Type operator()(Args... _args) { return m_func->invoke(_args...); }

    };

}


#endif // FUNCTION_WRAPPER_H
