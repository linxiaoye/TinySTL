#ifndef _SLIST_H_
#define _SLIST_H_

/*
*  Author  @yyehl
*/

#include "Allocator.h"
#include "Iterator.h"



// list是双向链表，slist是单向链表
// 由于slist没有指向prev的指针，所以insert和pop只能选择insert_back和erase_back

namespace TinySTL
{
	/** list_node_base **/
	struct slist_node_base
	{
		slist_node_base* next;
	};

	/** list_node **/
	template<class T>
	struct slist_node : public slist_node_base
	{
		T data;
	};

	// 全局函数，在pre_node节点之后插入new_node节点
	inline slist_node_base* slist_make_link(slist_node_base* pre_node, slist_node_base* new_node)
	{
		new_node->next = pre_node->next;
		pre_node->next = new_node->next;
		return new_node;
	}

	// 全局函数，计算单向链表的大小，只能一个个遍历
	inline size_t slist_count_size(slist_node_base* node)
	{
		size_t ret = 0;
		while (node != 0)
		{
			++ret;
			node = node->next;
		}
		return ret;
	}

	/** slist_iterator_base **/
	struct slist_iterator_base
	{
		typedef size_t                     size_type;          // 不允许有负数
		typedef ptrdiff_t                  difference_type;    // 可以有负数
		typedef forward_iterator_tag       iterator_category;

		slist_node_base* node;

		slist_iterator_base(slist_node_base* x) : node(x) { }

		/* 因为对于迭代器来说，比较操作仅仅是比较两者所指的地址相不相同，与类型无关，所以放在base里面好一些*/
		bool operator == (const slist_iterator_base& x) const { return node == x.node; }
		bool operator != (const slist_iterator_base& x) const { return node != x.node; }
	};

	/** slist_iterator **/
	template<class T, class Ref, class Ptr>      // 加上Ref和Ptr主要是为了const_iterator
	struct slist_iterator : public slist_iterator_base
	{
		typedef slist_iterator<T, T&, T*>                 iterator;
		typedef slist_iterator<T, const T&, const T*>     const_iterator;
		typedef slist_iterator<T, Ref, Ptr>               self;

		typedef T             value_type;
		typedef Ptr           pointer;
		typedef Ref           reference;
		
		slist_iterator(slist_node_base* x) : slist_iterator_base(x) { }
		slist_iterator() : slist_iterator_base(0) { }   // 调用end()时会造成slist_iterator(0)
		slist_iterator(const iterator& x) : slist_iterator_base(x.node) { }

		reference operator * () { return ((slist_node*)node)->data; }
		pointer operator -> () { return &(*this); }   // 标准实现

		self& operator ++ () { node = node->next; return *this; }
		self operator ++ (int) { self tmp = *this; node = node->next; return tmp; }

	};   // end of struct slist_iterator

	/******************* class slist ****************************/
	/* 友元函数定义 */
	template<class T, class Alloc>
	bool operator == (const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs) { return lhs.head == rhs.head; }
	template<class T, class Alloc>
	bool operator != (const slist<T, Alloc>& lhs, const slist<T, Alloc>& rhs) { return lhs.head != rhs.head; }
	template<class T, class Alloc>
	void swap(slist<T, Alloc>& lhs, slist<T, Alloc>& rhs) { lhs.swap(rhs); }

	/* class slist */
	template<class T, class Alloc = allocate<slist_node<T>>>
	class slist
	{
	public:
		bool operator == (const slist& lhs, cosnt slist& rhs);
		bool operator != (const slist& lhs, const slist& rhs);
		void swap(slist& lhs, slist& rhs);
		  
		typedef T                                      value_type;
		typedef slist_iterator<T, T&, T*>              iterator;
		typedef slist_iterator<T, const T&, const T*>  const_iterator;
		typedef T*                                     pointer;
		typedef const T*                               const_pointer;
		typedef T&                                     reference;
		typedef const T&                               const_reference;
		typedef size_t                                 size_type;
		typedef ptrdiff_t                              difference_type;

	private:
		typedef Alloc    node_allocator;

		slist_node_base head;  // 把base当做数据结构只是把这个节点放在头部位置当指示节点用，不存放数据
		                       // 在之后构造的节点，则是构造的slist_node类型，需要存放数据的
	public:
		slist() { head.next = 0; }
		slist(size_type n, const value_type& val);
		template<class InputIterator>
		slist(InputIterator first, InputIterator last);
		slist(const slist& x);
		~slist() { clear(); }

		slist& operator = (const slist& x);

		bool operator == (const slist& x);
		bool operator != (const slist& x);

		size_type size() const;
		bool empty() const;

		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		reference front();
		const_reference front const;

		void swap(slist& x);
		void push_front(const value_type& val);     // 从头部插入元素
		void pop_front();                           // 删除头部那个元素

		//slist没有指向前一个节点的指针，所以无法插在pos之前，只能插在之后
		iterator insert_back(iterator pos, const value_type& val);  //  插入后返回指向新插入元素的迭代器
		iterator insert_back(iterator pos, size_type n, const value_type& val); 
		template<class InputIterator>
		iterator insert_back(iterator pos, InputIterator first, InputIterator last);
		// 同理，slist只能删除后一个元素，不能删除自身
		void erase_back(iterator pos);
		void erase_back(iterator first, size_type n);

		void clear();

	private:     // 工具函数
		static slist_node<T>* create_node(const value_type& val);
		static void destroy_node(slist_node<T>* node);
		template<class InputIterator>
		slist_aux(InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		slist_aux(InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		insert_auc(iterator pos, InputIterator first, InputIterator last, std::false_type);

}  // namespace TinySTL

#include "./Detail/Slist.impl.h"

#endif // !_SLIST_H_

