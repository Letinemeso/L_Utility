#ifndef VECTOR_H
#define VECTOR_H

#include "L_Debug/L_Debug.h"


namespace LDS
{

	template<typename Data_Type>
	class Vector final
    {
	private:
		class Iterator_Base final
		{
		private:
			friend class LDS::Vector<Data_Type>;

		private:
            Vector<Data_Type>* m_parent = nullptr;
			int m_current_index = 0;
			bool m_begin_reached = false;
			bool m_end_reached = false;

		public:
			Iterator_Base(Vector<Data_Type>* _parent);
			Iterator_Base(const Iterator_Base& _other);
			void operator=(const Iterator_Base& _other);
			~Iterator_Base();

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;
			Data_Type* get_ptr();

		public:
			bool begin_reached() const;
			bool end_reached() const;

			bool is_ok() const;

		};

	public:
		class Iterator final
		{
		private:
			friend class Vector;

		private:
			Iterator_Base m_it;

		private:
			Iterator(Vector<Data_Type>* _parent);

		public:
			Iterator(const Iterator& _other);
			void operator=(const Iterator& _other);

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;
			Data_Type* operator->();
			const Data_Type* operator->() const;

		public:
			bool begin_reached() const;
			bool end_reached() const;

			bool is_ok() const;

		};

		class Const_Iterator final
		{
		private:
			friend class Vector;

		private:
            Iterator_Base m_it;

		private:
            Const_Iterator(const Vector<Data_Type>* _parent);

		public:
			Const_Iterator(const Const_Iterator& _other);
			void operator=(const Const_Iterator& _other);

		public:
			void operator++();
			void operator--();

		public:
			const Data_Type& operator*() const;
			const Data_Type* operator->() const;

		public:
			bool begin_reached() const;
			bool end_reached() const;

			bool is_ok() const;

		};

	private:
        Data_Type* m_array = nullptr;
		unsigned int m_elements_count = 0;
		unsigned int m_size = 0;

	public:
        Vector(unsigned int _initial_capacity = 5);
        Vector(unsigned int _initial_capacity, const Data_Type& _fill_with);
		Vector(const Vector<Data_Type>& _other);
		Vector(Vector<Data_Type>&& _other);
		void operator=(const Vector<Data_Type>& _other);
		void operator=(Vector<Data_Type>&& _other);
		~Vector();

	public:
		void resize(unsigned int _new_size);
        void resize_and_fill(unsigned int _new_size, const Data_Type& _fill_with);
		void clear();

	public:
		void push(const Data_Type& _data);
		void push(Data_Type&& _data);

		void pop(unsigned int _index);
		Iterator pop(const Iterator& _where);
		Const_Iterator pop(const Const_Iterator& _where);

		void swap(unsigned int _f_index, unsigned int _s_index);
		void swap(const Iterator& _first, const Iterator& _second);
		void swap(const Const_Iterator& _first, const Const_Iterator& _second);

	public:
        inline Data_Type* raw_data() { return m_array; }
        inline const Data_Type* raw_data() const { return m_array; }

        inline unsigned int size() const { return m_elements_count; }
        inline unsigned int capacity() const { return m_size; }

        Data_Type& operator[](unsigned int _index);
		const Data_Type& operator[](unsigned int _index) const;

		Iterator at(unsigned int _index);
		Const_Iterator at(unsigned int _index) const;

		Iterator iterator();
		Const_Iterator const_iterator() const;

	};

    //	Vector

    template<typename Data_Type>
    Vector<Data_Type>::Vector(unsigned int _initial_capacity)
    {
        m_size = _initial_capacity;
        m_array = new Data_Type[m_size];
    }

    template<typename Data_Type>
    Vector<Data_Type>::Vector(unsigned int _initial_capacity, const Data_Type& _fill_with)
    {
        m_size = _initial_capacity;
        m_elements_count = m_size;
        m_array = new Data_Type[m_size]{ _fill_with };
    }

    template<typename Data_Type>
    Vector<Data_Type>::Vector(const Vector<Data_Type>& _other)
    {
        m_size = _other.m_size;
        m_elements_count = _other.m_elements_count;

        m_array = new Data_Type[m_size];
        for(unsigned int i=0; i<m_elements_count; ++i)
            m_array[i] = _other[i];
    }

    template<typename Data_Type>
    Vector<Data_Type>::Vector(Vector<Data_Type>&& _other)
    {
        m_size = _other.m_size;
        _other.m_size = 5;
        m_elements_count = _other.m_elements_count;
        _other.m_elements_count = 0;

        m_array = _other.m_array;
        _other.m_array = new Data_Type[_other.m_size];
    }

    template<typename Data_Type>
    void Vector<Data_Type>::operator=(const Vector<Data_Type>& _other)
    {
        delete[] m_array;

        m_size = _other.m_size;
        m_elements_count = _other.m_elements_count;

        m_array = new Data_Type[m_size];
        for(unsigned int i=0; i<_other.m_elements_count; ++i)
            m_array[i] = _other[i];
    }

    template<typename Data_Type>
    void Vector<Data_Type>::operator=(Vector<Data_Type>&& _other)
    {
        delete[] m_array;

        m_size = _other.m_size;
        _other.m_size = 5;
        m_elements_count = _other.m_elements_count;
        _other.m_elements_count = 0;

        m_array = _other.m_array;
        _other.m_array = new Data_Type[_other.m_size];
    }

    template<typename Data_Type>
    Vector<Data_Type>::~Vector()
    {
        delete[] m_array;
    }



    template<typename Data_Type>
    void Vector<Data_Type>::resize(unsigned int _new_size)
    {
        Data_Type* temp = new Data_Type[_new_size];

        if(_new_size < m_size)
        {
            for(unsigned int i=0; i<_new_size; ++i)
                temp[i] = m_array[i];
        }
        else
        {
            for(unsigned int i=0; i<m_size; ++i)
                temp[i] = m_array[i];
        }

        delete[] m_array;
        m_array = temp;
        m_size = _new_size;

        if(m_elements_count > m_size)
            m_elements_count = m_size;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::resize_and_fill(unsigned int _new_size, const Data_Type& _fill_with)
    {
        resize(_new_size);
        for(unsigned int i=m_elements_count; i<m_size; ++i)
            m_array[i] = _fill_with;
        m_elements_count = m_size;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::clear()
    {
        delete[] m_array;

        m_size = 5;
        m_elements_count = 0;
        m_array = new Data_Type[m_size];
    }



    template<typename Data_Type>
    void Vector<Data_Type>::push(const Data_Type& _data)
    {
        if(m_elements_count == m_size)
            resize(m_size * 2);

        m_array[m_elements_count] = _data;

        ++m_elements_count;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::push(Data_Type&& _data)
    {
        if(m_elements_count == m_size)
            resize(m_size * 2);

        m_array[m_elements_count] = (Data_Type&&)_data;

        ++m_elements_count;
    }


    template<typename Data_Type>
    void Vector<Data_Type>::pop(unsigned int _index)
    {
        L_ASSERT(m_elements_count > 0);
        L_ASSERT(_index < m_elements_count);

        for(unsigned int i = _index; i < m_elements_count; ++i)
            m_array[i] = m_array[i + 1];

        --m_elements_count;
    }

    template<typename Data_Type>
    typename Vector<Data_Type>::Iterator Vector<Data_Type>::pop(const Iterator& _where)
    {
        L_ASSERT(m_elements_count > 0);
        L_ASSERT(_where.m_it.m_parent == this);

        unsigned int index = _where.m_it.m_current_index;

        pop(index);

        Iterator return_it(this);

        if(m_elements_count == 0)
        {
            return_it.m_it.m_end_reached = true;
        }
        else if(_where.m_it.m_current_index == m_elements_count)
        {
            return_it.m_it.m_current_index = m_elements_count - 1;
            return_it.m_it.m_end_reached = true;
        }
        else
        {
            return_it.m_it.m_current_index = _where.m_it.m_current_index;
        }

        return return_it;
    }

    template<typename Data_Type>
    typename Vector<Data_Type>::Const_Iterator Vector<Data_Type>::pop(const Const_Iterator& _where)
    {
        L_ASSERT(m_elements_count > 0);
        L_ASSERT(_where.m_it.m_parent == this);

        unsigned int index = _where.m_it.m_current_index;

        pop(index);

        Const_Iterator return_it(this);

        if(m_elements_count == 0)
        {
            return_it.m_it.m_end_reached = true;
        }
        else if(_where.m_it.m_current_index == m_elements_count)
        {
            return_it.m_it.m_current_index = m_elements_count - 1;
            return_it.m_it.m_end_reached = true;
        }
        else
        {
            return_it.m_it.m_current_index = _where.m_it.m_current_index;
        }

        return return_it;
    }


    template<typename Data_Type>
    void Vector<Data_Type>::swap(unsigned int _f_index, unsigned int _s_index)
    {
        L_ASSERT(_f_index < m_elements_count);

        L_ASSERT(_s_index < m_elements_count);

        Data_Type temp = m_array[_f_index];
        m_array[_f_index] = m_array[_s_index];
        m_array[_s_index] = temp;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::swap(const Iterator& _first, const Iterator& _second)
    {
        L_ASSERT(_first.m_it.m_parent == this && _second.m_it.m_parent == this);

        unsigned int first_index = _first.m_it.m_current_index;
        L_ASSERT(first_index < m_elements_count);

        unsigned int second_index = _second.m_it.m_current_index;
        L_ASSERT(second_index < m_elements_count);

        L_ASSERT(second_index != m_elements_count);

        swap(first_index, second_index);
    }

    template<typename Data_Type>
    void Vector<Data_Type>::swap(const Const_Iterator& _first, const Const_Iterator& _second)
    {
        L_ASSERT(_first.m_it.m_parent == this && _second.m_it.m_parent == this);

        unsigned int first_index = _first.m_it.m_current_index;
        L_ASSERT(first_index < m_elements_count);

        unsigned int second_index = _second.m_it.m_current_index;
        L_ASSERT(second_index < m_elements_count);

        L_ASSERT(second_index != m_elements_count);

        swap(first_index, second_index);
    }



    template<typename Data_Type>
    Data_Type& Vector<Data_Type>::operator[](unsigned int _index)
    {
        L_ASSERT(_index < m_size);

        return m_array[_index];
    }

    template<typename Data_Type>
    const Data_Type& Vector<Data_Type>::operator[](unsigned int _index) const
    {
        L_ASSERT(_index < m_elements_count);

        return m_array[_index];
    }


    template<typename Data_Type>
    typename Vector<Data_Type>::Iterator Vector<Data_Type>::at(unsigned int _index)
    {
        L_ASSERT(_index < m_elements_count);

        Iterator result(this);
        result.m_it.m_current_index = _index;
        return result;
    }

    template<typename Data_Type>
    typename Vector<Data_Type>::Const_Iterator Vector<Data_Type>::at(unsigned int _index) const
    {
        L_ASSERT(_index < m_elements_count);

        Const_Iterator result(this);
        result.m_it.m_current_index = _index;
        return result;
    }


    template<typename Data_Type>
    typename Vector<Data_Type>::Iterator Vector<Data_Type>::iterator()
    {
        return Iterator(this);
    }

    template<typename Data_Type>
    typename Vector<Data_Type>::Const_Iterator Vector<Data_Type>::const_iterator() const
    {
        return Const_Iterator(this);
    }





    //	Vector::Iterator_Base

    template<typename Data_Type>
    Vector<Data_Type>::Iterator_Base::Iterator_Base(Vector<Data_Type>* _parent)
        : m_parent(_parent)
    {

    }

    template<typename Data_Type>
    Vector<Data_Type>::Iterator_Base::Iterator_Base(const Iterator_Base& _other)
        : m_parent(_other.m_parent), m_current_index(_other.m_current_index)
    {

    }

    template<typename Data_Type>
    void Vector<Data_Type>::Iterator_Base::operator=(const Iterator_Base& _other)
    {
        m_parent = _other.m_parent;
        m_current_index = _other.m_current_index;
        m_begin_reached = _other.m_begin_reached;
        m_end_reached = _other.m_end_reached;
    }

    template<typename Data_Type>
    Vector<Data_Type>::Iterator_Base::~Iterator_Base()
    {

    }



    template<typename Data_Type>
    void Vector<Data_Type>::Iterator_Base::operator++()
    {
        L_ASSERT(!m_end_reached);

        m_begin_reached = false;

        if(m_current_index == m_parent->size() - 1)
        {
            m_end_reached = true;
            return;
        }

        ++m_current_index;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::Iterator_Base::operator--()
    {
        L_ASSERT(!m_begin_reached);

        m_end_reached = false;

        if(m_current_index == 0)
        {
            m_begin_reached = true;
            return;
        }

        --m_current_index;
    }



    template<typename Data_Type>
    Data_Type& Vector<Data_Type>::Iterator_Base::operator*()
    {
        L_ASSERT(m_current_index >= 0 || m_current_index < m_parent->size());

        return m_parent->m_array[m_current_index];
    }

    template<typename Data_Type>
    const Data_Type& Vector<Data_Type>::Iterator_Base::operator*() const
    {
        L_ASSERT(m_current_index >= 0 || m_current_index < m_parent->size());

        return m_parent->m_array[m_current_index];
    }

    template<typename Data_Type>
    Data_Type* Vector<Data_Type>::Iterator_Base::get_ptr()
    {
        L_ASSERT(m_current_index >= 0 || m_current_index < m_parent->size());

        return &m_parent->m_array[m_current_index];
    }



    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator_Base::begin_reached() const
    {
        if(!is_ok())
            return true;
        return m_begin_reached;
    }

    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator_Base::end_reached() const
    {
        if(!is_ok())
            return true;
        return m_end_reached;
    }


    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator_Base::is_ok() const
    {
        return (m_current_index >= 0) && (m_current_index < m_parent->size());
    }





    //	Vector::Iterator

    template<typename Data_Type>
    Vector<Data_Type>::Iterator::Iterator(Vector<Data_Type>* _parent)
        : m_it(_parent)
    {

    }



    template<typename Data_Type>
    Vector<Data_Type>::Iterator::Iterator(const Iterator& _other)
        : m_it(_other.m_it)
    {

    }

    template<typename Data_Type>
    void Vector<Data_Type>::Iterator::operator=(const Iterator& _other)
    {
        m_it = _other.m_it;
    }



    template<typename Data_Type>
    void Vector<Data_Type>::Iterator::operator++()
    {
        ++m_it;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::Iterator::operator--()
    {
        --m_it;
    }



    template<typename Data_Type>
    Data_Type& Vector<Data_Type>::Iterator::operator*()
    {
        return *m_it;
    }

    template<typename Data_Type>
    const Data_Type& Vector<Data_Type>::Iterator::operator*() const
    {
        return *m_it;
    }

    template<typename Data_Type>
    Data_Type* Vector<Data_Type>::Iterator::operator->()
    {
        return m_it.get_ptr();
    }

    template<typename Data_Type>
    const Data_Type* Vector<Data_Type>::Iterator::operator->() const
    {
        return m_it.get_ptr();
    }



    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator::begin_reached() const
    {
        return m_it.begin_reached();
    }

    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator::end_reached() const
    {
        return m_it.end_reached();
    }


    template<typename Data_Type>
    bool Vector<Data_Type>::Iterator::is_ok() const
    {
        return m_it.is_ok();
    }





    //	Vector::Const_Iterator

    template<typename Data_Type>
    Vector<Data_Type>::Const_Iterator::Const_Iterator(const Vector<Data_Type>* _parent)
        : m_it((Vector<Data_Type>*)_parent)
    {

    }



    template<typename Data_Type>
    Vector<Data_Type>::Const_Iterator::Const_Iterator(const Const_Iterator& _other)
        : m_it(_other.m_it)
    {

    }

    template<typename Data_Type>
    void Vector<Data_Type>::Const_Iterator::operator=(const Const_Iterator& _other)
    {
        m_it = _other.m_it;
    }



    template<typename Data_Type>
    void Vector<Data_Type>::Const_Iterator::operator++()
    {
        ++m_it;
    }

    template<typename Data_Type>
    void Vector<Data_Type>::Const_Iterator::operator--()
    {
        --m_it;
    }



    template<typename Data_Type>
    const Data_Type& Vector<Data_Type>::Const_Iterator::operator*() const
    {
        return *m_it;
    }

    template<typename Data_Type>
    const Data_Type* Vector<Data_Type>::Const_Iterator::operator->() const
    {
        return *m_it;
    }



    template<typename Data_Type>
    bool Vector<Data_Type>::Const_Iterator::begin_reached() const
    {
        return m_it.begin_reached();
    }

    template<typename Data_Type>
    bool Vector<Data_Type>::Const_Iterator::end_reached() const
    {
        return m_it.end_reached();
    }


    template<typename Data_Type>
    bool Vector<Data_Type>::Const_Iterator::is_ok() const
    {
        return m_it.is_ok();
    }

}


#endif // VECTOR_H
