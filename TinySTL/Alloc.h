#ifndef _ALLOC_H_
#define _ALLOC_H_

/*
*        Author  @linxiaoye
*/

#include <cstdlib>

namespace TinySTL {
	
/*
*     �μ��ռ��������������ڸ�Allocator�ṩ���ֽӿ� 
*     �ڲ�ʹ�� 
*     
*/
	class alloc {
	private:
		enum EAlign { ALIGN = 8 }; //С��������ϵ��߽� 
		enum EMaxbytes { MAXBYTES = 128 };  //С��������Ͻ磬����MAXBYTES��������malloc���� 
		enum ENFreelists { NFREELISTS = (EMaxbytes::MAXBYTES / EAlign::ALIGN) }; // free-lists�ĸ��� 
		enum ENObjs { NOBJS = 20 }; // ÿ�����ӵĽڵ��� 
	private: 
		// free-lists�Ľڵ㹹�죬ʹ��union����ָ��Ķ��⿪�� 
		union obj {
			union obj * next;
			char client[1];
		}; 
	private:
		static char* start_free; // �ڴ����ʼλ�ã�ֻ��chunk_alloc()�б仯
		static char* end_free; // �ڴ�ؽ���λ�ã�ֻ��chunk_aoolc()�б仯 
		static size_t heap_size; 
		
		
		// 16��freelists 
		static obj* free_list[ENFreelists::NFREELISTS];
		// ��������Ĵ�С������ʹ�õ�n��free-list��n��0��ʼ
		static size_t FREELIST_INDEX(size_t bytes) {
			return ((((bytes) + EAlign::ALIGN - 1) / EAlign::ALIGN) - 1);
		}
		// ��bytes�ϵ���8�ı���
		static size_t ROUND_UP(size_t bytes) {
			return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
		} 
		// ����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free-list
		static void* refill(size_t n);
		// ����һ���ռ䣬������nobjs����СΪsize������
		// �������nobjs�������������㣬nobjs���ܻή��
		static char* chunk_alloc(size_t size, size_t &nobjs); 
	
	public:
		static void* allocate(size_t bytes);
		static void deallocate(void* ptr, size_t bytes);
		static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
	};  	
}  // namespace TinySTL

#endif   // _ALLOC_H_
