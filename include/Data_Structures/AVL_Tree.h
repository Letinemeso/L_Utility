#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <cmath>

#include "Tree.h"


namespace LDS
{

	template<typename Data_Type>
	class AVL_Tree : public Tree<Data_Type>
	{
	private:
		using typename Tree<Data_Type>::Node;
		using Tree<Data_Type>::m_root;

	public:
		using typename Tree<Data_Type>::Iterator;
		using typename Tree<Data_Type>::Const_Iterator;

	private:
		enum class Rotation_Type
		{
			right,
			left,
			right_left,
			left_right
		};

	public:
		AVL_Tree();
		AVL_Tree(const AVL_Tree<Data_Type>& _other);
		void operator=(const AVL_Tree<Data_Type>& _other);
		AVL_Tree(AVL_Tree<Data_Type>&& _other);
		void operator=(AVL_Tree<Data_Type>&& _other);
		~AVL_Tree();

	private:
		int M_subtree_depth(const Node* _subroot) const;
		int M_subtree_balance(const Node* _subroot) const;

	private:
		void M_rotate_subtree__left(Node* _subroot);
		void M_rotate_subtree__right(Node* _subroot);
		void M_rotate_subtree__left_right(Node* _subroot);
		void M_rotate_subtree__right_left(Node* _subroot);

		void M_balance_subtree(Node* _subroot);
		void M_fix_root();

	public:
		void insert(const Data_Type& _data) override;
		void insert(Data_Type&& _data) override;

		void erase(const Iterator& _where) override;
		void erase(const Const_Iterator& _where) override;

	};

}

#include "../../source/Data_Structures/AVL_Tree.cpp"

#endif // AVL_TREE_H
