#pragma once

#include <L_Debug/L_Debug.h>
#include <Data_Structures/Vector.h>


namespace LDS
{

    template<typename _Type>
    class Memory_Stack final
    {
    public:
        class Scope final
        {
        private:
            friend class Memory_Stack<_Type>;

        private:
            Memory_Stack<_Type>& m_parent = nullptr;
            unsigned int m_offset = 0;
            unsigned int m_size = 0;

        private:
            Scope(Memory_Stack<_Type>& _parent, unsigned int _offset, unsigned int _size);

        public:
            Scope(const Scope& _other);

            Scope(Scope&&) = delete;
            void operator=(const Scope&) = delete;
            void operator=(Scope&&) = delete;

        public:
            inline unsigned int offset() const { return m_offset; }
            inline unsigned int size() const { return m_size; }

            _Type& operator[](unsigned int _index);
            const _Type& operator[](unsigned int _index) const;

        };

    private:
        LDS::Vector<_Type> m_data;
        unsigned int m_elements_amount = 0;

        unsigned int m_remembered_size = 0;

    public:
        Memory_Stack(unsigned int _size);
        ~Memory_Stack();

    public:
        inline unsigned int elements_amount() const { return m_elements_amount; }
        inline unsigned int remembered_size() const { return m_remembered_size; }

    public:
        void reserve(unsigned int _size);       //  Memory_Stack must not contain any data
        void clear();       //  not deleting anything, but resets size to 0

        void push(const _Type& _data);
        void push(_Type&& _data);
        void pop(unsigned int _amount = 1);
        void revert_to(unsigned int _amount);
        void revert();  //  reverts size to stored value
        void clear_scope(const Scope& _scope);      //  scope must be on top of the stack

        _Type& operator[](unsigned int _index);
        const _Type& operator[](unsigned int _index) const;

        void remember_size();
        Scope scope(unsigned int _offset, unsigned int _size);
        Scope scope();

    };



    //  Scope

    template<typename _Type>
    Memory_Stack<_Type>::Scope::Scope(Memory_Stack<_Type>& _parent, unsigned int _offset, unsigned int _size)
        : m_parent(_parent), m_offset(_offset), m_size(_size)
    {
        L_ASSERT(m_offset + m_size <= m_parent.elements_amount());
    }



    template<typename _Type>
    Memory_Stack<_Type>::Scope::Scope(const Scope& _other)
        : m_parent(_other.m_parent), m_offset(_other.m_offset), m_size(_other.m_size)
    {

    }



    template<typename _Type>
    _Type& Memory_Stack<_Type>::Scope::operator[](unsigned int _index)
    {
        L_ASSERT(_index < m_size);

        return m_parent[m_offset + _index];
    }

    template<typename _Type>
    const _Type& Memory_Stack<_Type>::Scope::operator[](unsigned int _index) const
    {
        L_ASSERT(_index < m_size);

        return m_parent[m_offset + _index];
    }



    //  Memory_Stack

    template<typename _Type>
    Memory_Stack<_Type>::Memory_Stack(unsigned int _size)
        : m_data(_size)
    {
        m_data.mark_full();
    }

    template<typename _Type>
    Memory_Stack<_Type>::~Memory_Stack()
    {

    }



    template<typename _Type>
    void Memory_Stack<_Type>::reserve(unsigned int _size)
    {
        L_ASSERT(m_elements_amount == 0);

        m_data.mark_empty();
        m_data.resize(_size);
        m_data.mark_full();
    }

    template<typename _Type>
    void Memory_Stack<_Type>::clear()
    {
        m_elements_amount = 0;
        m_remembered_size = 0;
    }


    template<typename _Type>
    void Memory_Stack<_Type>::push(const _Type& _data)
    {
        L_ASSERT_WITH_INFO(m_elements_amount < m_data.size(), "[Memory_Stack]: New element does not fit");

        m_data[m_elements_amount] = _data;
        ++m_elements_amount;
    }

    template<typename _Type>
    void Memory_Stack<_Type>::push(_Type&& _data)
    {
        L_ASSERT_WITH_INFO(m_elements_amount < m_data.size(), "[Memory_Stack]: New element does not fit");

        m_data[m_elements_amount] = (_Type&&)_data;
        ++m_elements_amount;
    }

    template<typename _Type>
    void Memory_Stack<_Type>::pop(unsigned int _amount)
    {
        L_ASSERT(_amount <= m_elements_amount);

        m_elements_amount -= _amount;
    }

    template<typename _Type>
    void Memory_Stack<_Type>::revert_to(unsigned int _amount)
    {
        L_ASSERT(_amount <= m_elements_amount);
        m_elements_amount = _amount;
    }

    template<typename _Type>
    void Memory_Stack<_Type>::revert()
    {
        L_ASSERT(m_remembered_size <= m_elements_amount);
        m_elements_amount = m_remembered_size;
    }

    template<typename _Type>
    void Memory_Stack<_Type>::clear_scope(const Scope& _scope)
    {
        L_ASSERT(_scope.offset() + _scope.size() == m_elements_amount);

        pop(_scope.size());
    }


    template<typename _Type>
    _Type& Memory_Stack<_Type>::operator[](unsigned int _index)
    {
        L_ASSERT(_index < m_elements_amount);

        return m_data[_index];
    }

    template<typename _Type>
    const _Type& Memory_Stack<_Type>::operator[](unsigned int _index) const
    {
        L_ASSERT(_index < m_elements_amount);

        return m_data[_index];
    }


    template<typename _Type>
    void Memory_Stack<_Type>::remember_size()
    {
        m_remembered_size = m_elements_amount;
    }

    template<typename _Type>
    typename Memory_Stack<_Type>::Scope Memory_Stack<_Type>::scope(unsigned int _offset, unsigned int _size)
    {
        L_ASSERT(_offset <= m_elements_amount);
        L_ASSERT(_offset + _size <= m_elements_amount);

        return Scope(*this, _offset, _size);
    }

    template<typename _Type>
    typename Memory_Stack<_Type>::Scope Memory_Stack<_Type>::scope()
    {
        return scope(m_remembered_size, m_elements_amount - m_remembered_size);
    }


}
