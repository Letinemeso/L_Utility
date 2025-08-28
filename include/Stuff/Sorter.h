#pragma once

#include <Data_Structures/Vector.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    template<typename _Type>
    class Sorter
    {
    public:
        using Compare_Function = LST::Function<bool(const _Type&, const _Type&)>;

    private:
        Compare_Function m_less;

    public:
        Sorter();
        Sorter(const Compare_Function& _func);
        ~Sorter();

    public:
        inline void set_less_function(const Compare_Function& _func) { m_less = _func; }

    private:
        void M_subsort(_Type* _subarray, unsigned int _size) const;

    public:
        void sort(LDS::Vector<_Type>& _what) const;
        LDS::Vector<_Type> sort(const LDS::Vector<_Type>& _what) const;

    };



    template<typename _Type>
    Sorter<_Type>::Sorter()
    {
        m_less = [](const _Type& _first, const _Type& _second)
        {
            return _first < _second;
        };
    }

    template<typename _Type>
    Sorter<_Type>::Sorter(const Compare_Function& _func)
    {
        set_less_function(_func);
    }

    template<typename _Type>
    Sorter<_Type>::~Sorter()
    {

    }



    template<typename _Type>
    void Sorter<_Type>::M_subsort(_Type* _subarray, unsigned int _size) const
    {
        if(_size < 2)
            return;

        _Type pivot = _subarray[0];

        unsigned int pivot_index = 0;
        for(unsigned int i = 1; i < _size; ++i)
        {
            if(!m_less(_subarray[i], pivot))
                continue;

            _Type temp = _subarray[pivot_index];
            _subarray[pivot_index] = _subarray[i];
            _subarray[i] = temp;

            ++pivot_index;
        }

        if(pivot_index == 0)
            ++pivot_index;

        M_subsort(_subarray, pivot_index);
        M_subsort(_subarray + pivot_index, _size - pivot_index);
    }



    template<typename _Type>
    void Sorter<_Type>::sort(LDS::Vector<_Type>& _what) const
    {
        _Type* raw = _what.raw_data();
        M_subsort(raw, _what.size());
    }

    template<typename _Type>
    LDS::Vector<_Type> Sorter<_Type>::sort(const LDS::Vector<_Type>& _what) const
    {
        LDS::Vector<_Type> result = _what;
        sort(result);
        return result;
    }

}
