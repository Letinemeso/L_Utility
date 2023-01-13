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
			Key_Type* m_key = nullptr;
			Data_Type* m_data = nullptr;

		private:
			Pair(){}

		public:
			Pair(const Key_Type& _key, const Data_Type& _data);
			Pair(Key_Type&& _key, Data_Type&& _data);
			Pair(const Pair& _other);
			Pair(Pair&& _other);
			~Pair();

		public:
			bool operator<(const Pair& _other) const;
			bool operator>(const Pair& _other) const;
			bool operator==(const Pair& _other) const;

		public:
			Key_Type& key();
			const Key_Type& key() const;
			Data_Type& data();
			const Data_Type& data() const;

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

}

#include "../../source/Data_Structures/Map.cpp"

#endif // MAP_H
