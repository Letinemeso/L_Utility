#include <iostream>

#include "Data_Structures/Vector.h"


struct test
{
	test() {}

	test(const test& _other)
	{
		std::cout << "copy\n";
	}

	test(test&& _other)
	{
		std::cout << "move\n";
	}

};


int main()
{
	LDS::Vector<test> vec;

	test t;

	vec.push(t);

	vec.push(test());

	return 0;
}
