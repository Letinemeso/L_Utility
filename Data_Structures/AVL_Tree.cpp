#include "AVL_Tree.h"

using namespace LDS;


template<typename Data_Type>
AVL_Tree<Data_Type>::AVL_Tree() : Tree<Data_Type> ()
{

}

template<typename Data_Type>
AVL_Tree<Data_Type>::~AVL_Tree()
{

}



template<typename Data_Type>
int AVL_Tree<Data_Type>::M_subtree_depth(const Node* _subroot) const
{
	if(_subroot == nullptr)
		return 0;

	int left_depth = M_subtree_depth(_subroot->child_left);
	int right_depth = M_subtree_depth(_subroot->child_right);

	return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

template<typename Data_Type>
int AVL_Tree<Data_Type>::M_subtree_balance(const Node* _subroot) const
{
	if(_subroot == nullptr)
		return 0;

	int weight_left = M_subtree_depth(_subroot->child_left);
	int weight_right = M_subtree_depth(_subroot->child_right);

	return weight_left - weight_right;
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__left(Node* _subroot)
{
	Node* subroot = _subroot;
	Node* subroot_parent = subroot->parent;
	Node** subroot_ptr_as_child = nullptr;
	if(subroot_parent != nullptr)
		subroot_ptr_as_child = subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right;
	Node* right_child = subroot->child_right;
	Node* right_child_left_child = right_child->child_left;

	subroot->parent = right_child;
	right_child->child_left = subroot;
	right_child->parent = subroot_parent;
	if(subroot_parent != nullptr)
		*subroot_ptr_as_child = right_child;

	subroot->child_right = right_child_left_child;
	if(right_child_left_child != nullptr)
		right_child_left_child->parent = subroot;
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__right(Node* _subroot)
{
	Node* subroot = _subroot;
	Node* subroot_parent = subroot->parent;
	Node** subroot_ptr_as_child = nullptr;
	if(subroot_parent != nullptr)
		subroot_ptr_as_child = subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right;
	Node* left_child = subroot->child_left;
	Node* left_child_right_child = left_child->child_right;

	subroot->parent = left_child;
	left_child->child_right = subroot;
	left_child->parent = subroot_parent;
	if(subroot_parent != nullptr)
		*subroot_ptr_as_child = left_child;

	subroot->child_left = left_child_right_child;
	if(left_child_right_child != nullptr)
		left_child_right_child->parent = subroot;
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__left_right(Node* _subroot)
{
	M_rotate_subtree__left(_subroot->child_right);
	M_rotate_subtree__right(_subroot);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__right_left(Node* _subroot)
{
	M_rotate_subtree__right(_subroot->child_left);
	M_rotate_subtree__left(_subroot);
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::M_balance_subtree(Node* _subroot)
{
	if(_subroot == nullptr)
		return;

	M_balance_subtree(_subroot->child_left);
	M_balance_subtree(_subroot->child_right);

	int balance = M_subtree_balance(_subroot);

	if(abs(balance) < 2)
		return;

	Rotation_Type rt;
	if(balance < 0)
	{
		int child_balance = M_subtree_balance(_subroot->child_right);
		if(child_balance <= 0)
			rt = Rotation_Type::left;
		else
			rt = Rotation_Type::right_left;
	}
	else
	{
		int child_balance = M_subtree_balance(_subroot->child_left);
		if(child_balance >= 0)
			rt = Rotation_Type::right;
		else
			rt = Rotation_Type::left_right;
	}

	switch (rt)
	{
	case (Rotation_Type::left):
		M_rotate_subtree__left(_subroot);
		break;
	case (Rotation_Type::right):
		M_rotate_subtree__right(_subroot);
		break;
	case (Rotation_Type::left_right):
		M_rotate_subtree__left_right(_subroot);
		break;
	case (Rotation_Type::right_left):
		M_rotate_subtree__right_left(_subroot);
		break;
	}

	M_fix_root();
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_fix_root()
{
	if(m_root == nullptr)
		return;

	while(m_root->parent != nullptr)
		m_root = m_root->parent;
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(const Data_Type& _data)
{
	Tree<Data_Type>::insert(_data);
	M_balance_subtree(m_root);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(Data_Type&& _data)
{
	Tree<Data_Type>::insert((Data_Type&&)_data);
	M_balance_subtree(m_root);
}


template<typename Data_Type>
void AVL_Tree<Data_Type>::erase(const Iterator& _where)
{
	Tree<Data_Type>::erase(_where);
	M_balance_subtree((m_root));
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::erase(const Const_Iterator& _where)
{
	Tree<Data_Type>::erase(_where);
	M_balance_subtree((m_root));
}





















































