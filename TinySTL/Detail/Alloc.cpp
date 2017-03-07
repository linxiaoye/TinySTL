#include "../Alloc.h"

namespace TinySTL {
	
	char* alloc::start_free = 0;  
	char* alloc::end_free = 0; 
	size_t alloc::heap_size = 0;
	
	alloc::obj* alloc::free_list[alloc::ENFreelists::NFREELISTS]
		= { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	
	void* alloc::allocate(size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) {   // �����Ҫ����Ŀռ����128bytes 
			return malloc(bytes);            // ��ֱ��ʹ�õ�һ��������malloc���� 
		}                                    //  ����Ҫ����Ŀռ�С��128bytes 
		size_t index = FREELIST_INDEX(bytes); // ��ʹ�õڶ�������������ռ� 
		obj* list = free_list[index];
		if (list) {  // ���freelist���пռ� 
			free_list[index] = list->next;  // �����freelist�е�ָ�룬ʹ��ָ����һ�����õĿռ� 
			return list;   
		}
		else {   // ���freelistû���ҵ����õĿռ� 
			return refill(ROUND_UP(bytes));   // ���������freelist 
		}
	}
	void alloc::deallocate(void *ptr, size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) { // �������128bytes����ʹ��freeֱ�ӻ��տռ� 
			free(ptr);
		}
		else {      // ���С��128bytes������յ�freelist�� 
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
		// ���ڴ����ȡnobjs����СΪbytes�������� 
		char* chunk = chunk_alloc(bytes, nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* current_obj = 0, *next_obj = 0;
		 
	}
	
	
	
	
}
