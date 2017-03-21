#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

/*
*   Author @linxiaoye
*/

namespace TinySTL {
	
	/***************���졢��ֵ��������غ���**********************/
	
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n)
	{
		allocate_and_fill_n(n, value_type());
	}

	template<class T, class Alloc>
	vector<T, Alloc>::vector(const size_type n, const value_type& value)
	{
		allocate_and_fill_n(n, value);
	}
	
	template<class T, class Alloc>
	vector<T, Alloc>::vector(const vector& v)
	{
		allocate_and_copy(v.cbegin(), v.cend());
	}
	
	template<class T, class Alloc>
	template<class InputIterator>              // ��Ҫ����ʵ�����������ֻ������������� 
	vector<T, Alloc>::vector(InputIterator first, InputIterator last)
	{
		vector_aux(first, last, typename std::is_integral<InputIterator>::type()); 
	}
	template<class T, class Alloc>
	template<class InputIterator>	            // ����������ֵĻ� 
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type)
	{
		allocate_and_copy(first, last);
	}
	template<class T, class Alloc>
	template<class InputIterator>	            // ��������ֵĻ� 
	void vector<T, Alloc>::vector_aux(InputIterator size, InputIterator val, std::true_type)
	{
		allocate_and_fill_n(size, val);
	}	
	
	template<class T, class Alloc>
	vector<T, Alloc>::vector(vector&& v)
	{
		_start = v._start;
		_finish = v._finish;
		_end_of_storage = v._storage;	
		v._start = v._finish = v._end_of_storage = 0;
	}	
	
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector& v)
	{
		if (this == &v)
			return *this;
		else 
			return allocate_and_copy(v.begin(), v.end());
	}
	
	template<class T, class Alloc>
	vector<T, Alloc>& vector<T, Alloc>::operator = (const vector&& v)
	{
		if (this != &v)
		{
			destroy_and_deallocate_all();
			_start = v._start;
	 		_finish = v._finish;
			_end_of_storage = v._storage;	
			v._start = v._finish = v._end_of_storage = 0;
		}

	}
	
	template<class T, class Alloc>
	vector<T, Alloc>::~vector()
	{
		clear();
	}

 	/*****************�Ƚϲ�����غ��� ***************************/
 	
	template<class T, class Alloc>
	bool vector<T, Alloc>::operator == (const vector& v) const 
	{
		if (size() != v.size())
			return false;
		auto iter1 = begin();
		auto iter2 = v.begin();
		for ( ; iter1 != end() && iter2 != v.end(); ++iter1, ++iter2) 
		{
			if (*iter1 != *iter2) 
				return false;
		}
		return true;	
	}

	template<class T, class Alloc>
	bool vector<T, Alloc>::operator != (const vector& v) const
	{
		return !((*this) == v);
	}
 	
	template<class T, class Alloc>   // ��Ԫ����  �ǳ�Ա���� 
	bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
	{
		return v1.operator == (v2);
	}
	
	template<class T, class Alloc>   // ��Ԫ����  �ǳ�Ա���� 
	bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2)
	{
		return !(v1 == v2);
	}
	
 	/*****************������С��غ��� **************************/
 	template<class T, class Alloc>
 	void vector<T, Alloc>::resize(size_type n, value_type val)
 	{
 		if (n < size()) 
		{
			dataAllocator::destroy(_start + n, _finish);
			_finish = _start + n;
		}
		else if (n > size() && n <= capacity())
		{
			uninitialized_fill_n(_finish, n - size(), val);
			_finish = _finish + (n - size());
		}
		else 
		{
			auto new_capacity = (n > capacity()) ? n : get_new_capacity();
			auto new_start = dataAllocator::allocate(new_capacity);
			uninitialized_copy(begin(), end(), new_start);
			uninitialized_fill_n(new_start + size(), n - size(), value_type());
			destroy_and_deallocate_all();
			_start = new_start;
			_finish = _start + n;
			_end_of_storage = _start + new_capacity;
		}
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::reserve(size_type n)
	{
		if (n > capacity())
		{
			auto new_start = dataAllocator::allocate(n);
			auto new_finish = uninitialized_copy(begin(), end(), new_start);
			uninitialized_fill_n(new_finish, n - size(), value_type());
			destroy_and_deallocate_all();
			_start = new_start;
			_finish = new_finish;
			_end_of_storage = _start + n;	
		}		
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::shrink_to_fit()
	{
		if (size() != capacity())
		{
			auto new_start = dataAllocator::allocate(size());
			auto new_finish = uninitialized_copy(begin(), end(), new_start);
			destroy_and_deallocate_all();
			_start = new_start;
			_finish = _end_of_storage = new_finish;
		}
	}

 	
 	
 	
	/******************�����޸���غ���****************************/

	template<class T, class Alloc>
	auto vector<T, Alloc>::insert(iterator pos, const value_type& val) -> iterator
	{
		if (size() != capacity())
		{
			insert(pos, 1, val);
			return pos;
		}
		else
		{
			auto tmp = pos - begin();
			insert(pos, 1, val);
			return begin() + tmp;
		}
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator pos, size_type n, const value_type& val)
	{
		if (n != 0)
		{
			if (n <= size_type(_end_of_storage - _finish))
			{
				auto elem_after = _finish - pos;   // �����֮��ʣ����Ԫ��
				if (elem_after > n)                 
				{
					auto iter = end();
					auto new_finish = end() + n;
					for ( ; iter != pos - 1; --iter, --new_finish)
					{
						*new_finish = *iter;
					}
					uninitialized_fill_n(pos, n, val);
					_finish = _finish + n;
				}
				else
				{
					uninitialized_copy(pos, end(), pos + n);
					uninitialized_fill_n(pos, n , val);
					_finish = _finish + n;
				} 
			}
			else 
			{
				size_type old_size = size();
				size_type new_capacity = (n > capacity()) ? n : get_new_capacity();
				auto new_start = dataAllocator(new_capacity);
				uninitialized_copy(begin(), pos, new_start);
				auto new_pos = new_start + (pos - begin());
				uninitialized_fill_n(new_pos, n, val);
				uninitialized_copy(pos, end(), new_pos + n);
				destroy_and_deallocate_all();
				_start = new_start;
				_finish = _start + old_size + n;
				_end_of_storage = _start + new_capacity;
			}
		}
	}
	
	template<class T, class Alloc>
	template<class InputIterator>     // ��Ҫ�ж�InputIterator�ǲ������� 
	void vector<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last)
	{
		insert_aux(pos, first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator pos, InputIterator n, InputIterator val, std::true_type)
	{
		insert(pos, n, val);
	}
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type)
	{
		/*  Ĭ��first��posָ�Ĳ���ͬһ��vector����������  */
		auto len_insert = last - first;
		auto len_left = capacity() - size();
		if (len_insert <= len_left) 
		{
			auto elem_after = end() - pos;
			if (len_insert < elem_after)
			{
				auto new_finish = _finish + len_insert;
				auto iter = end();
				for (int i = 0; i < elem_after; --iter, --new_finish)
				{
					*new_finish = *iter;
				}
				uninitialized_copy(first, last, pos);
				_finish = _finish + len_insert;
			}
			else
			{
				uninitialized_copy(pos, end(), pos + len_insert);
				uninitialized_copy(first, last, pos);
				_finish = _finish + len_insert;				
			}
		}
		else
		{
			size_type old_size = size();
			size_type new_capacity = (len_insert > capacity()) ? len_insert : get_new_capacity();
			auto new_start = dataAllocator(new_capacity);
			uninitialized_copy(begin(), pos, new_start);
			auto new_pos = new_start + (pos - begin());
			uninitialized_copy(first, last, new_pos);
			uninitialized_copy(pos, end(), new_pos + len_insert);
			destroy_and_deallocate_all();
			_start = new_start;
			_finish = _start + old_size + len_insert;
			_end_of_storage = _start + new_capacity;			
		}
	}

	template<class T, class Alloc>
	auto vector<T, Alloc>::erase(iterator first, iterator last) -> iterator
	{
		auto len_erase = last - first;
		if (len_erase > 0) 
		{
			auto elem_after = end() - last;
			auto ret = first;
			for ( ; last != end(); ++first, ++last)
			{
				*first = *last;
			}
			destroy(end() - len_erase, end());
			_finish = _finish - len_erase;
			return ret;
		}
		else
		{
			return first;
		}
	}
	
	template<class T, class Alloc>
	auto vector<T, Alloc>::erase(iterator pos) -> iterator 
	{
		return erase(pos, pos + 1);
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::push_back(const value_type& val)
	{
		insert(end(), val);
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::pop_back()
	{
		erase(end());
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::clear()
	{
		destroy_and_deallocate_all();
		_finish = _start;
	}	

	template<class T, class Alloc>
	void vector<T, Alloc>::swap(vector& v)
	{
		if (this != &v)
		{
			swap(_start, v._start);
			swap(_finish, v._finish);
			swap(_end_of_storage, v._end_of_storage);
		}
	}

 	/******************�����ռ�������غ���*******************/ 
	template<class T, class Alloc>
	void vector<T, Alloc>::allocate_and_fill_n(const size_type size, const value_type& val)
	{
		_start = dataAllocator::allocate(size);
		TinySTL::uninitialized_fill_n(_start, size, val);
		_finish = _end_of_storage = _start + size;
	}
	
	template<class T, class Alloc>
	template<class InputIterator>
	void vector<T, Alloc>::allocate_and_copy(InputIterator first, InputIterator last)
	{
		_start = dataAllocator::allocate(last - first);
		_finish = TinySTL::uninitialized_copy(first, last, _start);
		_end_of_storage = _finish;
	}
	
	template<class T, class Alloc>
	void vector<T, Alloc>::destroy_and_deallocate_all()
	{
		if (capacity() != 0)
		{
			dataAllocator::destroy(begin(), end());
			dataAllocator::deallocate(begin(), capacity());
		}		
	}
	
}   // namespace TinySTL


#endif  // _VECTOR_IMPL_H_
