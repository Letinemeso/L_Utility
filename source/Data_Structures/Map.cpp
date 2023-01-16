#include <Data_Structures/Map.h>

using namespace LDS;


//	Pair

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Pair::Pair(const Key_Type& _key, const Data_Type& _data)
    : m_key(_key), m_data(_data)
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Pair::Pair(Key_Type&& _key, Data_Type&& _data)
    : m_key((Key_Type&&)_key), m_data((Data_Type&&)_data)
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Pair::Pair(const Pair& _other)
    : m_key(_other.m_key), m_data(_other.m_data)
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Pair::Pair(Pair&& _other)
    : m_key((Key_Type&&)_other.m_key), m_data((Data_Type&&)_other.m_data)
{

}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Pair::operator=(const Pair& _other)
{
    m_key = _other.m_key;
    m_data = _other.m_data;
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Pair::operator=(Pair&& _other)
{
    m_key = (Key_Type&&)_other.m_key;
    m_data = (Data_Type&&)_other.m_data;
}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Pair::~Pair()
{

}



template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Pair::operator<(const Pair& _other) const
{
    return m_key < _other.m_key;
}

template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Pair::operator>(const Pair& _other) const
{
    return m_key > _other.m_key;
}

template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Pair::operator==(const Pair& _other) const
{
    return m_key == _other.m_key;
}



template<typename Key_Type, typename Data_Type>
Key_Type& Map<Key_Type, Data_Type>::Pair::key()
{
    return m_key;
}

template<typename Key_Type, typename Data_Type>
const Key_Type& Map<Key_Type, Data_Type>::Pair::key() const
{
    return m_key;
}

template<typename Key_Type, typename Data_Type>
Data_Type& Map<Key_Type, Data_Type>::Pair::data()
{
    return m_data;
}

template<typename Key_Type, typename Data_Type>
const Data_Type& Map<Key_Type, Data_Type>::Pair::data() const
{
    return m_data;
}



//	Iterator

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Iterator::Iterator(typename Tree::Iterator&& _it)
    : m_it(_it)
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Iterator::Iterator(const Iterator& _other)
    : m_it(_other.m_it)
{

}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Iterator::operator=(const Iterator& _other)
{
    m_it = _other.m_it;
}



template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Iterator::operator++()
{
    ++m_it;
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Iterator::operator--()
{
    --m_it;
}



template<typename Key_Type, typename Data_Type>
Data_Type& Map<Key_Type, Data_Type>::Iterator::operator*()
{
    return m_it->data();
}

template<typename Key_Type, typename Data_Type>
const Data_Type& Map<Key_Type, Data_Type>::Iterator::operator*() const
{
    return *m_it;
}

template<typename Key_Type, typename Data_Type>
Data_Type* Map<Key_Type, Data_Type>::Iterator::operator->()
{
    return m_it.get_ptr();
}

template<typename Key_Type, typename Data_Type>
const Data_Type* Map<Key_Type, Data_Type>::Iterator::operator->() const
{
    return m_it.get_ptr();
}



template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Iterator::begin_reached() const
{
    return m_it.begin_reached();
}

template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Iterator::end_reached() const
{
    return m_it.end_reached();
}


template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Iterator::is_ok() const
{
    return m_it.is_ok();
}





//	Const_Iterator

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Const_Iterator::Const_Iterator(typename Tree::Const_Iterator&& _it)
    : m_it(_it)
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Const_Iterator::Const_Iterator(const Const_Iterator& _other)
    : m_it(_other.m_it)
{

}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Const_Iterator::operator=(const Const_Iterator& _other)
{
    m_it = _other.m_it;
}



template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Const_Iterator::operator++()
{
    ++m_it;
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::Const_Iterator::operator--()
{
    --m_it;
}



template<typename Key_Type, typename Data_Type>
const Data_Type& Map<Key_Type, Data_Type>::Const_Iterator::operator*() const
{
    return *m_it;
}

template<typename Key_Type, typename Data_Type>
const Data_Type* Map<Key_Type, Data_Type>::Const_Iterator::operator->() const
{
    return m_it.get_ptr();
}



template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Const_Iterator::begin_reached() const
{
    return m_it.begin_reached();
}

template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Const_Iterator::end_reached() const
{
    return m_it.end_reached();
}


template<typename Key_Type, typename Data_Type>
bool Map<Key_Type, Data_Type>::Const_Iterator::is_ok() const
{
    return m_it.is_ok();
}



//	Map

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Map()
{

}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Map(const Map<Key_Type, Data_Type>& _other)
    : m_tree(_other.m_tree)
{

}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::operator=(const Map<Key_Type, Data_Type>& _other)
{
    m_tree = _other.m_tree;
}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::Map(Map<Key_Type, Data_Type>&& _other)
    : m_tree((Tree&&)_other.m_tree)
{

}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::operator=(Map<Key_Type, Data_Type>&& _other)
{
    m_tree = (Tree&&)_other.m_tree;
}

template<typename Key_Type, typename Data_Type>
Map<Key_Type, Data_Type>::~Map()
{
    m_tree.clear();
}



template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::insert(const Key_Type& _key, const Data_Type& _data)
{
    m_tree.insert(Pair(_key, _data));
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::insert(Key_Type&& _key, Data_Type&& _data)
{
    m_tree.insert(Pair((Key_Type&&)_key, (Data_Type&&)_data));
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::insert(Key_Type&& _key, const Data_Type& _data)
{
    m_tree.insert(Pair((Key_Type&&)_key, _data));
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::insert(const Key_Type& _key, Data_Type&& _data)
{
    m_tree.insert(Pair(_key, (Data_Type&&)_data));
}


template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::erase(const Iterator& _where)
{
    m_tree.erase(_where.m_it);
}

template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::erase(const Const_Iterator& _where)
{
    m_tree.erase(_where.m_it);
}


template<typename Key_Type, typename Data_Type>
void Map<Key_Type, Data_Type>::clear()
{
    m_tree.clear();
}



template<typename Key_Type, typename Data_Type>
typename Map<Key_Type, Data_Type>::Iterator Map<Key_Type, Data_Type>::iterator()
{
    return Iterator(m_tree.iterator());
}

template<typename Key_Type, typename Data_Type>
typename Map<Key_Type, Data_Type>::Const_Iterator Map<Key_Type, Data_Type>::iterator() const
{
    return Const_Iterator(m_tree.iterator());
}

#include "Stuff/Stopwatch.h"

template<typename Key_Type, typename Data_Type>
typename Map<Key_Type, Data_Type>::Iterator Map<Key_Type, Data_Type>::find(const Key_Type& _key)
{
    Pair stub;
    stub.m_key = _key;

    return Iterator(m_tree.find(stub));
}

template<typename Key_Type, typename Data_Type>
typename Map<Key_Type, Data_Type>::Const_Iterator Map<Key_Type, Data_Type>::find(const Key_Type& _key) const
{
    Pair stub;
    stub.m_key = _key;

    return Const_Iterator(m_tree.find(stub));
}

































































