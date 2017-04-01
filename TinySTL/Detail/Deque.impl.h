#ifndef _DEQUE_IMPL_H_
#define _DEQUE_IMPL_H_

/*
*   Author @linxiaoye
*/

namespace TinySTL 
{
	/******************* class dq_iter *****************************/
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::
		operator - (const self& x) const -> difference_type
	{ 
		return difference_type(buffer_size()*(x.node-node-1)+(cur-first)+(x.last-x.cur));
	}
	
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator ++ () -> self&
	{
		++cur;
		if (cur == last)
		{
			set_node(node+1);
			cur = first;
		}
		return *this;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator ++ (int) -> self
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator -- () -> self&
	{
		if (cur == first) 
		{
			set_node(node-1);
			cur = last - 1;
		}
		else
			--cur;
		return cur;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator -- (int) -> self
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator += (difference_type n) -> self&
	{
		difference_type offset = (cur - first) + n;
		if (offset >= 0 && offset < difference_type(buffer_size()))
		{
			cur = cur + n;
		}
		else if (offset < 0)
		{
			difference_type node_offset = offset / buffer_size() - 1;
			set_node(node + node_offset);
			offset = offset - (node_offset + 1) * buffer_size();
			cur = last + offset;
		}
		else 
		{
			difference_type node_offset = offset / buffer_size();
			set_node(node + node_offset);
			offset = offset - node_offset * buffer_size();
			cur = first + offset;
		}
		return *this;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator -= (difference_type n) -> self&
	{
		return this->operator+=(-n);
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator + (difference_type n) const -> self
	{
		self tmp = *this;
		tmp += n;
		return tmp;
	}
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	auto dq_iter<T, Ref, Ptr, Buf_sz>::operator - (difference_type n) const -> self
	{
		self tmp = *this;
		tmp -= n;
		return tmp;
	}
		
	template<class T, class Ref, class Ptr, size_t Buf_sz>
	void dq_iter<T, Ref, Ptr, Buf_sz>::set_node(map_pointer new_node)
	{
		node = new_node;
		first = *node;
		last = first + buffer_size();	
	}	
	
	/*************************** class deque **********************************/
	
	/*  构造、析构、复制函数等  */
	template<class T, class Alloc, size_t Buf_sz>
	deque<T, Alloc, Buf_sz>::deque() : start(), finish(), map(0), map_size(0)
	{
		create_map_and_nodes(0);
	}
	template<class T, class Alloc, size_t Buf_sz>
	deque<T, Alloc, Buf_sz>::deque(size_t n, const value_type& val) : start(), finish(), map(0), mapsize(0)
	{
		fill_initialize(n, val);
	}
	template<class T, class Alloc, size_t Buf_sz>
	template<class InputIterator>
	deque<T, Alloc, Buf_sz>::deque(InputIterator first, InputIterator last)
	{
		deque_aux(first, last, std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc, size_t Buf_sz>
	deque<T, Alloc, Buf_sz>::deque(const deque& x)
	{
		deque(x.start, x.finish);
	}
	template<class T, class Alloc, size_t Buf_sz>
	deque<T, Alloc, Buf_sz>::~deque()
	{
		for (map_pointer cur = start.node; cur <= finish.node; ++cur)
		{
			data_allocator::destroy(*cur, *cur + buffer_size());
			data_allocator::deallocate(*cur, buffer_size());
		}
		map_allocator::destroy(map, map + map_size);
		map_allocator::deallocate(map, map_size);
		map_size = 0;
		map = nullptr;
		start.~dq_iter();
		finish.~dq_iter();
	}

	template<class T, class Alloc, size_t Buf_sz>
	auto deque<T, Alloc, Buf_sz>::operator=(const deque& x) -> deque&
	{
		if (*this != x) 
		{
			~deque();
			deque(x);
		}
		return *this;
	}
	template<class T, class Alloc, size_t Buf_sz>
	auto deque<T, Alloc, Buf_sz>::operator=(deque&& x) -> deque&
	{
		start = x.start;
		finish = x.finish;
		map = x.map;
		map_size = x.map_size;
		x.start = x.finish = iterator();
		map = nullptr;
		map_size = 0;
		
	}
	
	/*  其他元素操作函数 */
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::push_front(const value_type& val)
	{
		if (start.cur != start.first) // 如果缓存行头部还有至少还有一个空间
		{
			--start.cur;
			data_allocator::construct(start.cur, val);
		}
		else if (start.node != map) // 如果map头部至少还有一个空间
		{
			map_pointer tmp = map;
			*tmp = data_allocator::allocate(buffer_size());
			start.set_node(tmp);
			start.cur = start.last;
			data_allocator::construct(start.cur, val);
		}
		else // map已经没有空间了，需要重新安排map
		{
			difference_type tmp = finish.node - start.node;
			if (map_size - tmp >= 2) //如果map尾部还剩2个及以上空间
			{                                               //那么就原map往后平移
				
				uninitialized_copy(start.node, finish.node, map + (map_size - tmp) / 2);
				start.node = map + tmp / 2;
				finish.node = start.node + tmp;
				start.set_node(start - 1);
				start.cur = start.last;
				data_allocator::construct(start.cur, val);
			}
			else          // 如果尾部空间也不够了，那就必须重新分配map空间
			{
				size_type new_map_size = map_size * 2;
				map_pointer new_map = map_allocator::allocate(new_map_size);
				uninitialized_copy(start.node, finish.node, new_map + new_map_size / 4);
				map_allocator::destroy(start.node, finish.node);
				map_allocator::deallocate(map, map_size);
				map = new_map;
				map_size = new_map_size;
				start.node = new_map + new_map_size / 4;
				finish.node = start.node + tmp;
				start.set_node(start - 1);
				start.cur = start.last;
				data_allocator::construct(start.cur, val);
			}
		}
	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::push_back(const value_type& val)
	{
		if (finish.cur != finish.last - 1)  // 如果这段缓存区尾部还有大于一个空间
		{
			data_allocator::construct(finish.cur, val);
		}
		else if (finish.node != map + map_size - 1) // 如果map尾部还有至少一个空间，那只需要分配一段缓存区
		{
			data_allocator::construct(finish.cur, val);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		else  // 如果map尾部的空间也不足一个了。。
		{
			difference_type tmp = finish.node - start.node;
			if (map_size - tmp >= 2) // 头部空间还大于两个
			{
				uninitialized_copy(start.node, finish.node, map + (map_size - tmp) / 2);
				start.node = map + (map_size - tmp) / 2;
				finish.node = start.node + tmp;
				data_allocator::construct(finish.cur, val);
				finish.set_node(finish.node + 1);
				finish.cur = finish.first;
			} 
			else   //如果头部也没有两个以上空间了，就必须重新再分配map了
			{
				size_type new_map_size = 2 * map_size;
				map_pointer new_map = map_allocator::allocate(new_map_size);
				uninitialized_copy(start.node, finish.node, new_map + new_map_size / 4);
				map_allocator::destroy(start.node, finish.node);
				map_allocator::deallocate(map, map_size);
				start.node = new_map + new_map_size / 4;
				finish.node = start.node + tmp;
				map = new_map; 
				map_size = new_map_size;
				data_allocator::construct(finish.cur, val);
				finish.set_node(finish.node + 1);
				finish.cur = finish.first;
			}
		}
	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::pop_back()
	{
		if (finish.cur != finish.first) // 如果该段缓存区剩余元素大于1
		{
			--finish.cur;
			data_allocator::destroy(finish.cur);
		}
		else  // 如果该段缓存区已经没有元素了，则需要回收这段缓存区
		{
			data_allocator::deallocate(finish.first, buffer_size());
			finish.set_node(finish.node - 1);
			finish.cur = finish.last - 1;
			data_allocator::destroy(finish.cur);
		}
	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::pop_front()
	{
		if (start.cur != start.last - 1) //如果该缓存区剩余元素大于1
		{
			data_allocator::destroy(start.cur);
			++start.cur;
		}
		else  //否则，就需要回收这一段缓存区
		{
			data_allocator::destroy(start.cur);
			data_allocator::deallocate(start.first, buffer_size());
			start.set_node(start.node + 1);
			start.cur = start.first;
		}
	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::swap(deque& x)
	{
		swap(start, x.start);
		swap(finish, x.finish);
		swap(map, x.map);
		swap(map_size, x.map_size);
	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::clear()
	{
		for (map_pointer cur = start.node + 1; cur != finish.node; ++cur)
		{
			data_allocator::destroy(*cur, *cur + buffer_size());
			data_allocator::deallocate(*cur, buffer_size());
		}   
		// 此时还剩下首尾的两行，可能相连，也可能不相连，因为map上的节点位置并没有改变
		if (start.node != finish.node)
		{                                  // 如果还有两行缓存区，那就留一行缓存区
			data_allocator::destroy(start.cur, start.last);
			data_allocator::destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else  // 如果本就只有一行
		{
			data_allocator::destroy(start.cur, start.last);
		}
		finish = start; // 尾迭代器与首迭代器完全一样，指向同一个地方
	}

	template<class T, class Alloc, size_t Buf_sz>
	bool deque<T, Alloc, Buf_sz>::operator == (const deque& x) const
	{
		return start == x.start && finish == x.finish && map == x.map && map_size = x.map_size;
	}
	template<class T, class Alloc, size_t Buf_sz>
	bool deque<T, Alloc, Buf_sz>::operator != (const deque& x) const
	{
		return !(this -> operator == (x));
	}

	/*  内部工具函数 */
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::create_map_and_nodes(size_type num_elem)
	{
		size_type num_nodes = num_elem / buffer_size() + 1;   // 计算node的数量 
		map_size = max(initial_map_size(), num_nodes + 2);  // 计算map的size，如果不足8，取8，大于8，取node数量+2，前后各加一个
		map = map_allocator::allocate(map_size);  // 分配map的空间
		map_pointer new_start = map + (map_size - num_nodes) / 2; 
		map_pointer new_finish = new_start + num_nodes - 1;       // 取map的中间的指针
		for (map_pointer cur = new_start; cur <= new_finish; ++cur)
		{
			*cur = data_allocator::allocate(buffer_size());  // 为每段缓存区分配空间
		}
		start.set_node(new_start);   // 调整start指向的缓存区
		finish.set_node(new_finish);  // 调整finish指向的缓存区
		start.cur = start.first;     
		finish.cur = finish.first + num_elem % buffer_size();
	}

	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::fill_initialize(size_type n, const value_type& val)
	{
		create_map_and_nodes(n);   // 先分配
		for (map_pointer cur = start.node; cur != finish.node; ++cur)
		{
			*cur = unitialized_fill_n(*cur, buffer_size(), val);  
		}
		*cur = unitialized_fill_n(*cur, n % buffer_size(), val);  // 最后一段单独填充，因为可能不需要填充满
	}

	template<class T, class Alloc, size_t Buf_sz>
	template<class InputIterator>
	void deque<T, Alloc, Buf_sz>::deque_aux(InputIterator first, InputIterator last, std::true_type)
	{
		deque(static_cast<size_type>(first), last);
	}
	template<class T, class Alloc, size_t Buf_sz>
	template<class InputIterator>
	void deque<T, Alloc, Buf_sz>::deque_aux(InputIterator first, InputIterator last, std::false_type)
	{
		if (first != last)
		{
			size_type num_elem = last - first;
			create_map_and_nodes(num_elem);
			uninitialized_copy(start, first, last);
		}
	}


	/*  外部友元函数 */
	template<class T, class Alloc, size_t Buf_sz>
	bool operator == (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs)
	{
		return lhs.operator==(rhs);
	}
	template<class T, class Alloc, size_t Buf_sz>
	bool operator != (const deque<T, Alloc, Buf_sz>& lhs, const deque<T, Alloc, Buf_sz>& rhs)
	{
		return lhs.operator!=(rhs);
	}
	template<class T, class Alloc, size_t Buf_sz>
	void swap(deque<T, Alloc, Buf_sz>& lhs, deque<T, Alloc, Buf_sz>& rhs)
	{
		lhs.swap(rhs);
	}

}  // namespace TinySTL

#endif  // _DEQUE_IMPL_H_
