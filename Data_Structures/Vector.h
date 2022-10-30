#ifndef VECTOR_H
#define VECTOR_H

#include "Debug/Debug.h"


namespace LDS
{

	template<typename Data_Type>
	class Vector final
	{
	private:
		class Iterator_Base final
		{
		private:
			friend class Vector;

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
			Const_Iterator(Vector<Data_Type>* _parent);

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

		Iterator pop(const Iterator& _where);
		Const_Iterator pop(const Const_Iterator& _where);

	public:
		unsigned int size() const;
		unsigned int capacity() const;

		Data_Type& operator[](unsigned int _index);
		const Data_Type& operator[](unsigned int _index) const;

		Iterator iterator();
		Const_Iterator const_iterator() const;

	};

}


//	i just want to separate declaration from implementation as God intended!
#include "Vector.cpp"


#endif // VECTOR_H
