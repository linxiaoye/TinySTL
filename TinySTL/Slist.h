#ifndef _SLIST_H_
#define _SLIST_H_

/*
*  Author  @yyehl
*/

#include "Allocator.h"
#include "Iterator.h"



// list��˫������slist�ǵ�������
// ����slistû��ָ��prev��ָ�룬����insert��popֻ��ѡ��insert_back��erase_back

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

	// ȫ�ֺ�������pre_node�ڵ�֮�����new_node�ڵ�
	inline slist_node_base* slist_make_link(slist_node_base* pre_node, slist_node_base* new_node)
	{
		new_node->next = pre_node->next;
		pre_node->next = new_node->next;
		return new_node;
	}

	// ȫ�ֺ��������㵥������Ĵ�С��ֻ��һ��������
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
		typedef size_t                     size_type;          // �������и���
		typedef ptrdiff_t                  difference_type;    // �����и���
		typedef forward_iterator_tag       iterator_category;

		slist_node_base* node;

		slist_iterator_base(slist_node_base* x) : node(x) { }

		/* ��Ϊ���ڵ�������˵���Ƚϲ��������ǱȽ�������ָ�ĵ�ַ�಻��ͬ���������޹أ����Է���base�����һЩ*/
		bool operator == (const slist_iterator_base& x) const { return node == x.node; }
		bool operator != (const slist_iterator_base& x) const { return node != x.node; }
	};

	/** slist_iterator **/
	template<class T, class Ref, class Ptr>      // ����Ref��Ptr��Ҫ��Ϊ��const_iterator
	struct slist_iterator : public slist_iterator_base
	{
		typedef slist_iterator<T, T&, T*>                 iterator;
		typedef slist_iterator<T, const T&, const T*>     const_iterator;
		typedef slist_iterator<T, Ref, Ptr>               self;

		typedef T             value_type;
		typedef Ptr           pointer;
		typedef Ref           reference;
		
		slist_iterator(slist_node_base* x) : slist_iterator_base(x) { }
		slist_iterator() : slist_iterator_base(0) { }   // ����end()ʱ�����slist_iterator(0)
		slist_iterator(const iterator& x) : slist_iterator_base(x.node) { }

		reference operator * () { return ((slist_node*)node)->data; }
		pointer operator -> () { return &(*this); }   // ��׼ʵ��

		self& operator ++ () { node = node->next; return *this; }
		self operator ++ (int) { self tmp = *this; node = node->next; return tmp; }

	};   // end of struct slist_iterator

	/******************* class slist ****************************/
	/* ��Ԫ�������� */
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

		slist_node_base head;  // ��base�������ݽṹֻ�ǰ�����ڵ����ͷ��λ�õ�ָʾ�ڵ��ã����������
		                       // ��֮����Ľڵ㣬���ǹ����slist_node���ͣ���Ҫ������ݵ�
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
		void push_front(const value_type& val);     // ��ͷ������Ԫ��
		void pop_front();                           // ɾ��ͷ���Ǹ�Ԫ��

		//slistû��ָ��ǰһ���ڵ��ָ�룬�����޷�����pos֮ǰ��ֻ�ܲ���֮��
		iterator insert_back(iterator pos, const value_type& val);  //  ����󷵻�ָ���²���Ԫ�صĵ�����
		iterator insert_back(iterator pos, size_type n, const value_type& val); 
		template<class InputIterator>
		iterator insert_back(iterator pos, InputIterator first, InputIterator last);
		// ͬ��slistֻ��ɾ����һ��Ԫ�أ�����ɾ������
		void erase_back(iterator pos);
		void erase_back(iterator first, size_type n);

		void clear();

	private:     // ���ߺ���
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

