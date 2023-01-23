#include "Data_Structures/List.h"

using namespace LDS;


//	List::Iterator_Base

template<typename Data_Type>
List<Data_Type>::Iterator_Base::Iterator_Base(const List<Data_Type>* _parent)
	: m_parent(_parent)
{

}

template<typename Data_Type>
List<Data_Type>::Iterator_Base::Iterator_Base(const Iterator_Base& _other)
	: m_parent(_other.m_parent), m_current_pos(_other.m_current_pos)
{

}

template<typename Data_Type>
void List<Data_Type>::Iterator_Base::operator=(const Iterator_Base& _other)
{
	m_parent = _other.m_parent;
	m_current_pos = _other.m_current_pos;
}

template<typename Data_Type>
List<Data_Type>::Iterator_Base::~Iterator_Base()
{

}



template<typename Data_Type>
void List<Data_Type>::Iterator_Base::operator++()
{
	L_ASSERT(!m_end_reached);

	m_begin_reached = false;

	if(m_current_pos->next == nullptr)
	{
		m_end_reached = true;
		return;
	}

	m_current_pos = m_current_pos->next;
}

template<typename Data_Type>
void List<Data_Type>::Iterator_Base::operator--()
{
	L_ASSERT(!m_begin_reached);

	m_end_reached = false;

	if(m_current_pos->prev == nullptr)
	{
		m_begin_reached = true;
		return;
	}

	m_current_pos = m_current_pos->prev;
}



template<typename Data_Type>
Data_Type& List<Data_Type>::Iterator_Base::operator*()
{
	L_ASSERT(is_ok());

	return m_current_pos->data;
}

template<typename Data_Type>
const Data_Type& List<Data_Type>::Iterator_Base::operator*() const
{
	L_ASSERT(is_ok());

	return m_current_pos->data;
}

template<typename Data_Type>
Data_Type* List<Data_Type>::Iterator_Base::get_ptr()
{
	L_ASSERT(is_ok());

	return &m_current_pos->data;
}

template<typename Data_Type>
const Data_Type* List<Data_Type>::Iterator_Base::get_ptr() const
{
	L_ASSERT(is_ok());

	return &m_current_pos->data;
}



template<typename Data_Type>
bool List<Data_Type>::Iterator_Base::begin_reached() const
{
	return m_begin_reached;
}

template<typename Data_Type>
bool List<Data_Type>::Iterator_Base::end_reached() const
{
	return m_end_reached;
}


template<typename Data_Type>
bool List<Data_Type>::Iterator_Base::is_ok() const
{
	return (m_current_pos) && (m_parent);
}





//	List::Iterator

template<typename Data_Type>
List<Data_Type>::Iterator::Iterator(List<Data_Type>* _parent)
	: m_it(_parent)
{

}



template<typename Data_Type>
List<Data_Type>::Iterator::Iterator(const Iterator& _other)
	: m_it(_other.m_it)
{

}

template<typename Data_Type>
void List<Data_Type>::Iterator::operator=(const Iterator& _other)
{
	m_it = _other.m_it;
}



template<typename Data_Type>
void List<Data_Type>::Iterator::operator++()
{
	++m_it;
}

template<typename Data_Type>
void List<Data_Type>::Iterator::operator--()
{
	--m_it;
}



template<typename Data_Type>
Data_Type& List<Data_Type>::Iterator::operator*()
{
	return *m_it;
}

template<typename Data_Type>
const Data_Type& List<Data_Type>::Iterator::operator*() const
{
	return *m_it;
}

template<typename Data_Type>
Data_Type* List<Data_Type>::Iterator::operator->()
{
	return m_it.get_ptr();
}

template<typename Data_Type>
const Data_Type* List<Data_Type>::Iterator::operator->() const
{
	return m_it.get_ptr();
}



template<typename Data_Type>
bool List<Data_Type>::Iterator::begin_reached() const
{
	return m_it.begin_reached();
}

template<typename Data_Type>
bool List<Data_Type>::Iterator::end_reached() const
{
	return m_it.end_reached();
}


template<typename Data_Type>
bool List<Data_Type>::Iterator::is_ok() const
{
	return m_it.is_ok();
}





//	List::Const_Iterator

template<typename Data_Type>
List<Data_Type>::Const_Iterator::Const_Iterator(const List<Data_Type>* _parent)
	: m_it(_parent)
{

}



template<typename Data_Type>
List<Data_Type>::Const_Iterator::Const_Iterator(const Const_Iterator& _other)
	: m_it(_other.m_it)
{

}

template<typename Data_Type>
void List<Data_Type>::Const_Iterator::operator=(const Const_Iterator& _other)
{
	m_it = _other.m_it;
}



template<typename Data_Type>
void List<Data_Type>::Const_Iterator::operator++()
{
	++m_it;
}

template<typename Data_Type>
void List<Data_Type>::Const_Iterator::operator--()
{
	--m_it;
}



template<typename Data_Type>
const Data_Type& List<Data_Type>::Const_Iterator::operator*() const
{
	return *m_it;
}

template<typename Data_Type>
const Data_Type* List<Data_Type>::Const_Iterator::operator->() const
{
	return m_it.get_ptr();
}



template<typename Data_Type>
bool List<Data_Type>::Const_Iterator::begin_reached() const
{
	return m_it.begin_reached();
}

template<typename Data_Type>
bool List<Data_Type>::Const_Iterator::end_reached() const
{
	return m_it.end_reached();
}


template<typename Data_Type>
bool List<Data_Type>::Const_Iterator::is_ok() const
{
	return m_it.is_ok();
}



//	List

template<typename Data_Type>
List<Data_Type>::List()
{

}

template<typename Data_Type>
List<Data_Type>::List(const List& _other)
{
	const Node* other_tptr = _other.m_head;
	while(other_tptr != nullptr)
	{
		push_back(other_tptr->data);
		other_tptr = other_tptr->next;
	}
}

template<typename Data_Type>
List<Data_Type>::List(List&& _other)
{
	m_size = _other.m_size;
	_other.m_size = 0;
	m_head = _other.m_head;
	_other.m_head = nullptr;
	m_tail = _other.m_tail;
	_other.m_tail = nullptr;
}


template<typename Data_Type>
void List<Data_Type>::operator=(const List& _other)
{
	m_size = _other.m_size;
	const Node* other_tptr = _other.m_head;
	while(other_tptr != nullptr)
	{
		push_back(other_tptr->data);
		other_tptr = other_tptr->next;
	}
}

template<typename Data_Type>
void List<Data_Type>::operator=(List&& _other)
{
	m_size = _other.m_size;
	_other.m_size = 0;
	m_head = _other.m_head;
	_other.m_head = nullptr;
	m_tail = _other.m_tail;
	_other.m_tail = nullptr;
}


template<typename Data_Type>
List<Data_Type>::~List()
{
	clear();
}



template<typename Data_Type>
void List<Data_Type>::push_back(const Data_Type& _data)
{
	Node* node = new Node(_data);

	if(m_tail == nullptr)
	{
		m_head = node;
		m_tail = node;
	}
	else
	{
		m_tail->next = node;
		node->prev = m_tail;
		m_tail = m_tail->next;
	}

	++m_size;
}

template<typename Data_Type>
void List<Data_Type>::push_back(Data_Type&& _data)
{
	Node* node = new Node((Data_Type&&)_data);

	if(m_tail == nullptr)
	{
		m_head = node;
		m_tail = node;
	}
	else
	{
		m_tail->next = node;
		node->prev = m_tail;
		m_tail = m_tail->next;
	}

	++m_size;
}

template<typename Data_Type>
void List<Data_Type>::push_front(const Data_Type& _data)
{
	Node* node = new Node(_data);

	if(m_head == nullptr)
	{
		m_head = node;
		m_tail = node;
	}
	else
	{
		m_head->prev = node;
		node->next = m_head;
		m_head = node;
	}

	++m_size;
}

template<typename Data_Type>
void List<Data_Type>::push_front(Data_Type&& _data)
{
	Node* node = new Node((Data_Type&&)_data);

	if(m_tail == nullptr)
	{
		m_head = node;
		m_tail = node;
	}
	else
	{
		m_head->prev = node;
		node->next = m_head;
		m_head = node;
	}

	++m_size;
}


template<typename Data_Type>
void List<Data_Type>::pop_back()
{
	L_ASSERT(m_head);

	if(m_size == 1)
	{
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
	}
	else
	{
		Node* node = m_tail;
		m_tail = m_tail->prev;
		m_tail->next = nullptr;
		delete node;
	}

	--m_size;
}

template<typename Data_Type>
void List<Data_Type>::pop_front()
{
	L_ASSERT(m_head);

	if(m_size == 1)
	{
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
	}
	else
	{
		Node* node = m_head;
		m_head = m_head->next;
		m_head->prev = nullptr;
		delete node;
	}

	--m_size;
}


template<typename Data_Type>
void List<Data_Type>::erase(const Iterator& _where)
{
	L_ASSERT(m_head);
	L_ASSERT(_where.is_ok());
	L_ASSERT(_where.m_it.m_parent == this);

	if(m_size == 1)
	{
		L_ASSERT(_where.m_it.m_current_pos == m_head);
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
	}
	else if(_where.m_it.m_current_pos == m_head)
	{
		Node* node = m_head;
		m_head = m_head->next;
		m_head->prev = nullptr;
		delete node;
	}
	else if(_where.m_it.m_current_pos == m_tail)
	{
		Node* node = m_tail;
		m_tail = m_tail->prev;
		m_tail->next = nullptr;
		delete node;
	}
	else
	{
		Node* cur = _where.m_it.m_current_pos;
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		delete cur;
	}

	--m_size;
}

template<typename Data_Type>
void List<Data_Type>::clear()
{
	while(m_head != nullptr)
	{
		Node* tptr = m_head;
		m_head = m_head->next;
		delete tptr;
	}
	m_tail = nullptr;
	m_size = 0;
}


template<typename Data_Type>
void List<Data_Type>::append(const List<Data_Type>& _other)
{
	Node* ptr = _other.m_head;
	while(ptr != nullptr)
	{
		push_back(ptr->data);
		ptr = ptr->next;
	}
}

template<typename Data_Type>
void List<Data_Type>::append(List<Data_Type>&& _other)
{
	m_size += _other.m_size;

	if(m_head == nullptr)
	{
		m_head = _other.m_head;
		m_tail = _other.m_tail;
	}
	else
	{
		m_tail->next = _other.m_head;
		if(_other.m_tail != nullptr)
			m_tail = _other.m_tail;
	}

	_other.m_head = nullptr;
	_other.m_tail = nullptr;
	_other.m_size = 0;
}



template<typename Data_Type>
typename List<Data_Type>::Iterator List<Data_Type>::begin()
{
	Iterator result(this);
	result.m_it.m_current_pos = m_head;
	return result;
}

template<typename Data_Type>
typename List<Data_Type>::Const_Iterator List<Data_Type>::begin() const
{
	Const_Iterator result(this);
	result.m_it.m_current_pos = m_head;
	return result;
}

template<typename Data_Type>
typename List<Data_Type>::Iterator List<Data_Type>::end()
{
	Iterator result(this);
	result.m_it.m_current_pos = m_tail;
	result.m_it.m_begin_reached = false;
	result.m_it.m_end_reached = true;
	return result;
}

template<typename Data_Type>
typename List<Data_Type>::Const_Iterator List<Data_Type>::end() const
{
	Const_Iterator result(this);
	result.m_it.m_current_pos = m_tail;
	result.m_it.m_begin_reached = false;
	result.m_it.m_end_reached = true;
	return result;
}










































































