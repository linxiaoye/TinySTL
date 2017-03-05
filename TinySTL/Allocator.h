#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

/*
*     Author  @linxiaoye
*     Date    2017.2.27
*/

#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>

namespace TinySTL{
	
	/*
	*    空间配置器标准接口 
	*/
	
	template<typename T>
	class allocator {
		public:
			typedef T                   value_type;
			typedef T*                  pointer;
			typedef const T*            const_pointer;
			typedef T&                  reference;
			typedef const T&            const_reference;
			typedef size_t              size_type;
			typedef ptrdiff_t           difference_type;
		
		public:
			// 内存的分配与释放 
			static pointer allocate();
			static pointer allocate(size_type n);
			static void deallocate(pointer ptr);
			static void deallocate(pointer ptr, size_type n);
			
			// 对象的构造与析构 
			static void construct(pointer ptr);
			static void construct(pointer ptr, const_reference value);
			static void destroy(pointer ptr);
			static void destroy(pointer first, pointer last);		
	};
	
	template<typename T>
    auto allocator<T>::allocate() -> pointer {
		return static_cast<pointer>(alloc::allocate(sizeof(T)));
	}
	template<typename T>
	auto allocator<T>::allocate(size_type n) -> pointer {
		if (n == 0) return 0;
		return static_cast<pointer>(alloc::allocate(sizeof(T) * n));
	}
	template<typename T>
	void allocator<T>::deallocate(pointer ptr) {
		alloc::deallocate(static_cast<void* >(ptr), sizeof(T));
	}
	template<typename T>
	void allocator<T>::deallocate(pointer ptr, size_type n) {
		if (n == 0) return;
		alloc::deallocate(static_cast<void* >(ptr), sizeof(T) * n);
	}
	
	
	template<typename T>
	void allocator<T>::construct(pointer ptr) {
		new(ptr)T();
	}
	template<typename T>
	void allocator<T>::construct(pointer ptr, const_reference value) {
		new(ptr)T(value);
	}
	template<typename T>
	void allocator<T>::destroy(pointer ptr) {
		ptr->~T();
	}
	template<typename T>
	void allocator<T>::destroy(pointer first, pointer last) {
		for ( ; first != last; ++first) {
			first->~T();
		}
	}
	
}

#endif
