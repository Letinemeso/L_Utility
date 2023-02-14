#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "Vector.h"


namespace LDS
{

	template<typename Data_Type>
	class Binary_Heap final
	{
	private:
		LDS::Vector<Data_Type> m_array;

	public:
		Binary_Heap();
		Binary_Heap(const Binary_Heap<Data_Type>& _other);
		Binary_Heap(Binary_Heap<Data_Type>&& _other);
		~Binary_Heap();

	private:
		void subheapify(unsigned int _index);
		void subheapify_limited(unsigned int _index, unsigned int _limit);
		void fix_after_push(unsigned int _index);

		bool is_subhipifyed(unsigned int _index) const;

	public:
		void build_from_raw(const LDS::Vector<Data_Type>& _from);
		void build_from_raw(LDS::Vector<Data_Type>&& _from);
		void push(const Data_Type& _data);
		void push(Data_Type&& _data);
		void pop_max();	//	only works if heapifyed

		void heapify();
		void sort();

	public:
		unsigned int size() const;
		LDS::Vector<Data_Type>& array();
		const LDS::Vector<Data_Type>& array() const;

		bool is_heapifyed() const;

	};

    //	Binary_Heap

    template<typename Data_Type>
    Binary_Heap<Data_Type>::Binary_Heap()
    {

    }

    template<typename Data_Type>
    Binary_Heap<Data_Type>::Binary_Heap(const Binary_Heap<Data_Type>& _other)
    {
        m_array = _other.m_array;
    }

    template<typename Data_Type>
    Binary_Heap<Data_Type>::Binary_Heap(Binary_Heap<Data_Type>&& _other)
    {
        m_array = (LDS::Vector<Data_Type>&&)(_other.m_array);
    }

    template<typename Data_Type>
    Binary_Heap<Data_Type>::~Binary_Heap()
    {

    }



    template<typename Data_Type>
    void Binary_Heap<Data_Type>::subheapify(unsigned int _index)
    {
        unsigned int left_child = (_index + 1) * 2 - 1;
        unsigned int right_child = (_index + 1) * 2;

        if(left_child >= m_array.size())
            return;
        if(right_child >= m_array.size())
        {
            if(m_array[_index] < m_array[left_child])
                m_array.swap(_index, left_child);
            return;
        }

        if(m_array[_index] >= m_array[left_child] && m_array[_index] >= m_array[right_child])
            return;

        unsigned int larger = left_child;
        if(m_array[right_child] > m_array[left_child])
            larger = right_child;

        m_array.swap(_index, larger);

        subheapify(larger);
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::subheapify_limited(unsigned int _index, unsigned int _limit)
    {
        L_ASSERT(_limit <= m_array.size());

        unsigned int left_child = (_index + 1) * 2 - 1;
        unsigned int right_child = (_index + 1) * 2;

        if(left_child >= _limit)
            return;
        if(right_child >= _limit)
        {
            if(m_array[_index] < m_array[left_child])
                m_array.swap(_index, left_child);
            return;
        }

        if(m_array[_index] >= m_array[left_child] && m_array[_index] >= m_array[right_child])
            return;

        unsigned int larger = left_child;
        if(m_array[right_child] > m_array[left_child])
            larger = right_child;

        m_array.swap(_index, larger);

        subheapify_limited(larger, _limit);
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::fix_after_push(unsigned int _index)
    {
        L_ASSERT(_index < m_array.size());

        if(_index == 0)
            return;

        unsigned int parent_index = (_index + 1) / 2 - 1;

        if(m_array[_index] <= m_array[parent_index])
            return;

        m_array.swap(_index, parent_index);

        fix_after_push(parent_index);
    }


    template<typename Data_Type>
    bool Binary_Heap<Data_Type>::is_subhipifyed(unsigned int _index) const
    {
        unsigned int left_child = (_index + 1) * 2 - 1;
        unsigned int right_child = (_index + 1) * 2;

        if(left_child < m_array.size())
        {
            if(m_array[_index] < m_array[left_child])
                return false;
        }
        else
        {
            return true;
        }

        if(right_child < m_array.size())
        {
            if(m_array[_index] < m_array[right_child])
                return false;
        }
        else
        {
            return true;
        }

        if(!is_subhipifyed(left_child))
            return false;
        if(!is_subhipifyed(right_child))
            return false;

        return true;
    }




    template<typename Data_Type>
    void Binary_Heap<Data_Type>::build_from_raw(const LDS::Vector<Data_Type>& _from)
    {
        m_array = _from;
        heapify();
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::build_from_raw(LDS::Vector<Data_Type>&& _from)
    {
        m_array = (LDS::Vector<Data_Type>&&)_from;
        heapify();
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::push(const Data_Type& _from)
    {
        m_array.push(_from);
        fix_after_push(m_array.size() - 1);
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::push(Data_Type&& _from)
    {
        m_array.push((Data_Type&&)_from);
        fix_after_push(m_array.size() - 1);
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::pop_max()
    {
        L_ASSERT(m_array.size() > 0);
        L_ASSERT(is_heapifyed());

        if(m_array.size() == 1)
        {
            m_array.clear();
            return;
        }

        m_array.swap(0, m_array.size() - 1);
        m_array.pop(m_array.size() - 1);
        subheapify(0);
    }


    template<typename Data_Type>
    void Binary_Heap<Data_Type>::heapify()
    {
        for(int i = m_array.size() / 2; i >= 0; --i)
            subheapify(i);
    }

    template<typename Data_Type>
    void Binary_Heap<Data_Type>::sort()
    {
        unsigned int last = m_array.size() - 1;

        while(last > 0)
        {
            m_array.swap(0, last);

            subheapify_limited(0, last);

            --last;
        }
    }



    template<typename Data_Type>
    unsigned int Binary_Heap<Data_Type>::size() const
    {
        return m_array.size();
    }

    template<typename Data_Type>
    LDS::Vector<Data_Type>& Binary_Heap<Data_Type>::array()
    {
        return m_array;
    }

    template<typename Data_Type>
    const LDS::Vector<Data_Type>& Binary_Heap<Data_Type>::array() const
    {
        return m_array;
    }


    template<typename Data_Type>
    bool Binary_Heap<Data_Type>::is_heapifyed() const
    {
        return is_subhipifyed(0);
    }

}



#endif // BINARY_HEAP_H
