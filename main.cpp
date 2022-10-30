#include <iostream>

#include <stdlib.h>
#include <ctime>

#include "Data_Structures/Vector.h"

int counter = 0;

struct Test
{
	bool moved = false;

	Test()
	{
		++counter;
	}

	Test(const Test& _other)
	{
		++counter;
//		std::cout << "copy\n";
	}

	Test(Test&& _other)
	{
		_other.moved = true;

//		std::cout << "move\n";
	}

	~Test()
	{
		if(!moved)
			--counter;
	}

};


int main()
{
	srand(time(nullptr));

	while(true)
	{
		unsigned int random_count = (rand() % 10000) + 2;

		std::cout << "random_count: " << random_count << "\n";

		LDS::Vector<Test>* vec = new LDS::Vector<Test>;

		for(unsigned int i=0; i<random_count; ++i)
		{
			bool need_to_copy = rand() % 2 == 0;

			if(need_to_copy)
			{
				Test test;
				vec->push(test);
			}
			else
			{
				vec->push(Test());
			}
		}

		std::cout << "after initialization counter: " << counter << "\n";

		LDS::Vector<Test>::Iterator it = vec->iterator();

		while(!it.end_reached())
		{
			it->moved = it->moved;

			++it;
		}

		while(!it.begin_reached())
		{
			it->moved = it->moved;

			--it;
		}

		unsigned int middle = vec->size() / 2;

		for(unsigned int i=0; i<middle; ++i)
			++it;

		vec->pop(it);


		std::cout << "after deleting element counter: " << counter << "\n";

		delete vec;

		std::cout << "after deleting Vector counter: " << counter << "\n\n";
	}

	return 0;
}
