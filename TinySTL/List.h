#ifndef _LIST_H_
#define _LIST_H_

/*
*		Author  @yyehl
*/

#include "Allocator.h"
#include "Construct.h"
#include "Uninitialized.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Utility.h"
#include "Algorithm.h"

#include <type_traits>


namespace TinySTL 
{
	
	template<class T>
	class list;            // 先声明list类 
	
	/*********************[node class]************************************/
	template <class T>
	struct node 
	{
		T data;
		node* prev;
		node* next;
		list<T>* container;
		
		node(const T& d, node* p, node* n, list<T>* c):
				data(d), prev(p), next(n), container(c) { }
		bool operator == (const node& n)
		{
			return (data == n.data) && (prev == n.prev) && (next == n.next) && (container == n.container);
		}
	};     // end of struct node
	
	/*********************[list_iterator class]*******************************/
	template<class T>
	struct list_iterator;
	template<class T>
	bool operator== (list_iterator<T>& lhs, list_iterator<T>& rhs);
	template<class T>
	bool operator!= (list_iterator<T>& lhs, list_iterator<T>& rhs);
	
	
	template<class T>	
	struct list_iterator: public iterator<bidirectional_iterator_tag, T>
	{
		                                    // 表示只有参数同为T时，这个list才是对应list_iterator的友元类 
		
		public:
			typedef node<T>* node_ptr;
			node_ptr p;                    // list_iterator的数据成员，一个指向node的指针 
		public:
			explicit list_iterator(node_ptr ptr = nullptr) : p(ptr) { }   // 不允许隐式转换 
			  
			inline list_iterator& operator ++ ();    // 前置++ 
			inline list_iterator operator ++ (int);  // 后置++ 
			inline list_iterator& operator -- ();
			inline list_iterator operator -- (int);
			
			T& operator * () { return (*p).data;}
			T* operator -> () { return &(operator*()); }  // 标准做法 
			
	
	};  // end of class list_iteraetor

	/*********************[list_const_iterator class]*******************************/
	template<class T>
	struct list_const_iterator;
	template<class T>
	bool operator== (list_const_iterator<T>& lhs, list_const_iterator<T>& rhs) { return lhs.p == rhs.p; }
	template<class T>
	bool operator!= (list_const_iterator<T>& lhs, list_const_iterator<T>& rhs) { return !(lhs == rhs); }

	template<class T>
	struct list_const_iterator : public list_iterator<T>
	{
		// 表示只有参数同为T时，这个list才是对应list_iterator的友元类 
		
		explicit list_const_iterator(node_ptr ptr = nullptr) : list_iterator<T>::list_iterator(ptr) { }   // 不允许隐式转换 

		inline list_const_iterator& operator ++ () { p = (node_ptr)(*p).next; return *this; }
		inline list_const_iterator operator ++ (int) { node_ptr tmp = p; p = (node_ptr)(*p).next; return list_const_iterator(tmp); }
		inline list_const_iterator& operator -- () { p = (node_ptr)(*p).prev; return *this; }
		inline list_const_iterator operator -- (int) { node_ptr tmp = p; p = (node_ptr)(*p).prev; return list_const_iterator(tmp); }

		const T& operator * () const { return (*p).data; }
		const T* operator -> () const { return &(operator*()); }  // 标准做法 


	};  // end of class list_iteraetor


	
	/*********************[class list]*******************************/
	/***友元函数先声明***/

	
	template<class T>
	class list {
	public:
		friend struct list_iterator<T>;
		friend struct list_const_iterator<T>;
		friend void swap  (list<T>& lhs, list<T>& rhs);
		friend bool operator == <T> (const list<T>& lhs, const list<T>& rhs);
		friend bool operator != <T> (const list<T>& lhs, const list<T>& rhs);
		
	private:
		typedef allocator<node<T>>   	 node_allocator;
		typedef node<T>*              	 node_ptr;
	public:
		typedef T                        value_type;
		typedef list_iterator<T>         iterator;
		typedef list_const_iterator<T>   const_iterator;
		typedef T&                       reference;
		typedef const T&                 const_reference;
		typedef size_t                   size_type;
		typedef ptrdiff_t                difference_type;
	private:
		node_ptr head;
		node_ptr tail;
	public:
		list();
		explicit list(size_type n, const value_type& val = value_type());
		template<class InputIterator>
		list(InputIterator first, InputIterator last);
		list(const list& l);
		list& operator = (const list& rhs);
		~list();

		bool empty() { return head->next == tail; }
		size_type size();
		reference front() { return head->next->data; }
		reference back() { return tail->prev->data; }
		
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		
		void push_back(const value_type& val);
		void pop_back();
		void push_front(const value_type& val);
		void pop_front();
		
		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
			
		iterator erase(iterator pos);   // 返回的iterator指向删除节点后一个位置 
		iterator erase(iterator first, iterator last);
		
		void clear();
		void swap(list& rhs);
		void splice(iterator pos, list& x);
		void splice(iterator pos, list& x, iterator i);
		void splice(iterator pos, list& x, iterator first, iterator last);
		void remove(const value_type& val);    // 将值为val的所有元素移除 
		template<class Predicate>
		void remove_if(Predicate pred);   // 自定义Predicate，如果 pred(*iter)为真，则删除节点 
		void unique(); // 移除相同值的连续元素，只有相同值且连续，才会被移除至只剩一个元素 
		template<class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& x); // 合并两个已经过递增排序的序列 
		template<class Compare>
		void merge(list& x, Compare comp);
		void sort();
		template<class Compare>
		void sort(Compare comp);
		void reserve();
		
		/**   tool functions  **/ 
	private:        
		node_ptr new_node(const value_type& val = value_type());
		template<class InputIterator>
		void ctor_aux(InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		void ctor_aux(InputIterator first, InputIterator last, std::true_type);		
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type);		
		void delete_node(node_ptr ptr);
		void transfer(iterator pos, iterator first, iterator last);
		void init();
		
	};   // end of class list
	
	template<class T>
	void swap (list<T>& lhs, list<T> rhs)
	{
		lhs.swap(rhs);
	}
	template<class T>
	bool operator == (const list<T>& lhs, const list<T>& rhs)
	{
		if (*lhs.begin() != *rhs.begin()) return false;
		list<T>::const_iterator iter1 = lhs.begin();
		list<T>::const_iterator iter2 = rhs.begin();
		for (; iter1 != lhs.end() && iter2 != rhs.end(); ++iter1, ++iter2)
		{
			if (*iter1 != *iter2) return false;
		}
		if (iter1 == lhs.end() && iter2 == rhs.end()) return true;
		else  return false;
	}
	template<class T>
	bool operator != (const list<T>& lhs, const list<T>& rhs)
	{
		return !(lhs == rhs);
	}



}  // namespace TinySTL

#include "./Detail/List.impl.h"

#endif  // _LIST_H_
