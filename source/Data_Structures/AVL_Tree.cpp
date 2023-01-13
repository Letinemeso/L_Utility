#include <Data_Structures/AVL_Tree.h>
#include <iomanip>

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
    AVL_Node* subroot_parent = subroot->parent;
    AVL_Node** subroot_ptr_as_child = nullptr;
    if(subroot_parent != nullptr)
	subroot_ptr_as_child = subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right;
    AVL_Node* right_child = subroot->child_right;
    AVL_Node* right_child_left_child = right_child->child_left;

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
void AVL_Tree<Data_Type>::M_rotate_subtree__right(AVL_Node* _subroot)
{
    AVL_Node* subroot = _subroot;
    AVL_Node* subroot_parent = subroot->parent;
    AVL_Node** subroot_ptr_as_child = nullptr;
    if(subroot_parent != nullptr)
	subroot_ptr_as_child = subroot_parent->child_left == subroot ? &subroot_parent->child_left : &subroot_parent->child_right;
    AVL_Node* left_child = subroot->child_left;
    AVL_Node* left_child_right_child = left_child->child_right;

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
void AVL_Tree<Data_Type>::M_rotate_subtree__left_right(AVL_Node* _subroot)
{
    M_rotate_subtree__left(_subroot->child_left);
    M_rotate_subtree__right(_subroot);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::M_rotate_subtree__right_left(AVL_Node* _subroot)
{
    M_rotate_subtree__right(_subroot->child_right);
    M_rotate_subtree__left(_subroot);
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::M_balance_subtree(AVL_Node* _subroot, int _depth)
{
    if(_subroot == nullptr)
	return;



//    int balance = M_subtree_balance(_subroot);

    int left_depth, right_depth;
    if(_depth < 0)
    {
	right_depth = abs(_depth);
	left_depth = M_subtree_depth(_subroot->child_left);
    }
    else if(_depth > 0)
    {
	left_depth = abs(_depth);
	right_depth = M_subtree_depth(_subroot->child_right);
    }
    else
    {
	left_depth = M_subtree_depth(_subroot->child_left);
	right_depth = M_subtree_depth(_subroot->child_right);
    }

    int balance = left_depth - right_depth;

    if(abs(balance) < 2)
    {
	M_balance_subtree(_subroot->parent, (left_depth > right_depth ? left_depth + 1 : -(right_depth + 1)));
	return;
    }

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
typename Tree<Data_Type>::Node* AVL_Tree<Data_Type>::M_allocate_node() const
{
    return new AVL_Node;
}



template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(const Data_Type& _data)
{
    Node* node = M_allocate_node();
    node->data = new Data_Type(_data);

    ++Tree<Data_Type>::m_size;

    if(m_root == nullptr)
    {
	    m_root = node;
	    return;
    }

    Tree<Data_Type>::M_insert_node(node, m_root);

    M_balance_subtree(node);
}

template<typename Data_Type>
void AVL_Tree<Data_Type>::insert(Data_Type&& _data)
{
    Node* node = M_allocate_node();
    node->data = new Data_Type((Data_Type&&)_data);

    ++Tree<Data_Type>::m_size;

    if(m_root == nullptr)
    {
	    m_root = node;
	    return;
    }

    Tree<Data_Type>::M_insert_node(node, m_root);

    M_balance_subtree(node);
}


template<typename Data_Type>
void AVL_Tree<Data_Type>::erase(const Iterator& _where)
{
    Node* ptr = Tree<Data_Type>::M_extract_pointer_from_iterator(_where);
    Tree<Data_Type>::erase(_where);
    M_balance_subtree(ptr);
}





















































