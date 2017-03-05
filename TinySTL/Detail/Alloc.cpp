#include "../Alloc.h"

namespace TinySTL {
	
	char* alloc::start_free = 0;  
	char* alloc::end_free = 0; 
	size_t alloc::heap_size = 0;
	
	alloc::obj* alloc::free_list[alloc::ENFreelists::NFREELISTS]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	
	
	
	
}
