#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_

/*
*	Author  @linxiaoye
*/

namespace TinySTL 
{	
	/*********************[list_iterator class]*******************************/	
	template<class T>
	auto list_iterator<T>::operator ++ () -> list_iterator&
	{
		p = (node_ptr)(*p).next;
		return *this;
	}
	template<class T>
	auto list_iterator<T>::operator ++ (int) -> list_iterator
	{
		list_iterator tmp = *this;
		++(*this);
		return tmp;
	}
	template<class T>
	auto list_iterator<T>::operator -- () -> list_iterator&
	{
		p = (node_ptr)(*p).prev;
		return *this;
	}
	template<class T>
	auto list_iterator<T>::operator --(int) -> list_iterator
	{
		list_iterator<T> tmp = *this;
		--(*this);
		return tmp;
	}
	template<class T>
	bool operator== (list_iterator<T>& lhs, list_iterator<T>& rhs)
	{
		return lhs.p == rhs.p;
	}
	template<class T>
	bool operator!= (list_iterator<T>& lhs, list_iterator<T>& rhs)
	{
		return lhs.p != rhs.p;
	}
	
	/*********************[class list]*******************************/	
	
	/******构造、复制、析构等函数*******/
	
	template<class T>
	list<T>::list()
	{
		head = new_node();
		tail = head;
	}
	template<class T>
	list<T>::list(size_type n, const value_type& val)
	{
		head = new_node();
		tail = head;
		while (n--)
			push_back(val);
	}
	template<class T>
	template<class InputIterator>
	list<T>::list(InputIterator first, InputIterator last)      // 需要判断InputIterator是不是数字 
	{
		ctor_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T>
	template<class InputIterator>   // 若不是数字 
	void list<T>::ctor_aux(InputIterator first, InputIterator last, std::false_type)
	{
		head = new_node();    /////////////////////////////////
	}
	template<class T>
	template<class InputIterator>   // 若是数字 
	void list<T>::ctor_aux(InputIterator first, InputIterator last, std::true_type)
	{
		head = new_node();
		tail = head;
		while (first--)
			push_back(last);		
	}
	
	/*******************************/
	
	
	template<class T>
	void list<T>::push_back(const value_type& val)
	{
		insert(list_iterator<T>(tail), val);
	}
	template<class T>
	void list<T>::pop_back()
	{
		node_ptr tmp = tail->prev;
		erase(list_iterator<T>(tmp));
	}
	template<class T>
	void list<T>::push_front(const value_type& val)
	{
		insert(list_iterator<T>(head), val);
	}
	template<class T>
	void list<T>::pop_front()
	{
		erase(list_iterator<T>(head));
	}
	
	template<class T>
	typename list<T>::iterator list<T>::begin()
	{
		return list_iterator<T>(head); 
	}
	template<class T>
	typename list<T>::iterator list<T>::end()
	{
		return list_iterator<T>(tail);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::begin() const 
	{
		return list_iterator<T>(head);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::end() const
	{
		return list_iterator<T>(tail);
	}
	
	template<class T>
	typename list<T>::size_type list<T>::size()
	{
		size_type ret = 0;
		if (head == tail)  return ret;
		node_ptr tmp = head;
		while(tmp != tail)
		{
			++ret;
			tmp = tmp->next;
		}
		return ret;
	}
	
	template<class T>
	void list<T>::clear()
	{
		erase(list_iterator<T>(head), list_iterator<T>(tail));
	}
	
	template<class T>
	auto list<T>::insert(iterator pos, const value_type& val) -> iterator
	{
		node_ptr tmp = new_node(val);
		tmp->prev = pos.p->prev;
		tmp->next = pos.p;
		pos.p->prev->next = tmp;
		pos.p->prev = tmp;
		return list_iterator<T>(tmp);
	}
	template<class T>
	void list<T>::insert(iterator pos, size_type n, const value_type& val)
	{
		while (n--)
		{
			insert(pos, val);
		}
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert(iterator pos, InputIterator first, InputIterator last)
	{
		insert_aux(pos, first, last, std::is_integral<InputIterator>::type());
	}
	template<class T>
	template<class InputIterator>
	void list<T>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type)
	{
		while (first--)
		{
			insert(pos, last);	
		}	
	}	
	template<class T>
	template<class InputIterator>
	void list<T::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type)
	{
		while (first != last)
		{
			insert(pos, *(first++));	
		}
	}
	
	
		 
	
	
	
	
	/******* tool functions *******/
	template<class T>
	auto list<T>::new_node(const value_type& val) -> node_ptr
	{
		node_ptr ret = node_allocator::allocate();
		node_allocator::construct(ret, node<T>(val, nullptr, nullptr, this));
		return ret;
	} 	

	
	
	
	
	
	
}    // namespace TinySTL

#endif     // _LIST_IMPL_H_
