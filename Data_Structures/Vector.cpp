#include "Vector.h"

using namespace LDS;


template<typename Data_Type>
Vector<Data_Type>::Vector()
{
	m_size = 5;

	m_array = new Data_Type * [m_size];
	for(unsigned int i=0; i<m_size; ++i)
		m_array[i] = nullptr;
}

template<typename Data_Type>
Vector<Data_Type>::~Vector()
{
	for(unsigned int i=0; i<m_size; ++i)
		delete m_array[i];
	delete[] m_array;
}



template<typename Data_Type>
void Vector<Data_Type>::resize(unsigned int _new_size)
{
	Data_Type** temp = new Data_Type * [_new_size];


	if(_new_size < m_size)
	{
		for(unsigned int i=0; i<_new_size; ++i)
			temp[i] = m_array[i];
		for(unsigned int i=_new_size; i < m_size; ++i)
			delete m_array[i];
	}
	else
	{
		for(unsigned int i=0; i<m_size; ++i)
			temp[i] = m_array[i];
		for(unsigned int i=m_size; i < _new_size; ++i)
			temp[i] = nullptr;
	}

	delete[] m_array;
	m_array = temp;
	m_size = _new_size;
}

template<typename Data_Type>
void Vector<Data_Type>::clear()
{
	for(unsigned int i=0; i<m_size; ++i)
	{
		delete m_array[i];
		m_array[i] = nullptr;
	}
}



template<typename Data_Type>
void Vector<Data_Type>::push(const Data_Type& _data)
{
	if(m_elements_count == m_size)
		resize(m_size + 10);
	m_array[m_elements_count] = new Data_Type(_data);
	++m_elements_count;
}

template<typename Data_Type>
void Vector<Data_Type>::push(Data_Type&& _data)
{
	if(m_elements_count == m_size)
		resize(m_size + 10);
	m_array[m_elements_count] = new Data_Type((Data_Type&&)_data);
	++m_elements_count;
}



template<typename Data_Type>
unsigned int Vector<Data_Type>::size() const
{
	return m_elements_count;
}

template<typename Data_Type>
unsigned int Vector<Data_Type>::capacity() const
{
	return m_size;
}


template<typename Data_Type>
Data_Type& Vector<Data_Type>::operator[](unsigned int _index)
{
	L_ASSERT(_index < m_elements_count);

	return *(m_array[_index]);
}

template<typename Data_Type>
const Data_Type& Vector<Data_Type>::operator[](unsigned int _index) const
{
	L_ASSERT(_index < m_elements_count);

	return *(m_array[_index]);
}



































































