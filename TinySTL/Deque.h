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
	
	template<class T, class Ref, class Ptr, size_t Buf_sz = 0>	 // Buf_szָ����һ�λ�����װ����TԪ�� 
	class dq_iter
	{
	public:
		friend class deque<T>;
		
		typedef dq_iter<T, T&, T*, Buf_sz>                 iterator;
		typedef dq_iter<T, const T&, const T*, Buf_sz>     const_iterator;
		
		/* buffer_size �����õ����� һ�λ�������װ����Ԫ�� T */ 
		static size_t buffer_size() { return _dq_buf_sz(Buf_sz, sizeof(T)); }
		
		// ����δ�̳�TinySTL::iterator��������Ҫ׫д5������������Ӧ�ͱ�
		typedef random_access_iterator_tag    iterator_category;
		typedef T                             value_type;
		typedef Ptr                           pointer;
		typedef Ref                           reference;
		typedef size_t                        size_type;
		typedef ptrdiff_t                     difference_type;
		typedef T**                           map_pointer;
		
		typedef dq_iter                       self; 
		
	private:	
		T* cur;   // ��ǰ��������ָ�Ļ����е�����Ԫ��
		T* first; // ��ǰ��������ָ�Ļ����е�ͷ
		T* last;  // ��ǰ��������ָ�Ļ����е�β  
		map_pointer node;  // ָ��ܿ����� , Ҳ����һ�������Ĵ���ÿ�λ�������ָ�����ν��map 
	
	public:
		/* ���캯��*/ 
		dq_iter(T* x, map_pointer y) 
			: cur(x), first(*y), last(*y + buffer_size()), node(y) { }
		dq_iter() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) { }
		dq_iter(const dq_iter& x) 
			: cur(x.cur), first(x.first), last(x.last), node(x.node) { }
		~dq_iter() { cur = first = last = node = nullptr;  }
		
		/* ���ز����� */
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
	/*��Ԫ����������*/
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	bool operator == (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs);
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	bool operator != (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs);
	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	void swap (deque<T, Alloc, Buf_sz>& lhs, deque<T, Alloc, Buf_sz>& rhs);


	template<class T, class Alloc = allocator<T>, size_t Buf_sz = 0>
	class deque
	{
	public:  // ��Ԫ
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
		iterator start;   // ָ����ʼλ��
		iterator finish;  // ָ��ĩβλ��
		map_pointer map;  // ָ����ν��map
		size_type map_size; // map�ĳ��ȣ�Ҳ�����м��λ�����

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
		iterator begin() { return start; }  //start������ǵ�����
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
		void clear(); //clear֮�󻹻�ʣ��һ�λ�������map��û�б��������ͷ�

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
