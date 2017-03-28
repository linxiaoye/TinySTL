#ifndef _DEQUE_H_
#define _DEQUE_H_

/*
*  Author  @linxiaoye
*/


#include "Allocator.h"
#include "Iterator.h"


namespace TinySTL
{
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	class deque;
	
	inline size_t _dq_buf_sz(size_t n, size_t sz)
	{
		return n != 0 ? n : (sz < 512 ? size_t(512/sz) : size_t(1));	
	} 
	
	template<class T, class Ref, class Ptr, size_t Buf_sz = 0>	 // Buf_sz指的是一段缓冲区装几个T元素 
	class dq_iter
	{
	public:
		friend class deque<T>;
		
		typedef dq_iter<T, T&, T*, Buf_sz>                 iterator;
		typedef dq_iter<T, const T&, const T*, Buf_sz>     const_iterator;
		
		/* buffer_size 函数得到的是 一段缓存区内装几个元素 T */ 
		static size_t buffer_size() { return _dq_buf_sz(Buf_sz, sizeof(T)); }
		
		// 由于未继承TinySTL::iterator，所以需要撰写5个迭代器的相应型别
		typedef random_access_iterator_tag    iterator_category;
		typedef T                             value_type;
		typedef Ptr                           pointer;
		typedef Ref                           reference;
		typedef size_t                        size_type;
		typedef ptrdiff_t                     difference_type;
		typedef T**                           map_pointer;
		
		typedef dq_iter                       self; 
		
	private:	
		T* cur;   // 当前迭代器所指的缓冲行的现行元素
		T* first; // 当前迭代器所指的缓冲行的头
		T* last;  // 当前迭代器所指的缓冲行的尾  
		map_pointer node;  // 指向管控中心 , 也就是一段连续的存有每段缓存区的指针的所谓的map 
	
	public:
		/* 构造函数*/ 
		dq_iter(T* x, map_pointer y) 
			: cur(x), first(*y), last(*y + buffer_size()), node(y) { }
		dq_iter() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) { }
		dq_iter(const dq_iter& x) 
			: cur(x.cur), first(x.first), last(x.last), node(x.node) { }
		
		/* 重载操作符 */
		reference operator * () const { return *cur; }
		difference_type operator - (const self& x) const;
		self& operator ++ ();
		self operator ++ (int);
		self& operator -- ();
		self operator -- (int);
		self& operator += (difference_type n);
		self& operator -= (difference_type n);
		self operator + (difference_type n) const ;
		self operator - (difference_type n) const ;
		reference operator [] (difference_type n) const { return *(*this + n); }
		
		bool operator == (const self& x) const { return cur == x.cur; }
		bool operator != (const self& x) const { return !(*this == x); }
		bool operator < (const self& x) const { return (*this - cur) > 0 ? false : true ;}
		
	private:
		void set_node(map_pointer new_node);
	}; 
	
	
	
	
	
	
	
	
	
	
}


#include "./Detail/Deque.impl.h"


#endif // _DEQUE_H_
