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
	
	/*  ���졢���������ƺ�����  */
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
	}

	template<class T, class Alloc, size_t Buf_sz>
	auto deque<T, Alloc, Buf_sz>::operator=(const deque& x) -> deque&
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	auto deque<T, Alloc, Buf_sz>::operator=(deque&& x) -> deque&
	{

	}
	
	/*  ����Ԫ�ز������� */
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::push_back(const value_type& val)
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::push_front(const value_type& val)
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::pop_back()
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::pop_front()
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::swap()
	{

	}
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::clear()
	{
		for (map_pointer cur = start.node + 1; cur != finish.node; ++cur)
		{
			data_allocator::destroy(*cur, *cur + buffer_size());
			data_allocator::deallocate(*cur, buffer_size());
		}   
		// ��ʱ��ʣ����β�����У�����������Ҳ���ܲ���������Ϊmap�ϵĽڵ�λ�ò�û�иı�
		if (start.node != finish.node)
		{                                  // ����������л��������Ǿ���һ�л�����
			data_allocator::destroy(start.cur, start.last);
			data_allocator::destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else  // �������ֻ��һ��
		{
			data_allocator::destroy(start.cur, start.last);
		}
		finish = start; // β���������׵�������ȫһ����ָ��ͬһ���ط�
	}

	/*  �ڲ����ߺ��� */
	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::create_map_and_nodes(size_type num_elem)
	{
		size_type num_nodes = num_elem / buffer_size() + 1;   // ����node������ 
		map_size = max(initial_map_size(), num_nodes + 2);  // ����map��size���������8��ȡ8������8��ȡnode����+2��ǰ�����һ��
		map = map_allocator::allocate(map_size);  // ����map�Ŀռ�
		map_pointer new_start = map + (map_size - num_nodes) / 2; 
		map_pointer new_finish = new_start + num_nodes - 1;       // ȡmap���м��ָ��
		for (map_pointer cur = new_start; cur <= new_finish; ++cur)
		{
			*cur = data_allocator::allocate(buffer_size());  // Ϊÿ�λ���������ռ�
		}
		start.set_node(new_start);   // ����startָ��Ļ�����
		finish.set_node(new_finish);  // ����finishָ��Ļ�����
		start.cur = start.first;     
		finish.cur = finish.first + num_elem % buffer_size();
	}

	template<class T, class Alloc, size_t Buf_sz>
	void deque<T, Alloc, Buf_sz>::fill_initialize(size_type n, const value_type& val)
	{
		create_map_and_nodes(n);   // �ȷ���
		for (map_pointer cur = start.node; cur != finish.node; ++cur)
		{
			*cur = unitialized_fill_n(*cur, buffer_size(), val);  
		}
		*cur = unitialized_fill_n(*cur, n % buffer_size(), val);  // ���һ�ε�����䣬��Ϊ���ܲ���Ҫ�����
	}

	template<class T, class Alloc, size_t Buf_sz>
	template<class InputIterator>
	void deque<T, Alloc, Buf_sz>::deque_aux(InputIterator first, InputIterator last, std::true_type)
	{
		deque(static_cast<size_type>(first), last);
	}
	template<class T, class Alloc, class Buf_sz>
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


	/*  �ⲿ��Ԫ���� */



}  // namespace TinySTL




#endif  // _DEQUE_IMPL_H_
