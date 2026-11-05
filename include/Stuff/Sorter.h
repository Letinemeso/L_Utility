#pragma once

#include <Data_Structures/Vector.h>


namespace LST
{

    template<typename _Type>
    class Sorter
    {
    private:
        void M_subsort(_Type* _subarray, unsigned int _size) const;

    public:
        void sort(LDS::Vector<_Type>& _what) const;
        LDS::Vector<_Type> sort(const LDS::Vector<_Type>& _what) const;

    };



    template<typename _Type>
    void Sorter<_Type>::M_subsort(_Type* _subarray, unsigned int _size) const
    {
        if(_size < 2)
            return;

        _Type pivot = _subarray[0];

        unsigned int pivot_index = 0;
        for(unsigned int i = 1; i < _size; ++i)
        {
            if(!(_subarray[i] < pivot))
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
