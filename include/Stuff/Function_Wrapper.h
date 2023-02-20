#ifndef FUNCTION_WRAPPER_H
#define FUNCTION_WRAPPER_H

namespace LST
{

    template<typename Func_Return_Type, typename... Func_Args>
    class Function_Wrapper
    {
    public:
        virtual Func_Return_Type invoke(Func_Args... _args) const = 0;

    public:
        virtual ~Function_Wrapper() { }

    };



    template<typename Func_Return_Type, typename... Func_Args>
    class Global_Function_Wrapper_Impl : public Function_Wrapper<Func_Return_Type, Func_Args...>
    {
    private:
        using Func_Pointer_Type = Func_Return_Type(*)(Func_Args...);

        Func_Pointer_Type m_function = nullptr;

    private:
        Global_Function_Wrapper_Impl(Func_Pointer_Type _function_pointer) : m_function(_function_pointer) { }

        Global_Function_Wrapper_Impl(const Global_Function_Wrapper_Impl& _other) = delete;
        Global_Function_Wrapper_Impl(Global_Function_Wrapper_Impl&& _other) = delete;

    public:
        template<typename F_Func_Return_Type, typename... F_Func_Args>
        friend Function_Wrapper<F_Func_Return_Type, F_Func_Args...>* make_wrapper(F_Func_Return_Type(*_function)(F_Func_Args...));

    public:
        Func_Return_Type invoke(Func_Args... _args) const override { return m_function(_args...); }

    };



    template<typename Func_Return_Type, typename Owner_Class, typename... Func_Args>
    class Member_Function_Wrapper_Impl : public Function_Wrapper<Func_Return_Type, Func_Args...>
    {
    private:
        using Func_Pointer_Type = Func_Return_Type(Owner_Class::*)(Func_Args...);

        Func_Pointer_Type m_function = nullptr;
        Owner_Class* m_object = nullptr;

    private:
        Member_Function_Wrapper_Impl(Owner_Class* _object, Func_Pointer_Type _function_pointer) : m_object(_object), m_function(_function_pointer) { }

        Member_Function_Wrapper_Impl(const Member_Function_Wrapper_Impl& _other) = delete;
        Member_Function_Wrapper_Impl(Member_Function_Wrapper_Impl&& _other) = delete;

    public:
        template<typename F_Func_Return_Type, typename F_Owner_Class, typename... F_Func_Args>
        friend Function_Wrapper<F_Func_Return_Type, F_Func_Args...>* make_wrapper(F_Owner_Class& _owner_class_object, F_Func_Return_Type(F_Owner_Class::*_function)(F_Func_Args...));

    public:
        Func_Return_Type invoke(Func_Args... _args) const override { return (m_object->*(m_function))(_args...); }

    };



    template<typename Func_Return_Type, typename Owner_Class, typename... Func_Args>
    class Const_Member_Function_Wrapper_Impl : public Function_Wrapper<Func_Return_Type, Func_Args...>
    {
    private:
        using Func_Pointer_Type = Func_Return_Type(Owner_Class::*)(Func_Args...) const;

        Func_Pointer_Type m_function = nullptr;
        const Owner_Class* m_object = nullptr;

    private:
        Const_Member_Function_Wrapper_Impl(const Owner_Class* _object, Func_Pointer_Type _function_pointer) : m_object(_object), m_function(_function_pointer) { }

        Const_Member_Function_Wrapper_Impl(const Const_Member_Function_Wrapper_Impl& _other) = delete;
        Const_Member_Function_Wrapper_Impl(Const_Member_Function_Wrapper_Impl&& _other) = delete;

    public:
        template<typename F_Func_Return_Type, typename F_Owner_Class, typename... F_Func_Args>
        friend Function_Wrapper<F_Func_Return_Type, F_Func_Args...>* make_wrapper(const F_Owner_Class& _owner_class_object, F_Func_Return_Type(F_Owner_Class::*_function)(F_Func_Args...) const);

    public:
        Func_Return_Type invoke(Func_Args... _args) const override { return (m_object->*(m_function))(_args...); }

    };



    //  caller is responsible for deleting created objects
    template<typename Func_Return_Type, typename... Func_Args>
    Function_Wrapper<Func_Return_Type, Func_Args...>* make_wrapper(Func_Return_Type(*_function)(Func_Args...))
    {
        return new Global_Function_Wrapper_Impl<Func_Return_Type, Func_Args...>(_function);
    }

    //  caller is responsible for deleting created objects
    template<typename Func_Return_Type, typename Owner_Class, typename... Func_Args>
    Function_Wrapper<Func_Return_Type, Func_Args...>* make_wrapper(Owner_Class& _owner_class_object, Func_Return_Type(Owner_Class::*_function)(Func_Args...))
    {
        return new Member_Function_Wrapper_Impl<Func_Return_Type, Owner_Class, Func_Args...>(&_owner_class_object, _function);
    }

    //  caller is responsible for deleting created objects
    template<typename Func_Return_Type, typename Owner_Class, typename... Func_Args>
    Function_Wrapper<Func_Return_Type, Func_Args...>* make_wrapper(const Owner_Class& _owner_class_object, Func_Return_Type(Owner_Class::*_function)(Func_Args...) const)
    {
        return new Const_Member_Function_Wrapper_Impl<Func_Return_Type, Owner_Class, Func_Args...>(&_owner_class_object, _function);
    }

}

#endif // FUNCTION_WRAPPER_H
