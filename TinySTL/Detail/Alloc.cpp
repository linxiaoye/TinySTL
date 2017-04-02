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
		if (bytes > EMaxbytes::MAXBYTES) {    // �����Ҫ����Ŀռ����128bytes 
			return malloc(bytes);             // ��ֱ��ʹ�õ�һ��������malloc���� 
		}                                     //  ����Ҫ����Ŀռ�С��128bytes 
		size_t index = FREELIST_INDEX(bytes); // ��ʹ�õڶ�������������ռ� 
		obj* list = free_list[index];
		if (list) {                           // ���freelist���пռ� 
			free_list[index] = list->next;    // �����freelist�е�ָ�룬ʹ��ָ����һ�����õĿռ� 
			return list;   
		}
		else {                                // ���freelistû���ҵ����õĿռ� 
			return refill(ROUND_UP(bytes));   // ���������freelist 
		}
	}
	void alloc::_deallocate(void *ptr, size_t bytes) {
		if (bytes > EMaxbytes::MAXBYTES) {    // �������128bytes����ʹ��freeֱ�ӻ��տռ� 
			free(ptr);
		}
		else {                                // ���С��128bytes������յ�freelist�� 
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
	void* alloc::refill(size_t bytes) {       // ��freelist��û�к���bytes�Ŀ飬��ִ��refill 
		size_t nobjs = ENObjs::NOBJS;
		// ���ڴ����ȡnobjs����СΪbytes�������� 
		char* chunk = chunk_alloc(bytes, nobjs);
		obj** my_free_list = 0;
		obj* result = 0;
		obj* current_obj = 0, *next_obj = 0;
		
		if (nobjs == 1) {                     // ��ȡ�������ڴ����һ������ʹ�� 
			return chunk;                     // ��ֱ�ӷ��ظ��Ͷˣ�Ҳ����Ҫ��������ȡ�����Ŀռ䣨��Ϊ��û�ж�ģ� 
		}
	 	else {                                // ���ȡ������������������1���Ǿ���Ҫ��ʣ�������ŵ�freelist�й������� 
	 		my_free_list = free_list + FREELIST_INDEX(bytes);
	 		result = (obj* )(chunk);          // Ҫ���ظ��Ͷ˵�1������ 
	 		*my_free_list = next_obj = (obj* )(chunk + bytes); // ��Ҫ����freelist��������ʼλ�� 
	 		// ��ʼ�����������飬���뵽freelist�� 
	 		for (int i = 1;  ; ++i) {
	 			current_obj = next_obj;
	 			next_obj = (obj* )((char* )next_obj + bytes);
	 			if (nobjs - 1 == i) {         // ָ��ָ�����һ������ 
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
	char* alloc::chunk_alloc(size_t bytes, size_t& nobjs) {  // ע��nobjs�� by-reference�� 
		char* result = 0;
		size_t total_bytes = bytes * nobjs;    // �ܹ���Ҫ���õ�bytes���� 
		size_t bytes_left = end_free - start_free;  // memory-pool�л�ʣ��Ŀռ� 
		
		if (total_bytes <= bytes_left) {       // �����Ҫ��������memory-pool��ʣ����� 
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else if (bytes <= bytes_left) {        // �����Ҫ����С��������������Ǵ���һ������ 
			nobjs = bytes_left / bytes;
			total_bytes = nobjs * bytes;
			result = start_free;
			start_free = start_free + total_bytes;
			return result;
		}
		else {                                 // ���memory-pool����һ�������ʣ������û���� 
			size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			
			if (bytes_left > 0) {              //  ��ʣ�����һ���ռ�������ʵ�freelist���й��������˷� 
				obj** my_free_list  = free_list + FREELIST_INDEX(bytes_left);
				((obj* )start_free)->next = *my_free_list;
				*my_free_list = (obj* )start_free;
			}
			
			start_free = (char* )malloc(bytes_to_get);  //ʹ��malloc�����ڴ� 
			
			if (!start_free) {                    // �������ʧ�ܣ�����ԭ��freelist������ڴ�����Ѱ 
				obj** my_free_list = 0, *p = 0;   // ����û�л���δʹ�õ����� 
				for (int i = 0; i < EMaxbytes::MAXBYTES; i += EAlign::ALIGN) {
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					if (p != 0) {
						*my_free_list = p->next;
						start_free = (char* )p;
						end_free = start_free + i;
						return chunk_alloc(bytes, nobjs);  // �ݹ���������Ը���nobjs 
					}
				}
				end_free = 0;
			}
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return chunk_alloc(bytes, nobjs);  // �ݹ�������� ����nobjs 
		} 
	} 	
}   // namespace TinySTL

