#include "..\Vector.h"
#include <iostream>
#include <stdlib.h>

int main()
{
	TinySTL::vector<int> ivec(10);
	// std::cout << ivec[0] << " " << ivec[1] << std::endl;

	
	ivec.push_back(1);
	ivec.push_back(2);
	for (auto i : ivec)  std::cout << i << " ";
	system("pause");
	std::cout << std::endl;   

	system("pause");

	return 0;
}