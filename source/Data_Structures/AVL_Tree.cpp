#include <Data_Structures/AVL_Tree.h>

using namespace LDS;


template<typename Data_Type>
AVL_Tree<Data_Type>::AVL_Tree() : Tree<Data_Type> ()
{

}

template<typename Data_Type>
AVL_Tree<Data_Type>::AVL_Tree(const AVL_Tree<Data_Type>& _other) : Tree<Data_Type> (_other)
{

}

template<typename Data_Type>
void AVL_Tree<Data_Type>::operator=(const AVL_Tree<Data_Type>& _other)
{
    ((Tree<Data_Type>*)this)->operator=(_other);
}

template<typename Data_Type>
AVL_Tree<Data_Type>::AVL_Tree(AVL_Tree<Data_Type>&& _other) : Tree<Data_Type> ((Tree<Data_Type>&&)_other)
{

}

template<typename Data_Type>
void AVL_Tree<Data_Type>::operator=(AVL_Tree<Data_Type>&& _other)
{
    ((Tree<Data_Type>*)this)->operator=((Tree<Data_Type>&&)_other);
}

template<typename Data_Type>
AVL_Tree<Data_Type>::~AVL_Tree()
{

}



template<typename Data_Type>
int AVL_Tree<Data_Type>::M_subtree_depth(const AVL_Node* _subroot) const
{
    if(_subroot == nullptr)
	return 0;

    int left_depth = M_subtree_depth(_subroot->child_left);
    int right_depth = M_subtree_depth(_subroot->child_right);

    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

template<typename Data_Type>
int AVL_Tree<Data_Type>::M_subtree_balance(const AVL_Node* _subroot) const
{
    if(_subroot == nullptr)
	return 0;

    int weight_left = M_subtree_depth(_subroot->child_left);
    int weight_right = M_subtree_depth(_subroot->child_right);

    return weight_left - weight_right;
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__left(AVL_Node* _subroot)
{
    AVL_Node* subroot = _subroot;
	AVL_Node* subroot_parent = (AVL_Node*)subroot->parent;
    AVL_Node** subroot_ptr_as_child = nullptr;
    if(subroot_parent != nullptr)
	subroot_ptr_as_child = (AVL_Node**)(subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right);
	AVL_Node* right_child = (AVL_Node*)subroot->child_right;
	AVL_Node* right_child_left_child = (AVL_Node*)right_child->child_left;

    subroot->parent = right_child;
    right_child->child_left = subroot;
    right_child->parent = subroot_parent;
    if(subroot_parent != nullptr)
	*subroot_ptr_as_child = right_child;

    subroot->child_right = right_child_left_child;
    if(right_child_left_child != nullptr)
	right_child_left_child->parent = subroot;

	M_update_node_depth(subroot);
	M_update_node_balance(subroot);
	M_update_node_depth(right_child);
	M_update_node_balance(right_child);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__right(AVL_Node* _subroot)
{
    AVL_Node* subroot = _subroot;
	AVL_Node* subroot_parent = (AVL_Node*)subroot->parent;
    AVL_Node** subroot_ptr_as_child = nullptr;
    if(subroot_parent != nullptr)
	subroot_ptr_as_child = (AVL_Node**)(subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right);
	AVL_Node* left_child = (AVL_Node*)subroot->child_left;
	AVL_Node* left_child_right_child = (AVL_Node*)left_child->child_right;

    subroot->parent = left_child;
    left_child->child_right = subroot;
    left_child->parent = subroot_parent;
    if(subroot_parent != nullptr)
	*subroot_ptr_as_child = left_child;

    subroot->child_left = left_child_right_child;
    if(left_child_right_child != nullptr)
	left_child_right_child->parent = subroot;

	M_update_node_depth(subroot);
	M_update_node_balance(subroot);
	M_update_node_depth(left_child);
	M_update_node_balance(left_child);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__left_right(AVL_Node* _subroot)
{
	M_rotate_subtree__left((AVL_Node*)_subroot->child_left);
    M_rotate_subtree__right(_subroot);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__right_left(AVL_Node* _subroot)
{
	M_rotate_subtree__right((AVL_Node*)_subroot->child_right);
    M_rotate_subtree__left(_subroot);
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::M_balance_subtree(AVL_Node* _subroot, int _depth)
{
	if(_subroot == nullptr)
		return;

	int balance = _subroot->balance;

	if(abs(balance) < 2)
		return;

	Rotation_Type rt;
	if(balance < 0)
	{
		int child_balance = ((AVL_Node*)_subroot->child_right)->balance;
		if(child_balance <= 0)
			rt = Rotation_Type::left;
		else
			rt = Rotation_Type::right_left;
	}
	else
	{
		int child_balance = ((AVL_Node*)_subroot->child_left)->balance;
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
void AVL_Tree<Data_Type>::M_update_node_depth(AVL_Node* _node)
{
	if(_node->child_left && !_node->child_right)
		_node->depth = ((AVL_Node*)_node->child_left)->depth + 1;
	else if(!_node->child_left && _node->child_right)
		_node->depth = ((AVL_Node*)_node->child_right)->depth + 1;
	else if(_node->child_left && _node->child_right)
		_node->depth = (((AVL_Node*)_node->child_left)->depth > ((AVL_Node*)_node->child_right)->depth ? ((AVL_Node*)_node->child_left)->depth : ((AVL_Node*)_node->child_right)->depth) + 1;
	else
		_node->depth = 1;
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_update_node_balance(AVL_Node* _node)
{
	int depth_left = 0;
	int depth_right = 0;

	if(_node->child_left)
		depth_left = ((AVL_Node*)_node->child_left)->depth;
	if(_node->child_right)
		depth_right = ((AVL_Node*)_node->child_right)->depth;

	_node->balance = depth_left - depth_right;
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_backtrace(Node* _from_where)
{
	if(_from_where->parent == nullptr)
		return;

//	AVL_Node* parent = (AVL_Node*)_from_where->parent;
	AVL_Node* parent = (AVL_Node*)_from_where;

	while(parent != nullptr)
	{
		M_update_node_depth(parent);
		M_update_node_balance(parent);

		if(abs(parent->balance) > 1)
		{
			M_balance_subtree(parent);
			return;
		}

		parent = (AVL_Node*)parent->parent;
	}
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
typename Tree<Data_Type>::Node* AVL_Tree<Data_Type>::M_allocate_node(const Data_Type& _data) const
{
	return new AVL_Node(_data);
}

template<typename Data_Type>
typename Tree<Data_Type>::Node* AVL_Tree<Data_Type>::M_allocate_node(Data_Type&& _data) const
{
	return new AVL_Node((Data_Type&&)_data);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_erase_node(Node* _node)
{
	L_ASSERT(_node);

	Node* parent = _node->parent;

	if(_node->child_left == nullptr && _node->child_right == nullptr)
	{
		if(_node->parent)
		{
			if(_node->parent->child_left == _node)
				_node->parent->child_left = nullptr;
			else
				_node->parent->child_right = nullptr;
		}
		else
			m_root = nullptr;

		delete _node;

		--m_size;
	}
	else if(_node->child_left == nullptr && _node->child_right != nullptr)
	{
		Node* right = _node->child_right;
		if(_node->parent)
		{
			if(_node->parent->child_left == _node)
				_node->parent->child_left = right;
			else
				_node->parent->child_right = right;
			right->parent = _node->parent;
		}
		else
		{
			m_root = right;
			right->parent = nullptr;
		}

		delete _node;

		--m_size;
	}
	else if(_node->child_left != nullptr && _node->child_right == nullptr)
	{
		Node* left = _node->child_left;
		if(_node->parent)
		{
			if(_node->parent->child_left == _node)
				_node->parent->child_left = left;
			else
				_node->parent->child_right = left;
			left->parent = _node->parent;
		}
		else
		{
			m_root = left;
			left->parent = nullptr;
		}

		delete _node;

		--m_size;
	}
	else
	{
		Node* next_minimal = Tree<Data_Type>::M_find_minimal_in_subtree(_node->child_right);
		parent = next_minimal->parent;
		_node->data = (Data_Type&&)next_minimal->data;
		M_erase_node(next_minimal);
	}

	if(parent)
		M_backtrace(parent);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_insert_node(Node* _subtree, Node* _insert_where)
{
	Tree<Data_Type>::M_insert_node(_subtree, _insert_where);

	M_backtrace(_subtree->parent);
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(const Data_Type& _data)
{
	Node* node = M_allocate_node(_data);

    ++Tree<Data_Type>::m_size;

    if(m_root == nullptr)
    {
	    m_root = node;
	    return;
    }

	M_insert_node(node, m_root);

	M_balance_subtree((AVL_Node*)node);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(Data_Type&& _data)
{
	Node* node = M_allocate_node((Data_Type&&)_data);

    ++Tree<Data_Type>::m_size;

    if(m_root == nullptr)
    {
	    m_root = node;
	    return;
    }

	M_insert_node(node, m_root);

	M_balance_subtree((AVL_Node*)node);
}


template<typename Data_Type>
void AVL_Tree<Data_Type>::erase(const Iterator& _where)
{
    Node* ptr = Tree<Data_Type>::M_extract_pointer_from_iterator(_where);
    Tree<Data_Type>::erase(_where);
//	M_balance_subtree((AVL_Node*)ptr);
}





















































