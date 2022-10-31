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
		void heapify_subheap(unsigned int _index);
		void heapify_subheap_limited(unsigned int _index, unsigned int _limit);
		void fix_after_push(unsigned int _index);

	public:
		void build_from_raw(const LDS::Vector<Data_Type>& _from);
		void build_from_raw(LDS::Vector<Data_Type>&& _from);
		void push(const Data_Type& _data);
		void push(Data_Type&& _data);

		void heapify();
		void sort();

	public:
		unsigned int size() const;
		LDS::Vector<Data_Type>& array();
		const LDS::Vector<Data_Type>& array() const;

	};

}

#include "Binary_Heap.cpp"

#endif // BINARY_HEAP_H
