#ifndef _LIST_H_
#define _LIST_H_

/*
*		Author  @linxiaoye
*/

#include "Allocator.h"
#include "Construct.h"
#include "Uninitialized.h"
#include "Iterator.h"
#include "TypeTraits.h"

#include <type_traits>


namespace TinySTL 
{
	
	template<class T>
	class list;            // ������list�� 
	
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
		template<T>                        // ��ʾֻ�в���ͬΪTʱ�����list���Ƕ�Ӧlist_iterator����Ԫ�� 
		friend class list;                 //  list ���Է��� list_iterator�����г�Ա 
		
		public:
			typedef node<T>* node_ptr;
			node_ptr p;                    // list_iterator�����ݳ�Ա��һ��ָ��node��ָ�� 
		public:
			explicit list_iterator(node_ptr ptr = nullptr) : p(ptr) { }   // ��������ʽת�� 
			
			inline list_iterator& operator ++ ();    // ǰ��++ 
			inline list_iterator operator ++ (int);  // ����++ 
			inline list_iterator& operator -- ();
			inline list_iterator operator -- (int);
			
			T& operator * () { return (*p).data;}
			T* operator -> () { return &(operator*()); }  // ��׼���� 
			

			friend bool operator== <T> (const list_iterator<T>& lhs, const list_iterator<T>& rhs);
			friend bool operator!= <T> (const list_iterator<T>& lhs, const list_iterator<T>& rhs);
	
	};  // end of class list_iteraetor
	
	/*********************[class list]*******************************/
	/***��Ԫ����������***/
	template<class T>
	void swap(list<T>& lhs, list<T>& rhs);
	template<class T>
	bool operator == (const list<T>& lhs, const list<T>& rhs);
	template<class T>
	bool operator != (const list<T>& lhs, const list<T>& rhs);
	
	template<class T>
	class list {
	public:
		friend struct list_iterator<T>;
		friend void swap<T> (list<T>& lhs, list<T>& rhs);
		friend bool operator == <T>(const list<T>& lhs, const list<T>& rhs);
		friend bool operator != <T>(const list<T>& lhs, const list<T>& rhs);
		
	private:
		typedef allocator<node<T>>   	 node_allocator;
		typedef node<T>*              	 node_ptr;
	public:
		typedef T                        value_type;
		typedef list_iterator<T>         iterator;
		typedef list_iterator<const T>   const_iterator;
		typedef T&                       reference;
		typedef const T&                 const_reference;
		typedef size_t                   size_type;
		typedef ptrdiff_t                different_type;
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

		bool empty() { return head == tail; }
		size_type size();
		reference front() { return head->data; }
		reference back() { return tail->prev->data; }
		
		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		
		void push_back(const value_type& val);
		void pop_back();
		void push_front(const value_type& val);
		void pop_front();
		
		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
		
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		
		void clear();
		void swap(list& rhs);
		void splice(iterator pos, list& x);
		void splice(iterator pos, list& x, iterator i);
		void splice(iterator pos, list& x, iterator first, iterator last);
		void remove(const value_type& val);
		template<class Predicate>
		void remove_if(Predicate pred);
		void unique();
		template<class BinaryPredicate>
		void unique(BinaryPredicate binary_pred);
		void merge(list& x);
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
		
		
	};   // end of class list
	
	
}  // namespace TinySTL

#include "./Detail/List.impl.h"

#endif  // _LIST_H_