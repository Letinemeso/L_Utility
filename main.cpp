#include <iostream>

#include <stdlib.h>
#include <ctime>

#include "Data_Structures/Vector.h"
#include "Data_Structures/Binary_Heap.h"
#include "Data_Structures/Tree.h"
#include "Data_Structures/AVL_Tree.h"

int main()
{
	while(true)
	{
		LDS::AVL_Tree<int> tree;

		tree.insert(6);
		tree.insert(3);
		tree.insert(1);

		LDS::AVL_Tree<int> copy = tree;

		tree.clear();


		tree.insert(3);
		tree.insert(2);
		tree.insert(1);

		copy = tree;

		auto it = copy.iterator();

	}

	return 0;
}
