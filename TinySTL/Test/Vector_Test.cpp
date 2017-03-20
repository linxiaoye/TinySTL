

#include <iostream>
#include "../Vector.h"
#include "../Alloc.h"
#include "../Allocator.h"
#include <memory>


using namespace TinySTL;

int main()
{
	allocator<int> alloc;
	int* ptr = alloc.allocate();
	
	return 0;
}
