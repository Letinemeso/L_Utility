#include <Data_Structures/Tree.h>

using namespace LDS;


//	Tree

template<typename Data_Type>
Tree<Data_Type>::Tree()
{

}

template<typename Data_Type>
Tree<Data_Type>::Tree(const Tree<Data_Type>& _other)
{
	m_size = _other.m_size;
	M_copy_subtree(_other.m_root, m_root);
}

template<typename Data_Type>
void Tree<Data_Type>::operator=(const Tree<Data_Type>& _other)
{
	clear();
	m_size = _other.m_size;
	M_copy_subtree(_other.m_root, m_root);
}

template<typename Data_Type>
Tree<Data_Type>::Tree(Tree<Data_Type>&& _other)
{
	m_size = _other.m_size;
	_other.m_size = 0;
	m_root = _other.m_root;
	_other.m_root = nullptr;
}

template<typename Data_Type>
void Tree<Data_Type>::operator=(Tree<Data_Type>&& _other)
{
	clear();
	m_size = _other.m_size;
	_other.m_size = 0;
	m_root = _other.m_root;
	_other.m_root = nullptr;
}

template<typename Data_Type>
Tree<Data_Type>::~Tree()
{
	M_erase_subtree(m_root);
}



template<typename Data_Type>
void Tree<Data_Type>::M_insert_node(Node* _subtree, Node* _insert_where)
{
	Node* future_parent = _insert_where;

	while(true)
	{
		if(!future_parent->child_left && !future_parent->child_right)
			break;

		if(*_subtree->data < *future_parent->data)
		{
			if(future_parent->child_left)
				future_parent = future_parent->child_left;
			else
				future_parent = future_parent->child_right;
		}
		else
		{
			if(future_parent->child_right)
				future_parent = future_parent->child_right;
			else
				future_parent = future_parent->child_left;
		}
	}

	if(*_subtree->data < *future_parent->data)
		future_parent->child_left = _subtree;
	else
		future_parent->child_right = _subtree;
	_subtree->parent = future_parent;
}

template<typename Data_Type>
typename Tree<Data_Type>::Node* Tree<Data_Type>::M_extract_pointer_from_iterator(const Iterator& _it)
{
	return _it.m_it.m_current_pos;
}

template<typename Data_Type>
typename Tree<Data_Type>::Node* Tree<Data_Type>::M_allocate_node() const
{
	return new Node;
}

template<typename Data_Type>
void Tree<Data_Type>::M_erase_node(Node* _node)
{
	L_ASSERT(_node);

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

		delete _node->data;
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

		delete _node->data;
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

		delete _node->data;
		delete _node;

		--m_size;
	}
	else
	{
		Node* next_minimal = M_find_minimal_in_subtree(_node->child_right);
		delete _node->data;
		_node->data = next_minimal->data;
		next_minimal->data = nullptr;
		M_erase_node(next_minimal);
	}
}

template<typename Data_Type>
typename Tree<Data_Type>::Node* Tree<Data_Type>::M_find_minimal_in_subtree(Node* _subroot) const
{
	while(_subroot->child_left != nullptr)
		_subroot = _subroot->child_left;
	return _subroot;
}



template<typename Data_Type>
void Tree<Data_Type>::M_copy_subtree(Node* _subroot, Node*& _where)
{
	if(_subroot == nullptr)
		return;

	L_ASSERT(_where == nullptr);

	_where = M_allocate_node();
	_where->data = new Data_Type(*_subroot->data);

	M_copy_subtree(_subroot->child_left, _where->child_left);
	M_copy_subtree(_subroot->child_right, _where->child_right);

	if(_where->child_left != nullptr)
		_where->child_left->parent = _where;
	if(_where->child_right != nullptr)
		_where->child_right->parent = _where;
}

template<typename Data_Type>
void Tree<Data_Type>::M_erase_subtree(Node*& _subroot)
{
	if(_subroot == nullptr)
		return;

	//	recursuve
	M_erase_subtree(_subroot->child_left);
	M_erase_subtree(_subroot->child_right);

	delete _subroot->data;
	delete _subroot;

	//	iterative
	/*Node* parent = _subroot;

	while(_subroot != nullptr)
	{
		if(parent->child_left)
			parent = parent->child_left;
		else if(parent->child_right)
			parent = parent->child_right;
		else
		{
			Node* parent_parent = parent->parent;
			if(parent_parent == nullptr)
			{
				delete _subroot;
				_subroot = nullptr;
				return;
			}

			if(parent == parent_parent->child_left)
				parent_parent->child_left = nullptr;
			else
				parent_parent->child_right = nullptr;

			delete parent->data;
			delete parent;

			parent = parent_parent;
		}
	}*/
}



template<typename Data_Type>
void Tree<Data_Type>::insert(const Data_Type& _data)
{
	Node* node = M_allocate_node();
	node->data = new Data_Type(_data);

	++m_size;

	if(m_root == nullptr)
	{
		m_root = node;
		return;
	}

	M_insert_node(node, m_root);
}

template<typename Data_Type>
void Tree<Data_Type>::insert(Data_Type&& _data)
{
	Node* node = M_allocate_node();
	node->data = new Data_Type((Data_Type&&)_data);

	++m_size;

	if(m_root == nullptr)
	{
		m_root = node;
		return;
	}

	M_insert_node(node, m_root);
}


template<typename Data_Type>
void Tree<Data_Type>::erase(const Iterator& _where)
{
	L_ASSERT(_where.m_it.m_parent == this);
	L_ASSERT(_where.is_ok());

	M_erase_node(_where.m_it.m_current_pos);
}


template<typename Data_Type>
void Tree<Data_Type>::clear()
{
	M_erase_subtree(m_root);
	m_root = nullptr;
	m_size = 0;
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
typename Tree<Data_Type>::Iterator Tree<Data_Type>::find(const Data_Type& _value)
{
	Node* search = m_root;
	while(search != nullptr)
	{
		if(_value < *search->data)
			search = search->child_left;
		else if(_value > *search->data)
			search = search->child_right;
		else
		{
			Iterator result(this);
			result.m_it.m_current_pos = search;
			return result;
		}
	}
	return Iterator(nullptr);
}

template<typename Data_Type>
typename Tree<Data_Type>::Const_Iterator Tree<Data_Type>::find(const Data_Type& _value) const
{
	Node* search = m_root;
	while(search != nullptr)
	{
		if(_value < *search->data)
			search = search->child_left;
		else if(_value > *search->data)
			search = search->child_right;
		else
		{
			Const_Iterator result(this);
			result.m_it.m_current_pos = search;
			return result;
		}
	}
	return Const_Iterator(nullptr);
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
	if(_parent == nullptr)
		return;

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












































































