#include "../Alloc.h"

namespace TinySTL {
	
	char* alloc::start_free = 0;  
	char* alloc::end_free = 0; 
	size_t alloc::heap_size = 0;
	
	alloc::obj* alloc::free_list[alloc::ENFreelists::NFREELISTS]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	void* alloc::allocate(size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) {   // 如果需要分配的空间大于128bytes 
			return malloc(bytes);            // 则直接使用第一级配置器malloc分配 
		}                                    //  若需要分配的空间小于128bytes 
		size_t index = FREELIST_INDEX(bytes); // 则使用第二级配置器分配空间 
		obj* list = free_list[index];
		if (list) {  // 如果freelist还有空间 
			free_list[index] = list->next;  // 则调整freelist中的指针，使其指向下一个可用的空间 
			return list;   
		}
		else {   // 如果freelist没有找到可用的空间 
			return refill(ROUND_UP(bytes));   // 则重新填充freelist 
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) { // 如果大于128bytes，则使用free直接回收空间 
			free(ptr);
		}
		else {      // 如果小于128bytes，则回收到freelist中 
			size_t index = FREELIST_INDEX(bytes);
			obj* node = static_cast<obj* >(ptr);
			node->next = free_list[index];
			free_list[index] = node;
		}
	} 
	void* alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz) {
		deallocate(ptr, old_sz);
		ptr = allocate(new_sz);
		return ptr;
	}
	void* alloc::refill(size_t bytes) {
		size_t nobjs = ENObjs::NOBJS;
		// 从内存池里取nobjs个大小为bytes的新区块 
		char* chunk = chunk_alloc(bytes, nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* current_obj = 0, *next_obj = 0;
		 
	}
	
	
	
	
}
