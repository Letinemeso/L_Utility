#ifndef MAP_H
#define MAP_H

#include "AVL_Tree.h"


namespace LDS
{

    template<typename Key_Type, typename Data_Type>
    class Map final
    {
    public:
        class Pair final
        {
        private:
            friend class Map;

        private:
            Key_Type m_key;
            Data_Type m_data;

        private:
            Pair(){}

        public:
            inline Pair(const Key_Type& _key, const Data_Type& _data);
            inline Pair(Key_Type&& _key, Data_Type&& _data);
            inline Pair(const Pair& _other);
            inline Pair(Pair&& _other);
            inline void operator=(const Pair& _other);
            inline void operator=(Pair&& _other);
            inline ~Pair();

        public:
            inline bool operator<(const Pair& _other) const;
            inline bool operator>(const Pair& _other) const;
            inline bool operator==(const Pair& _other) const;

        public:
            inline Key_Type& key();
            inline const Key_Type& key() const;
            inline Data_Type& data();
            inline const Data_Type& data() const;

        };

    private:
        typedef typename LDS::AVL_Tree<Pair> Tree;

    private:
        Tree m_tree;

    public:
        class Iterator final
        {
        private:
            friend class Map;

        private:
            typename Tree::Iterator m_it;

        private:
            Iterator(typename Tree::Iterator&& _it);

        public:
            Iterator(){}
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

            inline const Key_Type& key() const { return m_it->key(); }

        public:
            bool begin_reached() const;
            bool end_reached() const;

            bool is_ok() const;

        };

        class Const_Iterator final
        {
        private:
            friend class Map;

        private:
            typename Tree::Const_Iterator m_it;

        private:
            Const_Iterator(typename Tree::Const_Iterator&& _it);

        public:
            Const_Iterator(const Const_Iterator& _other);
            void operator=(const Const_Iterator& _other);

        public:
            void operator++();
            void operator--();

        public:
            const Data_Type& operator*() const;
            const Data_Type* operator->() const;

            inline const Key_Type& key() const { return m_it->key(); }

        public:
            bool begin_reached() const;
            bool end_reached() const;

            bool is_ok() const;

        };

    public:
        Map();
        Map(const Map<Key_Type, Data_Type>& _other);
        void operator=(const Map<Key_Type, Data_Type>& _other);
        Map(Map<Key_Type, Data_Type>&& _other);
        void operator=(Map<Key_Type, Data_Type>&& _other);
        ~Map();

    public:
        void insert(const Key_Type& _key, const Data_Type& _data);
        void insert(Key_Type&& _key, Data_Type&& _data);
        void insert(Key_Type&& _key, const Data_Type& _data);
        void insert(const Key_Type& _key, Data_Type&& _data);

        void erase(const Iterator& _where);
        void erase(const Const_Iterator& _where);

        void clear();

    public:
        Iterator iterator();
        Const_Iterator iterator() const;

        Iterator find(const Key_Type& _key);
        Const_Iterator find(const Key_Type& _key) const;

    };

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
    return m_it->data();
}

template<typename Key_Type, typename Data_Type>
Data_Type* Map<Key_Type, Data_Type>::Iterator::operator->()
{
    return &m_it->data();
}

template<typename Key_Type, typename Data_Type>
const Data_Type* Map<Key_Type, Data_Type>::Iterator::operator->() const
{
    return &m_it->data();
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
    return m_it->data();
}

template<typename Key_Type, typename Data_Type>
const Data_Type* Map<Key_Type, Data_Type>::Const_Iterator::operator->() const
{
    return &m_it->data();
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

}



#endif // MAP_H
