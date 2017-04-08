#ifndef _SLIST_IMPL_H_
#define _SLIST_IMPL_H_

/*
*   Author  @yyehl
*/

namespace TinySTL
{

	template<class T, class Alloc>
	slist<T, Alloc>::slist(size_type n, const value_type& val)
	{
		slist_node<T>* ptr; 
		while (n--)
		{
			ptr = create_node(val);
			slist_make_link(&head, ptr);
		}
	}
	template<class T, class Alloc>
	template<class InputIterator>
	slist<T, Alloc>::slist(InputIterator first, InputIterator last)
	{
		slist_aux(first, last, std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	template<class InputIterator>
	slist<T, Alloc>::slist_aux(InputIterator first, InputIterator last, std::true_type)
	{
		slist(size_type(first), last);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	slist<T, Alloc>::slist_aux(InputIterator first, InputIterator last, std::false_type)
	{
		slist_node<T>* ptr;
		for (auto cur = first; cur != last; ++cur)
		{
			ptr = create_node(value_type(*cur));
			slist_make_link(&head, ptr);
		}
	}
	template<class T, class Alloc>
	slist<T, Alloc>::slist(const slist& x)
	{
		slist(x.begin(), x.end());
	}

	template<class T, class Alloc>
	auto slist<T, Alloc>::operator = (const slist& x) -> slist&
	{
		clear();
		head = x.head;
	}

	template<class T, class Alloc>
	bool slist<T, Alloc>::operator == (const slist& x) const
	{
		return head == x.head;
	}
	template<class T, class Alloc>
	bool slist<T, Alloc>::operator != (const slist& x) const
	{
		return !(*this == x);
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::size() const -> size_type
	{
		return slist_count_size(head.next);
	}
	template<class T, class Alloc>
	bool slist<T, Alloc>::empty() const
	{
		return head.next == 0;
	}

	template<class T, class Alloc>
	auto slist<T, Alloc>::begin() -> iterator
	{
		return iterator((slist_node<T>*)(head.next));
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::begin() const -> const_iterator
	{
		return const_iterator((slist_node*)(head.next));
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::end() -> iterator
	{
		return iterator(0);
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::end() const -> const_iterator
	{
		return const_iterator(0);
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::front() -> reference
	{
		return ((slist_node<T>*)(head.next))->data;
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::front() const -> const_reference
	{
		return ((slist_node<T>*)(head.next))->data;
	}
	template<class T, class Alloc>
	void slist<T, Alloc>::swap(slist& x)
	{
		slist_node_base tmp = head;
		head = x.head;
		x.head = tmp;
	}
	template<class T, class Alloc>
	void slist<T, Alloc>::push_front(const value_type& val)
	{
		slist_node<T>* ptr = create_node(val);
		slist_make_link(&head, ptr);
	}
	template<class T, class Alloc>
	void slist<T, Alloc>::pop_front()
	{
		slist_node<T>* tmp = (slist_node<T>*)head.next;
		head.next = head.next->next;
		destory_node(tmp);
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::insert_back(iterator pos, const value_type& val) -> iterator
	{
		slist_node<T>* ptr = create_node(val);
		ptr->next = (pos.node)->next;
		slist_make_link(pos.node, ptr);
		return iterator(ret);
	}
	template<class T, class Alloc>
	auto slist<T, Alloc>::insert_back(iterator pos, size_type n, const value_type& val) -> iterator
	{
		slist_node<T>* ptr;
		slist_node<T>* ret = (slist_node<T>*)pos.node;
		while (n--)
		{
			ptr = create_node(val);
			ptr->next = ret->next;
			slist_make_link(ret, ptr);
			ret = ptr;
		}
		return iterator(ret);
	}
	template<class T, class Alloc>
	template<claas InputIterator>
	auto slist<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) -> iterator
	{
		return insert_aux(pos, first, last, std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	template<claas InputIterator>
	auto slist<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type)->iterator
	{
		return insert(pos, size_type(first), value_type(last));
	}
	template<class T, class Alloc>
	template<claas InputIterator>
	auto slist<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type)->iterator
	{
		slist_node<T>* ptr;
		slist_node<T>* ret = (slist_node<T>*)pos.node;
		for (auto cur = first; cur != last; ++cur)
		{
			ptr = create_node((value_type)*cur);
			ptr->next = ret->next;
			slist_make_link(ret, ptr);
			ret = ptr;
		}
		return iterator(ret);
	}

	template<class T, class Alloc>
	void slist<T, Alloc>::erase_back(iterator pos)
	{
		slist_node<T>* tmp = (slist_node<T>*)((pos.node)->next);
		(pos.node)->next = (pos.node)->next->next;
		destory(tmp);

	}
	template<class T, class Alloc>
	void slist<T, Alloc>::erase_back(iterator first, size_type n )
	{
		while (n--)
		{
			erase_back(first);
		}
	}
	template<class T, class Alloc>
	void slist<T, Alloc>::clear()
	{
		erase_back(begin(), size() - 1);
		destory(head.next);
		head = 0;
	}


	/******** 工具函数 ********/
	template<class T, class Alloc>
	slist_node<T>* slist<T, Alloc>::create_node(const value_type& val)
	{
		slist_node<T>* ptr = node_allocator::allocate(); // 分配空间
		constructe(&(ptr->data), val);  // 构造元素对象
		ptr->next = 0;  
		return ptr;
	}
	template<class T, class Alloc>
	void slist<T, Alloc>::destroy(slist_node<T>* node)
	{
		destroy(&(node->data)); // 析构元素
		node_allocator::deallocate(node); // 释放空间
	}




}  // namespace TinySTL


#endif // !_SLIST_IMPL_H_

