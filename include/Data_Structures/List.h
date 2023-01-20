#ifndef LIST_H
#define LIST_H

#include <Debug/Debug.h>


namespace LDS
{

	template<typename Data_Type>
	class List
	{
	private:
		struct Node
		{
			Data_Type data;

			Node* next = nullptr;
			Node* prev = nullptr;

			Node(Data_Type&& _data) : data((Data_Type&&)_data) { }
			Node(const Data_Type& _data) : data(_data) { }
		};

	private:
		class Iterator_Base final
		{
		private:
			friend class LDS::List<Data_Type>;

		private:
			const LDS::List<Data_Type>* m_parent = nullptr;

			LDS::List<Data_Type>::Node* m_current_pos = nullptr;

			bool m_begin_reached = false;
			bool m_end_reached = false;

		public:
			Iterator_Base(){}
			Iterator_Base(const LDS::List<Data_Type>* _parent);
			Iterator_Base(const Iterator_Base& _other);
			void operator=(const Iterator_Base& _other);
			~Iterator_Base();

		public:
			void operator++();
			void operator--();

		public:
			inline Data_Type& operator*();
			inline const Data_Type& operator*() const;
			inline Data_Type* get_ptr();
			inline const Data_Type* get_ptr() const;

		public:
			inline bool begin_reached() const;
			inline bool end_reached() const;

			inline bool is_ok() const;

		};

	public:
		class Iterator final
		{
		private:
			friend class LDS::List<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Iterator(LDS::List<Data_Type>* _parent);

		public:
			Iterator(){};
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
			friend class LDS::List<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Const_Iterator(const LDS::List<Data_Type>* _parent);

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
		Node* m_head = nullptr;
		Node* m_tail = nullptr;

		unsigned int m_size = 0;

	public:
		List();
		List(const List& _other);
		List(List&& _other);

		void operator=(const List& _other);
		void operator=(List&& _other);

		virtual ~List();

	public:
		void push_back(const Data_Type& _data);
		void push_back(Data_Type&& _data);
		void push_front(const Data_Type& _data);
		void push_front(Data_Type&& _data);

		void pop_back();
		void pop_front();

		void erase(const Iterator& _where);

		void clear();

	public:
		inline unsigned int size() const { return m_size; }

		Iterator begin();
		Const_Iterator begin() const;
		Iterator end();
		Const_Iterator end() const;

	};

}


#include "../source/Data_Structures/List.cpp"


#endif // LIST_H
