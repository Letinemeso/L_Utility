#ifndef VECTOR_H
#define VECTOR_H

#include "Debug/Debug.h"


namespace LDS
{

	template<typename Data_Type>
	class Vector final
	{/*
	private:
		class Base_Iterator final
		{
		private:
			Vector<Data_Type>* m_parent = nullptr;

		public:
			Base_Iterator(Vector<Data_Type>* _parent);
			Base_Iterator(const Base_Iterator& _other);
			void operator=(const Base_Iterator& _other);
			~Base_Iterator();

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;

		};

	public:
		class Iterator final
		{
		private:
			Base_Iterator m_it;

		public:
			Iterator();

		public:
			void operator++();
			void operator--();

		public:
			Data_Type& operator*();
			const Data_Type& operator*() const;

		};

		class Const_Iterator final
		{
		private:
			Base_Iterator m_it;

		public:
			Const_Iterator();

		public:
			void operator++();
			void operator--();

		public:
			const Data_Type& operator*() const;

		};
*/
	private:
		Data_Type** m_array = nullptr;
		unsigned int m_elements_count = 0;
		unsigned int m_size = 0;

	public:
		Vector();
		~Vector();

	public:
		void resize(unsigned int _new_size);
		void clear();

	public:
		void push(const Data_Type& _data);
		void push(Data_Type&& _data);

//		void pop();

	public:
		unsigned int size() const;
		unsigned int capacity() const;

		Data_Type& operator[](unsigned int _index);
		const Data_Type& operator[](unsigned int _index) const;

//		Iterator begin();
//		Const_Iterator begin() const;

	};

}


//	i just want to separate declaration from implementation as God intended!
#include "Vector.cpp"


#endif // VECTOR_H
