#include <iostream>

#include <stdlib.h>
#include <ctime>

#include "Data_Structures/Vector.h"
#include "Data_Structures/Binary_Heap.h"
#include "Data_Structures/Tree.h"

int main()
{
	while(true)
	{
		LDS::Tree<int> tree;

		tree.insert(6);
		tree.insert(3);
		tree.insert(1);
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);

		LDS::Tree<int>::Iterator it = tree.iterator();

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

		while(tree.size() != 0)
		{
			it = tree.iterator();
			tree.erase(it);
		}

		tree.insert(6);
		tree.insert(3);
		tree.insert(1);
		tree.insert(10);
		tree.insert(5);
		tree.insert(15);

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
