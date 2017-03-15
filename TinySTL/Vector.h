#ifndef _VECTOR_H_
#define _VECTOR_H_

/*
*  Author  @linxiaoye
*/

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Uninitialized.h"
#include "Algorithm.h"

namespace TinySTL 
{
	template<class T, class Alloc = Allocator<T>>
	class vector 
	{
	private:   
	    /*  vector的数据结构  */       
		T* start;             // 目前已被占用空间的起始位置 
		T* finish;            // 目前已被占用空间的尾元素后一位 
		T* end_of_storage;    // 目前可用空间的尾元素后一位 
		
	public:
		typedef T           value_type;
		typedef T*          pointer;
		typedef T*          iterator;
		typedef const T*    const_iterator;
		typedef T&          reference;
		typedef const T&    const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
	
	public:
		/*   构造、赋值、析构相关函数    */
		vector() : start(0), finish(0), end_of_storage(0) { }
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		vector(const vector& v);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		// vector(vector&& v);          // move操作，再仔细学习一下再写 
		vector& operator = (const vector& v);
		// vector& operator = (const vector&& v);
		~vector();
		
		/*    比较操作相关函数        */
		bool operator == (const vector& v) const;
		bool operator != (const vector& v) const;
		
		/*    容量大小相关函数       */
		different_type size() const { return finish - start; }
		disserent_type capacity() const { return end_of_storage - start; }
		bool empty() const { return start == finish; }
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		void shrink_to_fit();
		
		/*     迭代器相关函数       */
		
		/*     访问元素相关函数     */  
		
		/*     容器修改相关函数     */
		
		/*     容器的空间配置相关函数     */ 
		
		
		
	};  //  end of class vector 

	
}   // namespace TinySTL

#endif   //  _VECTOR_H_
