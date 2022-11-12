#include "Tree.h"

using namespace LDS;


//	Tree

template<typename Data_Type>
Tree<Data_Type>::Tree()
{

}

template<typename Data_Type>
Tree<Data_Type>::~Tree()
{
	erase_subtree(m_root);
}



template<typename Data_Type>
void Tree<Data_Type>::insert_node(Node* _subtree, Node* _insert_where)
{
	Node* left_subtree = _insert_where->child_left;
	Node* right_subtree = _insert_where->child_right;

	if(*_subtree->data < *_insert_where->data)
	{
		if(left_subtree == nullptr)
		{
			_insert_where->child_left = _subtree;
			_subtree->parent = _insert_where;
			return;
		}

		insert_node(_subtree, left_subtree);
	}
	else
	{
		if(right_subtree == nullptr)
		{
			_insert_where->child_right = _subtree;
			_subtree->parent = _insert_where;
			return;
		}

		insert_node(_subtree, right_subtree);
	}
}

template<typename Data_Type>
void Tree<Data_Type>::erase_subtree(Node* _subroot)
{
	if(_subroot == nullptr)
		return;

	erase_subtree(_subroot->child_left);
	erase_subtree(_subroot->child_right);

	if(_subroot->parent != nullptr)	//	TODO: move nullptr'ing parent into non-recursive function
		_subroot->parent = nullptr;

	delete _subroot->data;
	delete _subroot;
}

template<typename Data_Type>
void Tree<Data_Type>::erase_node(Node* _node)
{
	--m_size;

	Node* where = _node;

	Node* left = where->child_left;
	Node* right = where->child_right;
	Node* parent = where->parent;

	if(left != nullptr)
	{
		if(parent != nullptr)
		{
			if(parent->child_right == where)
				parent->child_right = left;
			else
				parent->child_left = left;
		}
		else
		{
			m_root = left;
		}

		left->parent = parent;

		if(right != nullptr)
		{
			Node* max = left;
			while(max->child_right != nullptr)
				max = max->child_right;
			max->child_right = right;
			right->parent = max;
		}
	}
	else if(right != nullptr)
	{
		if(parent != nullptr)
		{
			if(parent->child_right == where)
				parent->child_right = right;
			else
				parent->child_left = right;
		}
		else
		{
			m_root = right;
		}

		right->parent = parent;
	}
	else
	{
		if(parent != nullptr)
		{
			if(parent->child_right == where)
				parent->child_right = nullptr;
			else
				parent->child_left = nullptr;
		}
		else
		{
			m_root = nullptr;
		}
	}

	delete where->data;
	delete where;
}



template<typename Data_Type>
void Tree<Data_Type>::insert(const Data_Type& _data)
{
	Node* node = new Node;
	node->data = new Data_Type(_data);

	++m_size;

	if(m_root == nullptr)
	{
		m_root = node;
		return;
	}

	insert_node(node, m_root);
}

template<typename Data_Type>
void Tree<Data_Type>::insert(Data_Type&& _data)
{
	Node* node = new Node;
	node->data = new Data_Type((Data_Type&&)_data);

	++m_size;

	if(m_root == nullptr)
	{
		m_root = node;
		return;
	}

	insert_node(node, m_root);
}


template<typename Data_Type>
void Tree<Data_Type>::erase(const Iterator& _where)
{
	L_ASSERT(_where.m_it.m_parent == this);
	L_ASSERT(_where.is_ok());

	erase_node(_where.m_it.m_current_pos);
}

template<typename Data_Type>
void Tree<Data_Type>::erase(const Const_Iterator& _where)
{
	L_ASSERT(_where.m_it.m_parent == this);
	L_ASSERT(_where.is_ok());

	erase_node(_where.m_it.m_current_pos);
}



template<typename Data_Type>
typename Tree<Data_Type>::Iterator Tree<Data_Type>::iterator()
{
	return Iterator(this);
}

template<typename Data_Type>
typename Tree<Data_Type>::Const_Iterator Tree<Data_Type>::iterator() const
{
	return Const_Iterator(this);
}


template<typename Data_Type>
unsigned int Tree<Data_Type>::size() const
{
	return m_size;
}







//	Iterator_Base

template<typename Data_Type>
Tree<Data_Type>::Iterator_Base::Iterator_Base(Tree<Data_Type>* _parent)
{
	m_parent = _parent;
	m_current_pos = m_parent->m_root;
	if(m_current_pos == nullptr)
		m_end_reached = true;
}

template<typename Data_Type>
Tree<Data_Type>::Iterator_Base::Iterator_Base(const Iterator_Base& _other)
{
	m_parent = _other.m_parent;
	m_current_pos = _other.m_current_pos;
	m_begin_reached = _other.m_begin_reached;
	m_end_reached = _other.m_end_reached;
}

template<typename Data_Type>
void Tree<Data_Type>::Iterator_Base::operator=(const Iterator_Base& _other)
{
	m_parent = _other.m_parent;
	m_current_pos = _other.m_current_pos;
	m_begin_reached = _other.m_begin_reached;
	m_end_reached = _other.m_end_reached;
}

template<typename Data_Type>
Tree<Data_Type>::Iterator_Base::~Iterator_Base()
{

}



template<typename Data_Type>
bool Tree<Data_Type>::Iterator_Base::is_left_child() const
{
	if(m_current_pos->parent == nullptr)
		return false;
	if(m_current_pos == m_current_pos->parent->child_left)
		return true;
	return false;
}

template<typename Data_Type>
typename Tree<Data_Type>::Node* Tree<Data_Type>::Iterator_Base::closest_right_child(Node* _closest_to) const
{
	Node* prev = _closest_to;
	while(_closest_to != nullptr)
	{
		if(_closest_to->child_right != nullptr && _closest_to->child_right != prev)
			return _closest_to->child_right;
		prev = _closest_to;
		_closest_to = _closest_to->parent;
	}
	return nullptr;
}



template<typename Data_Type>
void Tree<Data_Type>::Iterator_Base::operator++()
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);
	L_ASSERT(!m_end_reached);

	m_begin_reached = false;

	if(m_current_pos->child_left != nullptr)
	{
		m_current_pos = m_current_pos->child_left;
		return;
	}

	Node* closest_right = closest_right_child(m_current_pos);
	if(closest_right == nullptr)
	{
		m_end_reached = true;
		return;
	}

	m_current_pos = closest_right;
}

template<typename Data_Type>
void Tree<Data_Type>::Iterator_Base::operator--()
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);
	L_ASSERT(!m_begin_reached);

	m_end_reached = false;

	if(m_current_pos->parent == nullptr)
		m_begin_reached = true;
	else if(is_left_child())
		m_current_pos = m_current_pos->parent;
	else if(m_current_pos->parent->child_left == nullptr)
		m_current_pos = m_current_pos->parent;
	else
	{
		Node* prev = m_current_pos->parent->child_left;
		while(prev->child_left != nullptr || prev->child_right != nullptr)
		{
			if(prev->child_right != nullptr)
				prev = prev->child_right;
			else
				prev = prev->child_left;
		}
		m_current_pos = prev;
	}
}



template<typename Data_Type>
Data_Type& Tree<Data_Type>::Iterator_Base::operator*()
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);

	return *m_current_pos->data;
}

template<typename Data_Type>
const Data_Type& Tree<Data_Type>::Iterator_Base::operator*() const
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);

	return *m_current_pos->data;
}

template<typename Data_Type>
Data_Type* Tree<Data_Type>::Iterator_Base::get_ptr()
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);

	return m_current_pos->data;
}



template<typename Data_Type>
bool Tree<Data_Type>::Iterator_Base::begin_reached() const
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);

	return m_begin_reached;
}

template<typename Data_Type>
bool Tree<Data_Type>::Iterator_Base::end_reached() const
{
	L_ASSERT(m_current_pos != nullptr);
	L_ASSERT(m_parent != nullptr);

	return m_end_reached;
}


template<typename Data_Type>
bool Tree<Data_Type>::Iterator_Base::is_ok() const
{
	return m_parent != nullptr && m_current_pos != nullptr;
}





//	Iterator

template<typename Data_Type>
Tree<Data_Type>::Iterator::Iterator(Tree<Data_Type>* _parent)
	: m_it(_parent)
{

}

template<typename Data_Type>
Tree<Data_Type>::Iterator::Iterator(const Iterator& _other)
	: m_it(_other.m_it)
{

}

template<typename Data_Type>
void Tree<Data_Type>::Iterator::operator=(const Iterator& _other)
{
	m_it = _other.m_it;
}



template<typename Data_Type>
void Tree<Data_Type>::Iterator::operator++()
{
	++m_it;
}

template<typename Data_Type>
void Tree<Data_Type>::Iterator::operator--()
{
	--m_it;
}



template<typename Data_Type>
Data_Type& Tree<Data_Type>::Iterator::operator*()
{
	return *m_it;
}

template<typename Data_Type>
const Data_Type& Tree<Data_Type>::Iterator::operator*() const
{
	return *m_it;
}

template<typename Data_Type>
Data_Type* Tree<Data_Type>::Iterator::operator->()
{
	return m_it.get_ptr();
}

template<typename Data_Type>
const Data_Type* Tree<Data_Type>::Iterator::operator->() const
{
	return m_it.get_ptr();
}



template<typename Data_Type>
bool Tree<Data_Type>::Iterator::begin_reached() const
{
	return m_it.begin_reached();
}

template<typename Data_Type>
bool Tree<Data_Type>::Iterator::end_reached() const
{
	return m_it.end_reached();
}


template<typename Data_Type>
bool Tree<Data_Type>::Iterator::is_ok() const
{
	return m_it.is_ok();
}





//	Const_Iterator

template<typename Data_Type>
Tree<Data_Type>::Const_Iterator::Const_Iterator(Tree<Data_Type>* _parent)
	: m_it(_parent)
{

}

template<typename Data_Type>
Tree<Data_Type>::Const_Iterator::Const_Iterator(const Const_Iterator& _other)
	: m_it(_other.m_it)
{

}

template<typename Data_Type>
void Tree<Data_Type>::Const_Iterator::operator=(const Const_Iterator& _other)
{
	m_it = _other.m_it;
}



template<typename Data_Type>
void Tree<Data_Type>::Const_Iterator::operator++()
{
	++m_it;
}

template<typename Data_Type>
void Tree<Data_Type>::Const_Iterator::operator--()
{
	--m_it;
}



template<typename Data_Type>
const Data_Type& Tree<Data_Type>::Const_Iterator::operator*() const
{
	return *m_it;
}

template<typename Data_Type>
const Data_Type* Tree<Data_Type>::Const_Iterator::operator->() const
{
	return m_it.get_ptr();
}



template<typename Data_Type>
bool Tree<Data_Type>::Const_Iterator::begin_reached() const
{
	return m_it.begin_reached();
}

template<typename Data_Type>
bool Tree<Data_Type>::Const_Iterator::end_reached() const
{
	return m_it.end_reached();
}


template<typename Data_Type>
bool Tree<Data_Type>::Const_Iterator::is_ok() const
{
	return m_it.is_ok();
}












































































