#include <iostream>

#include <stdlib.h>
#include <ctime>

#include "Data_Structures/Vector.h"
#include "Data_Structures/Binary_Heap.h"
#include "Data_Structures/Tree.h"
#include "Data_Structures/AVL_Tree.h"

int main()
{
//	while(true)
	{
		LDS::AVL_Tree<int> tree;

		tree.insert(6);
		tree.insert(3);
		tree.insert(1);
//		tree.insert(10);
//		tree.insert(5);
//		tree.insert(15);

		LDS::AVL_Tree<int>::Iterator it = tree.iterator();

		while(!it.end_reached())
		{
			std::cout << *it << "\n";
			++it;
		}
		std::cout << "\n";

		while(!it.begin_reached())
		{
			std::cout << *it << "\n";
			--it;
		}
		std::cout << "\n";

		while(tree.size() != 1)
		{
			it = tree.iterator();
			tree.erase(it);
		}
		it = tree.iterator();
		tree.erase(it);

		for(int i=1; i<=15; ++i)
			tree.insert(i);

		it = tree.iterator();

		while(*it != 8)
		{
			++it;
		}

		tree.erase(it);

		it = tree.iterator();
		while(!it.end_reached())
		{
			std::cout << *it << "\n";
			++it;
		}
		std::cout << "\n";
	}

	return 0;
}
