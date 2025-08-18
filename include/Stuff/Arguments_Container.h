#pragma once

#include <L_Debug/L_Debug.h>


namespace LST
{

    template <typename _Arg_Type>
    class Argument_Reference;


    namespace __Utility
    {

        template <typename... _Args>
        struct Subcontainer;


        template <>
        struct Subcontainer<>
        {
            inline void set() { }

            inline void* get_data() { return nullptr; }
            inline Subcontainer<>* get_subcontainer() { return nullptr; }
        };

        template <typename _Type, typename... _Rest>
        struct Subcontainer<_Type, _Rest...>
        {
            _Type data;
            Subcontainer<_Rest...> subcontainer;

            inline void set(_Type& _value, _Rest&... _rest)
            {
                data = _value;
                subcontainer.set(_rest...);
            }

            inline _Type* get_data() { return &data; }
            inline Subcontainer<_Rest...>* get_subcontainer() { return &subcontainer; }
        };


        template <unsigned int... _Indices>
        struct Indices { };

        template <unsigned int _Amount, unsigned int... _Rest_Indices>
        struct Indices_Construction_Helper : public Indices_Construction_Helper<_Amount - 1, _Amount - 1, _Rest_Indices...> {  };

        template <unsigned int... _Indices>
        struct Indices_Construction_Helper<0, _Indices...> : public Indices<_Indices...> {  };

    }


    template <typename _Arg_Type>
    class Argument_Reference
    {
    private:
        _Arg_Type* m_ptr = nullptr;

    public:
        inline void setup(_Arg_Type* _ptr) { m_ptr = _ptr; }

    public:
        inline _Arg_Type& get() { L_ASSERT(m_ptr); return *m_ptr; }
        inline const _Arg_Type& get() const { L_ASSERT(m_ptr); return *m_ptr; }

    };


    template <typename... _Arg_Types>
    class Arguments_Container
    {
    private:
        __Utility::Subcontainer<_Arg_Types...> m_subcontainer;

        using Indices_Sequence_Helper = __Utility::Indices_Construction_Helper<sizeof...(_Arg_Types)>;

    public:
        inline static constexpr unsigned int arguments_amount() { return sizeof...(_Arg_Types); }

    private:
        template <unsigned int _Index, typename... _Rest_Args>
        auto& M_get_argument_impl(__Utility::Subcontainer<_Rest_Args...>* _subcontainer);
        template <unsigned int _Index, typename... _Rest_Args>
        const auto& M_get_argument_impl(__Utility::Subcontainer<_Rest_Args...>* _subcontainer) const;

        auto M_get_argument_impl(__Utility::Subcontainer<>* _subcontainer);

        template <typename _Type, typename... _Args>
        void M_init_reference_impl(unsigned int _index, Argument_Reference<_Type>& _ref, __Utility::Subcontainer<_Args...>& _container);
        template <typename _Type, typename... _Args>
        void M_init_pointer_impl(unsigned int _index, _Type*& _ptr, __Utility::Subcontainer<_Args...>& _container);

        template <typename _Callable, unsigned int... _Indices>
        auto M_call_with_args_impl(const _Callable& _callable, __Utility::Indices<_Indices...>);
        template <typename _Owner_Object_Type, typename _Function_Ptr_Type, unsigned int... _Indices>
        auto M_call_with_args_impl(_Owner_Object_Type& _object, const _Function_Ptr_Type& _function_ptr, __Utility::Indices<_Indices...>);

    public:
        Arguments_Container();
        Arguments_Container(_Arg_Types... _args);

    public:
        template <unsigned int _Index>
        auto& get_argument();
        template <unsigned int _Index>
        const auto& get_argument() const;

        template <typename _Type>
        void init_reference(unsigned int _index, Argument_Reference<_Type>& _ref);
        template <typename _Type>
        void init_pointer(unsigned int _index, _Type*& _ptr);

    public:
        template <typename _Callable>
        auto call_with_args(const _Callable& _callable);
        template <typename _Owner_Object_Type, typename _Function_Ptr_Type>
        auto call_with_args(_Owner_Object_Type& _object, _Function_Ptr_Type _function_ptr);

    };


    template <>
    class Arguments_Container<>
    {
    public:
        template <unsigned int _Index>
        auto& get_argument() { L_ASSERT(false); return nullptr; }
        template <unsigned int _Index>
        const auto& get_argument() const { L_ASSERT(false); return nullptr; }

        auto& get_argument(unsigned int _index) { L_ASSERT(false); return *this; }
        const auto& get_argument(unsigned int _index) const { L_ASSERT(false); return *this; }

        template <typename _Type>
        void init_reference(unsigned int _index, Argument_Reference<_Type>& _ref) { L_ASSERT(false); }
        template <typename _Type>
        void init_pointer(unsigned int _index, _Type*& _ptr) { L_ASSERT(false); }

    public:
        template <typename _Callable>
        auto call_with_args(const _Callable& _callable) { return _callable(); }
    };





    template <typename... _Arg_Types>
    template <unsigned int _Index, typename... _Rest_Args>
    auto& Arguments_Container<_Arg_Types...>::M_get_argument_impl(__Utility::Subcontainer<_Rest_Args...>* _subcontainer)
    {
        if constexpr (_Index == 0)
            return *_subcontainer->get_data();
        else
            return M_get_argument_impl<_Index - 1>(_subcontainer->get_subcontainer());
    }

    template <typename... _Arg_Types>
    template <unsigned int _Index, typename... _Rest_Args>
    const auto& Arguments_Container<_Arg_Types...>::M_get_argument_impl(__Utility::Subcontainer<_Rest_Args...>* _subcontainer) const
    {
        if constexpr (_Index == 0)
            return *_subcontainer->get_data();
        else
            return M_get_argument_impl<_Index - 1>(_subcontainer->get_subcontainer());
    }


    template <typename... _Arg_Types>
    auto Arguments_Container<_Arg_Types...>::M_get_argument_impl(__Utility::Subcontainer<>* _subcontainer)
    {
        return nullptr;
    }


    template <typename... _Arg_Types>
    template <typename _Type, typename... _Args>
    void Arguments_Container<_Arg_Types...>::M_init_reference_impl(unsigned int _index, Argument_Reference<_Type>& _ref, __Utility::Subcontainer<_Args...>& _container)
    {
        if(_index == 0)
            _ref.setup((_Type*)_container.get_data());  //  this is completely type-unsafe but to hell with it! fucking templates
        else
            M_init_reference_impl(_index - 1, _ref, *_container.get_subcontainer());
    }

    template <typename... _Arg_Types>
    template <typename _Type, typename... _Args>
    void Arguments_Container<_Arg_Types...>::M_init_pointer_impl(unsigned int _index, _Type*& _ptr, __Utility::Subcontainer<_Args...>& _container)
    {
        if(_index == 0)
            _ptr = (_Type*)_container.get_data();  //  this is completely type-unsafe but to hell with it! fucking templates
        else
            M_init_pointer_impl(_index - 1, _ptr, *_container.get_subcontainer());
    }


    template <typename... _Arg_Types>
    template <typename _Callable, unsigned int... _Indices>
    auto Arguments_Container<_Arg_Types...>::M_call_with_args_impl(const _Callable& _callable, __Utility::Indices<_Indices...>)
    {
        return _callable( (_Arg_Types&&)get_argument<_Indices>()... );
    }

    template <typename... _Arg_Types>
    template <typename _Owner_Object_Type, typename _Function_Ptr_Type, unsigned int... _Indices>
    auto Arguments_Container<_Arg_Types...>::M_call_with_args_impl(_Owner_Object_Type& _object, const _Function_Ptr_Type& _function_ptr, __Utility::Indices<_Indices...>)
    {
        return (_object.*_function_ptr)( (_Arg_Types&&)get_argument<_Indices>()... );
    }



    template <typename... _Arg_Types>
    Arguments_Container<_Arg_Types...>::Arguments_Container()
    {

    }

    template <typename... _Arg_Types>
    Arguments_Container<_Arg_Types...>::Arguments_Container(_Arg_Types... _args)
    {
        m_subcontainer.set(_args...);
    }



    template <typename... _Arg_Types>
    template <unsigned int _Index>
    auto& Arguments_Container<_Arg_Types...>::get_argument()
    {
        return M_get_argument_impl<_Index>(&m_subcontainer);
    }

    template <typename... _Arg_Types>
    template <unsigned int _Index>
    const auto& Arguments_Container<_Arg_Types...>::get_argument() const
    {
        return M_get_argument_impl<_Index>(&m_subcontainer);
    }


    template <typename... _Arg_Types>
    template <typename _Type>
    void Arguments_Container<_Arg_Types...>::init_reference(unsigned int _index, Argument_Reference<_Type>& _ref)
    {
        M_init_reference_impl(_index, _ref, m_subcontainer);
    }

    template <typename... _Arg_Types>
    template <typename _Type>
    void Arguments_Container<_Arg_Types...>::init_pointer(unsigned int _index, _Type*& _ptr)
    {
        M_init_pointer_impl(_index, _ptr, m_subcontainer);
    }



    template <typename... _Arg_Types>
    template <typename _Callable>
    auto Arguments_Container<_Arg_Types...>::call_with_args(const _Callable& _callable)
    {
        return M_call_with_args_impl<_Callable>(_callable, Indices_Sequence_Helper());
    }

    template <typename... _Arg_Types>
    template <typename _Owner_Object_Type, typename _Function_Ptr_Type>
    auto Arguments_Container<_Arg_Types...>::call_with_args(_Owner_Object_Type& _object, _Function_Ptr_Type _function_ptr)
    {
        return M_call_with_args_impl(_object, _function_ptr, Indices_Sequence_Helper());
    }



}
