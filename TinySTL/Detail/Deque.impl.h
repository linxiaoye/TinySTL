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
	
	
}  // namespace TinySTL




#endif  // _DEQUE_IMPL_H_
