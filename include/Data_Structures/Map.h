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
			Key_Type* m_key = nullptr;
			Data_Type* m_data = nullptr;

		public:
			Pair(const Key_Type& _key, const Data_Type& _data);
			Pair(Key_Type&& _key, Data_Type&& _data);
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

	public:
		class Iterator final
		{
		private:
			typename LDS::AVL_Tree<Pair>::Iterator m_it;

		public:


		};

		class Const_Iterator final
		{

		};

	private:
		LDS::AVL_Tree<Pair> m_tree;

	public:


	};

}

#endif // MAP_H
