#include "../Alloc.h"

/*
*        Author  @yyehl
*/

namespace TinySTL {
	
	char* alloc::start_free = 0;  
	char* alloc::end_free = 0; 
	size_t alloc::heap_size = 0;
	
	alloc::obj* alloc::free_list[alloc::ENFreelists::NFREELISTS]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	void* alloc::_allocate(size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) {    // 如果需要分配的空间大于128bytes 
			return malloc(bytes);             // 则直接使用第一级配置器malloc分配 
		}                                     //  若需要分配的空间小于128bytes 
		size_t index = FREELIST_INDEX(bytes); // 则使用第二级配置器分配空间 
		obj* list = free_list[index];
		if (list) {                           // 如果freelist还有空间 
			free_list[index] = list->next;    // 则调整freelist中的指针，使其指向下一个可用的空间 
			return list;   
		}
		else {                                // 如果freelist没有找到可用的空间 
			return refill(ROUND_UP(bytes));   // 则重新填充freelist 
		}
	}
	void alloc::_deallocate(void *ptr, size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) {    // 如果大于128bytes，则使用free直接回收空间 
			free(ptr);
		}
		else {                                // 如果小于128bytes，则回收到freelist中 
			size_t index = FREELIST_INDEX(bytes);
			obj* node = static_cast<obj* >(ptr);
			node->next = free_list[index];
			free_list[index] = node;
		}
	} 
	void* alloc::_reallocate(void* ptr, size_t old_sz, size_t new_sz) {
		_deallocate(ptr, old_sz);
		ptr = _allocate(new_sz);
		return ptr;
	}
	void* alloc::refill(size_t bytes) {       // 当freelist中没有合适bytes的块，就执行refill 
		size_t nobjs = ENObjs::NOBJS;
		// 从内存池里取nobjs个大小为bytes的新区块 
		char* chunk = chunk_alloc(bytes, nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* current_obj = 0, *next_obj = 0;
		
		if (nobjs == 1) {                     // 当取出来的内存仅够一个对象使用 
			return chunk;                     // 那直接返回给客端，也不需要管理多余的取出来的空间（因为并没有多的） 
		}
	 	else {                                // 如果取出来的区块数量大于1，那就需要将剩余的区块放到freelist中管理起来 
	 		my_free_list = free_list + FREELIST_INDEX(bytes);
	 		result = (obj* )(chunk);          // 要返回给客端的1个区块 
	 		*my_free_list = next_obj = (obj* )(chunk + bytes); // 需要放入freelist的区块起始位置 
	 		// 开始管理多余的区块，加入到freelist中 
	 		for (int i = 1;  ; ++i) {
	 			current_obj = next_obj;
	 			next_obj = (obj* )((char* )next_obj + bytes);
	 			if (nobjs - 1 == i) {         // 指针指向最后一个区块 
	 				current_obj->next = 0;
	 				break;
				 }
				 else {
				 	current_obj->next = next_obj;
				 }
			 }
			return result;
		 }
	}
	char* alloc::chunk_alloc(size_t bytes, size_t& nobjs) {  // 注意nobjs的 by-reference的 
		char* result = 0;
		size_t total_bytes = bytes * nobjs;    // 总共需要配置的bytes数量 
		size_t bytes_left = end_free - start_free;  // memory-pool中还剩余的空间 
		
		if (total_bytes <= bytes_left) {       // 如果需要的量少于memory-pool中剩余的量 
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes <= bytes_left) {        // 如果需要的量小于需求的总量但是大于一个区块 
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else {                                 // 如果memory-pool中连一个区块的剩余量都没有了 
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			
			if (bytes_left > 0) {              //  将剩余的那一点点空间收入合适的freelist进行管理，避免浪费 
				obj** my_free_list  = free_list + FREELIST_INDEX(bytes_left);
				((obj* )start_free)->next = *my_free_list;
				*my_free_list = (obj* )start_free;
			}
			
			start_free = (char* )malloc(bytes_to_get);  //使用malloc分配内存 
			
			if (!start_free) {                    // 如果分配失败，则在原来freelist管理的内存中找寻 
				obj** my_free_list = 0, *p = 0;   // 看有没有还尚未使用的区块 
				for (int i = 0; i < EMaxbytes::MAXBYTES; i += EAlign::ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						*my_free_list = p->next;
						start_free = (char* )p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);  // 递归调用自身以更新nobjs 
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nobjs);  // 递归调用自身， 修正nobjs 
		} 
	} 	
}   // namespace TinySTL

