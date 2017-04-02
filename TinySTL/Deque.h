#ifndef _DEQUE_H_
#define _DEQUE_H_

/*
*  Author  @yyehl
*/


#include "Allocator.h"
#include "Iterator.h"
#include "Uninitialized.h"


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
		~dq_iter() { cur = first = last = node = nullptr;  }
		
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

	/*********************** class deque  *******************************/
	/*友元函数先声明*/
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	bool operator == (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs);
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	bool operator != (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs);
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	void swap (deque<T, Alloc, Buf_sz>& lhs, deque<T, Alloc, Buf_sz>& rhs);


	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	class deque
	{
	public:  // 友元
		friend class dq_iter<T, T&, T*, Buf_sz>;

		friend bool operator == (const deque& lhs, const deque& rhs);
		friend bool operator != (const deque& lhs, const deque& rhs);
		friend void swap(deque& lhs, deque& rhs);
	public:
		typedef T          value_type;
		typedef T*         pointer;
		typedef T&         reference;
		typedef const T&   const_reference;
		typedef size_t     size_type;
		typedef ptrdiff_t  difference_type;

		typedef dq_iter<T, T&, T*, Buf_sz>   iterator;

	private:
		typedef pointer*   map_pointer;
		typedef Alloc      data_allocator;
		typedef allocator<pointer> map_allocator;
	private:
		iterator start;   // 指向起始位置
		iterator finish;  // 指向末尾位置
		map_pointer map;  // 指向所谓的map
		size_type map_size; // map的长度，也就是有几段缓存区

	public:
		deque();
		explicit deque(size_type n, const value_type& val = value_type());
		template<class InputIterator>
		deque(InputIterator first, InputIterator last);
		deque(const deque& x);
		~deque();

		deque& operator = (const deque&);
		deque& operator = (deque&&);

	public:
		iterator begin() { return start; }  //start本身就是迭代器
		iterator end() { return finish; }

		reference operator [] (difference_type n) { return start[n]; }
		const_reference operator [] (difference_type n) const { return start[n]; }

		reference front() { return *start; }
		const_reference front() const { return *start; }
		reference back() { return *(finish - 1); }
		const_reference back() const { return *(finish - 1); }
		size_type size() const { return finish - start; }
		bool empty() const { return start == finish; }
		size_type max_size() { return size_type(-1); }

		void push_back(const value_type& val);
		void push_front(const value_type& val);
		void pop_back();
		void pop_front();
		void swap(deque& x);
		void clear(); //clear之后还会剩下一段缓存区，map还没有被析构和释放

		bool operator == (const deque& x) const;
		bool operator != (const deque& x) const;

	private:
		void create_map_and_nodes(size_type num_elem);
		size_type initial_map_size() { return 8; }
		void fill_initialize(size_type n, const value_type& val);
		template<class InputIterator>
		void deque_aux(InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		void deque_aux(InputIterator first, InpurIterator last, std::false_type);


	};  // end of class deque
	
}   // namespace TinySTL

#include "./Detail/Deque.impl.h"

#endif // _DEQUE_H_
