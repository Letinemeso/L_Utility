#pragma once

#include <L_Debug/L_Debug.h>
#include <Stuff/Cast_Tools.h>


namespace LDS
{

    template<typename _Type, unsigned int _Capacity>
    class Array final
    {
    private:
        _Type m_data[_Capacity];
        unsigned int m_size = 0;

    public:
        Array();
        Array(const Array<_Type, _Capacity>& _other);
        void operator=(const Array<_Type, _Capacity>& _other);

    public:
        inline constexpr static unsigned int capacity() { return _Capacity; }

        inline unsigned int size() const { return m_size; }
        inline _Type& operator[](unsigned int _index) { L_ASSERT(_index < _Capacity); return m_data[_index]; }
        inline const _Type& operator[](unsigned int _index) const { L_ASSERT(_index < _Capacity); return m_data[_index]; }

    public:
        void push(const _Type& _value);
        void push(_Type&& _value);

        void pop();

        void clear();
        void mark_empty();
        void mark_full();

        bool contains(const _Type& _value) const;

    };


    template<typename _Type, unsigned int _Capacity>
    Array<_Type, _Capacity>::Array()
    {

    }

    template<typename _Type, unsigned int _Capacity>
    Array<_Type, _Capacity>::Array(const Array<_Type, _Capacity>& _other)
    {
        m_size = _other.m_size;

        for(unsigned int i = 0; i < m_size; ++i)
            m_data[i] = _other.m_data[i];
    }

    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::operator=(const Array<_Type, _Capacity>& _other)
    {
        m_size = _other.m_size;

        for(unsigned int i = 0; i < m_size; ++i)
            m_data[i] = _other.m_data[i];
    }


    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::push(const _Type& _value)
    {
        L_ASSERT(m_size < _Capacity);

        m_data[m_size] = _value;
        ++m_size;
    }

    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::push(_Type&& _value)
    {
        L_ASSERT(m_size < _Capacity);

        m_data[m_size] = LST::move(_value);
        ++m_size;
    }


    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::pop()
    {
        L_ASSERT(m_size > 0);

        --m_size;
    }


    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::clear()
    {
        m_size = 0;
    }

    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::mark_empty()
    {
        m_size = 0;
    }

    template<typename _Type, unsigned int _Capacity>
    void Array<_Type, _Capacity>::mark_full()
    {
        m_size = _Capacity;
    }


    template<typename _Type, unsigned int _Capacity>
    bool Array<_Type, _Capacity>::contains(const _Type& _value) const
    {
        for(unsigned int i = 0; i < m_size; ++i)
        {
            if(m_data[i] == _value)
                return true;
        }
        return false;
    }

}
