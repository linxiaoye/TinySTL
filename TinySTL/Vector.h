#ifndef _VECTOR_H_
#define _VECTOR_H_

/*
*  Author  @linxiaoye
*/
#include <type_traits>

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Uninitialized.h"
#include "Algorithm.h"

namespace TinySTL 
{
	template<class T, class Alloc = allocator<T>>
	class vector 
	{
	private:   
	    /*  vector的数据结构  */       
		T* _start;             // 目前已被占用空间的起始位置 
		T* _finish;            // 目前已被占用空间的尾元素后一位 
		T* _end_of_storage;    // 目前可用空间的尾元素后一位 
		
	public:
		typedef T           value_type;
		typedef T*          pointer;
		typedef T*          iterator;
		typedef const T*    const_iterator;
		typedef T&          reference;
		typedef const T&    const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		
		typedef Alloc       dataAllocator;
	
	public:
		/*   构造、赋值、析构相关函数    */
		vector() : _start(0), _finish(0), _end_of_storage(0) { }
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
		difference_type size() const { return _finish - _start; }
		difference_type capacity() const { return _end_of_storage - _start; }
		bool empty() const { return _start == _finish; }
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		void shrink_to_fit();
		
		/*     迭代器相关函数       */
		iterator begin() { return _start; }
		const_iterator begin() const { return _start; }
		const_iterator cbegin() const { return _start; }
		iterator end() { return _finish; }
		const_iterator end() const { return _finish; }
		const_iterator cend() const { return _finish; }
		// reverse_iterator rbegin();
		// const_reverse_iterator crbrgin();
		// reverse_iterator rend();
		// const_reverse_iterator();
			
		/*     访问元素相关函数     */  
		reference front() { return *(begin()); }
		reference back() { return *(end() - 1); }
		reference operator [] (const difference_type i) { return *(begin() + i); }
		const_reference operator [] (const difference_type i) const { return *(cbegin() + i); }
		
		/*     容器修改相关函数     */
		void push_back(const value_type& val);
		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, const size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
		void pop_back();
		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void swap(vector& v);
		
		/*     容器的空间配置相关函数     */ 
	public:
		template<class T1, class Alloc_>
		friend bool operator == (const vector<T1, Alloc_>& v1, const vector<T1, Alloc_>& v2);
		template<class T1, class Alloc_>
		friend bool operator != (const vector<T1, Alloc_>& v1, const vector<T1, Alloc_>& v2);
	
	private:
		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, _false_type);  
		template<class InputIterator>  
		void vector_aux(InputIterator size, InputIterator val, _true_type);

		void allocate_and_fill_n(const size_type size, const value_type& val);
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		void destroy_and_deallocate_all();
		
		
		
		
	};  //  end of class vector 

	
}   // namespace TinySTL

#include "Detail\Vector.impl.h"          // 具体实现 

#endif   //  _VECTOR_H_
