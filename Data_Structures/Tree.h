#ifndef TREE_H
#define TREE_H

#include "Debug/Debug.h"


namespace LDS
{

	template<typename Data_Type>
	class Tree
	{
	protected:
		struct Node
		{
			Data_Type* data = nullptr;

			Node* parent = nullptr;

			Node* child_left = nullptr;
			Node* child_right = nullptr;
		};

	protected:
		Node* m_root = nullptr;
		unsigned int m_size = 0;

	private:
		class Iterator_Base final
		{
		private:
			friend class LDS::Tree<Data_Type>;

		private:
			LDS::Tree<Data_Type>* m_parent = nullptr;

			LDS::Tree<Data_Type>::Node* m_current_pos = nullptr;

			bool m_begin_reached = false;
			bool m_end_reached = false;

		public:
			Iterator_Base(LDS::Tree<Data_Type>* _parent);
			Iterator_Base(const Iterator_Base& _other);
			void operator=(const Iterator_Base& _other);
			~Iterator_Base();

		private:
			bool is_left_child() const;
			Tree<Data_Type>::Node* closest_right_child(Node* _closest_to) const;

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
			friend class LDS::Tree<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Iterator(LDS::Tree<Data_Type>* _parent);

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
			friend class LDS::Tree<Data_Type>;

		private:
			Iterator_Base m_it;

		private:
			Const_Iterator(LDS::Tree<Data_Type>* _parent);

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

	public:
		Tree();
		virtual ~Tree();

	private:
		void M_insert_node(Node* _subtree, Node* _insert_where);
		void M_erase_subtree(Node* _subroot);
		Node* M_find_minimal_in_subtree(Node* _subroot) const;
		void M_erase_node(Node* _node);

	public:
		virtual void insert(const Data_Type& _data);
		virtual void insert(Data_Type&& _data);

		virtual void erase(const Iterator& _where);
		virtual void erase(const Const_Iterator& _where);

	public:
		Iterator iterator();
		Const_Iterator iterator() const;

		unsigned int size() const;

	};

}

#include "Tree.cpp"

#endif // TREE_H
