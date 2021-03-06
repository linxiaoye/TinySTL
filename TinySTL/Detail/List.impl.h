#ifndef _LIST_IMPL_H_
#define _LIST_IMPL_H_

/*
*	Author  @yyehl
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
	void list<T>::init()
	{
		head = new_node();
		tail = new_node();
		head->next = tail;
		tail->prev = head;
	}
	template<class T>
	list<T>::list()
	{
		init();
	}
	template<class T>
	list<T>::list(size_type n, const value_type& val)
	{
		init();
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
		init();
		insert(list_iterator<T>(tail), first, last);    
	}
	template<class T>
	template<class InputIterator>   // 若是数字 
	void list<T>::ctor_aux(InputIterator first, InputIterator last, std::true_type)
	{
		init();
		while (first--)
			push_back(last);		
	}
	template<class T>
	list<T>::list(const list& l)
	{
		init();
		insert(end(), l.begin(), l.end());
	}
	template<class T>
	auto list<T>::operator = (const list& rhs) -> list&
	{
		if (*this == rhs) return *this;
		clear();
		init();
		insert(end(), rhs.begin(), rhs.end());
		return *this;	
	}	
	template<class T>
	list<T>::~list()
	{
		clear(); 
		head->next = tail;  
		head->prev = nullptr;
		tail->prev = head;
		tail->next = nullptr;
	}

	/*******************************/
	
	template<class T>
	void list<T>::push_back(const value_type& val)
	{
		insert(end(), val);
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
		insert(begin(), val);
	}
	template<class T>
	void list<T>::pop_front()
	{
		erase(begin());
	}
	
	template<class T>
	typename list<T>::iterator list<T>::begin()
	{
		return list_iterator<T>(head->next); 
	}
	template<class T>
	typename list<T>::iterator list<T>::end()
	{
		return list_iterator<T>(tail);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::end() const
	{
		return list_const_iterator<T>(tail);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::begin() const
	{
		return list_const_iterator<T>(head->next);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::cend() const
	{
		return list_const_iterator<T>(tail);
	}
	template<class T>
	typename list<T>::const_iterator list<T>::cbegin() const
	{
		return list_const_iterator<T>(head->next);
	}
	
	template<class T>
	typename list<T>::size_type list<T>::size()
	{
		size_type ret = 0;
		if (head->next == tail)  return ret;
		node_ptr tmp = head;
		tmp = tmp->next;
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
		erase(list_iterator<T>(head->next), list_iterator<T>(tail));
		head->prev = tail->next = nullptr;
		head->next = tail;
		tail->prev = head;
	}
	
	template<class T>
	auto list<T>::insert(iterator pos, const value_type& val) -> iterator
	{
		node_ptr tmp = new_node(val);

		if (pos.p == head->next)
		{
			head->next = tmp;
			tmp->prev = head;
			pos.p->prev = tmp;
			tmp->next = pos.p;
			return list_iterator<T>(tmp);
		}
		else if (pos.p == tail)
		{
			tmp->next = tail;
			tmp->prev = tail->prev;
			tail->prev->next = tmp;
			tail->prev = tmp;
			return list_iterator<T>(tmp);
		}
		else
		{
			tmp->prev = pos.p->prev;
			tmp->next = pos.p;
			pos.p->prev->next = tmp;
			pos.p->prev = tmp;
			return list_iterator<T>(tmp);
		}
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
	void list<T>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type) 
	{
		while (first != last)
		{
			insert(pos, *(first++));	
		}
	}
	
	template<class T>
	auto list<T>::erase(iterator pos) -> iterator
	{
		if (pos == begin())
		{
			node_ptr tmp = pos.p->next;
			tmp->prev = head;
			head->next = tmp;
			delete_node(pos.p);
			return list_iterator<T>(tmp);
		}
		iterator ret = list_iterator<T>(pos.p->next);
		pos.p->prev->next = pos.p->next;
		pos.p->next->prev = pos.p->prev;
		delete_node(pos.p);
		return ret;
	}
	
	template<class T>
	auto list<T>::erase(iterator first, iterator last) -> iterator
	{
		while (first != last)
		{
			erase(first++);
		}
		return last;
	}
		 
	template<class T>
	void list<T>::swap(list& rhs)
	{
		TinySTL::swap(head, rhs.head);  // swap function in Utility.h
		TinySTL::swap(tail, rhs.tail);
	}
	template<class T>    // x 必须不同于*this 
	void list<T>::splice(iterator pos, list& x)
	{
		if (!x.empty())
			transfer(pos, x.begin(), x.end());
	}
	template<class T>  // 将x中的i元素移到pos位置之前，x可以与*this相同 
	void list<T>::splice(iterator pos, list& x, iterator i)
	{
		auto j = i;
		j++;
		if (j == pos || i == pos)  return ;
		transfer(pos, i, j); 
	}
	/* 将[first, last)之间的元素，移到pos之前，x可以与*this指向相同， */
	template<class T>        /* 但是 pos不能位于[first, last)区间之内 */ 
	void list<T>::splice(iterator pos, list& x, iterator first, iterator last)
	{
		if (pos != last)
			transfer(pos, first, last); 
	}
	
	template<class T>
	void list<T>::remove(const value_type& val)
	{
		for (auto first = begin(); first != end(); )
		{
			if (*first == val)  first = erase(first);
			else  ++first;	
		} 
	}
	template<class T>
	template<class Predicate>
	void list<T>::remove_if(Predicate pred)
	{
		for (auto first = begin(); first != end(); )
		{
			if (pred(*first))  first = erase(first);
			else  ++first;	
		} 
	}
	template<class T>
	void list<T>::unique()
	{
		for (auto first = begin(); first != end(); )
		{
			auto iter = first;
			iter++;
			while(*iter == *first)
			{
				iter = erase(iter);
			}
			first = iter;
		}
	}
	template<class T>
	template<class BinaryPredicate>
	void list<T>::unique(BinaryPredicate binary_pred)
	{
		for (auto first = begin(); first != end(); )
		{
			auto iter = first;
			iter++;
			while(binary_pred(*iter, *first))
			{
				iter = erase(iter);
			}
			first = iter;
		}
	}
	template<class T>
	void list<T>::merge(list& x)
	{
		auto iter1 = begin();
		auto iter2 = x.begin();
		while (iter2 != x.end() && iter1 != end())
		{
			if (*iter2 < *iter1)
			{
				auto iter3 = iter2;
				++iter3;
				transfer(iter1, iter2, iter3);
				iter2 = iter3;
			}
			else  ++iter1;
		}
		if (iter2 != x.end())
			transfer(iter1, iter2, x.end());
	}
	template<class T>
	template<class Compare>
	void list<T>::merge(list& x, Compare comp)
	{
		auto iter1 = begin();
		auto iter2 = x.begin();
		while (iter2 != x.end() && iter1 != end())
		{
			if (comp(*iter1, *iter2))
			{
				auto iter3 = iter2;
				++iter3;
				transfer(iter1, iter2, iter3);
				iter2 = iter3;
			}
			else  ++iter1;
		}
		if (iter2 != x.end())
			transfer(iter1, iter2, x.end());
	}
	template<class T>
	void list<T>::sort()
	{
		sort(TinySTL::less<T>());
	}
	template<class T>
	template<class Compare>
	void list<T>::sort(Compare comp)
	{
		if (head->next == tail || head->next->next == tail)  return ;
		list carry;
		list counter[64];
		int fill = 0;
		while (!empty()){
			carry.splice(carry.begin(), *this, begin());
			int i = 0;
			while (i < fill && !counter[i].empty()){
				counter[i].merge(carry, comp);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if (i == fill)
				++fill;
		}
		for (int i = 0; i != fill; ++i){
			counter[i].merge(counter[i - 1], comp);
		}
		swap(counter[fill - 1]);
	}
	
	template<class T>   // 实现思路，遍历list，将元素插入begin()的位置 
	void list<T>::reserve()    // 注意：必须用begin()，因为head是在不断变化的 
	{                          // 所以要调用begin()来刷新迭代器 
		if (head->next == tail || head->next->next == tail) return ; 
		auto iter1 = begin(), iter2 = end();
		iter1++;
		while (iter1 != iter2)
		{
			auto old = iter1++;
			transfer(begin(), old, iter1);
		} 
	}
	
	/*********** friend functions *******************/

	
	/******* tool functions *******/
	template<class T>
	auto list<T>::new_node(const value_type& val) -> node_ptr
	{
		node_ptr ret = node_allocator::allocate();
		node_allocator::construct(ret, node<T>(val, nullptr, nullptr, this));
		return ret;
	} 	
	template<class T>
	void list<T>::delete_node(node_ptr ptr)
	{
		ptr->prev = ptr->next = nullptr;
		node_allocator::destroy(ptr);
		node_allocator::deallocate(ptr);
	}
	template<class T>
	void list<T>::transfer(iterator pos, iterator first, iterator last)
	{
		if (pos != last)  // 如果相等那就不需要任何操作了 
		{
			node_ptr(last.p->prev)->next = pos.p;
			node_ptr(first.p->prev)->next = last.p;
			node_ptr(pos.p->prev)->next = first.p;
			node_ptr tmp = pos.p->prev;
			pos.p->prev = last.p->prev;
			node_ptr(last.p)->prev = first.p->prev;
			first.p->prev = tmp;	
		}
	}
		
}    // namespace TinySTL

#endif     // _LIST_IMPL_H_
