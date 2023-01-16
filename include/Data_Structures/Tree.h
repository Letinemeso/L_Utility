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
			Data_Type data;

			Node* parent = nullptr;

			Node* child_left = nullptr;
			Node* child_right = nullptr;

			Node(const Data_Type& _data) : data(_data) { }
			Node(Data_Type&& _data) : data((Data_Type&&)_data) { }
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
			Iterator_Base(){}
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
			inline Data_Type& operator*();
			inline const Data_Type& operator*() const;
			inline Data_Type* get_ptr();

		public:
			inline bool begin_reached() const;
			inline bool end_reached() const;

			inline bool is_ok() const;

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
		Tree(const Tree<Data_Type>& _other);
		virtual void operator=(const Tree<Data_Type>& _other);
		Tree(Tree<Data_Type>&& _other);
		virtual void operator=(Tree<Data_Type>&& _other);
		virtual ~Tree();

	protected:
		virtual void M_insert_node(Node* _subtree, Node* _insert_where);
		Node* M_extract_pointer_from_iterator(const Iterator& _it);
		virtual Node* M_allocate_node(const Data_Type& _data) const;
		virtual Node* M_allocate_node(Data_Type&& _data) const;
		virtual void M_erase_node(Node* _node);
		Node* M_find_minimal_in_subtree(Node* _subroot) const;

	private:
		void M_copy_subtree(Node* _subroot, Node*& _where);
		void M_erase_subtree(Node*& _subroot);

	public:
		virtual void insert(const Data_Type& _data);
		virtual void insert(Data_Type&& _data);

		virtual void erase(const Iterator& _where);

		void clear();

	public:
		Iterator iterator();
		Const_Iterator iterator() const;

		Iterator find(const Data_Type& _value);
		Const_Iterator find(const Data_Type& _value) const;

		unsigned int size() const;

	};

}

#include "../../source/Data_Structures/Tree.cpp"

#endif // TREE_H
