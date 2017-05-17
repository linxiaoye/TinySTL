

#include "Test\Vector_Test.h"
#include "Test\Algorithm_Test.h"
#include "Test\List_Test.h"

#include <vector>
#include <iostream>
#include <list>
#include <iostream>

int main()
{
	//TinySTL::VectorTest::testAllCases();
	//TinySTL::testAll_algorithm_Cases();
	TinySTL::ListTest::testAllCases();

	

	std::cout << " Seccess! " << std::endl;
	system("pause");
	return 0;
}