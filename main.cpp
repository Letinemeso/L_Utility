#include <iostream>

#include <stdlib.h>
#include <ctime>

#include "Data_Structures/Vector.h"
#include "Data_Structures/Binary_Heap.h"


int main()
{
	LDS::Binary_Heap<unsigned int> heap;

	LDS::Vector<unsigned int> raw;
	raw.push(1);
	raw.push(2);
	raw.push(4);
	raw.push(5);
	raw.push(6);
	raw.push(8);
	raw.push(9);
	raw.push(10);
	raw.push(11);
	raw.push(16);

	heap.build_from_raw(raw);

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";

	heap.push(20);

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";

	heap.push(13);

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";

	heap.push(99);

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";

	std::cout << "\n\n";

	heap.sort();

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";

	heap.heapify();

	for(unsigned int i=0; i<heap.size(); ++i)
		std::cout << heap.array()[i] << " ";
	std::cout << "\n";


	raw = (LDS::Vector<unsigned int>&&)heap.array();


	return 0;
}
