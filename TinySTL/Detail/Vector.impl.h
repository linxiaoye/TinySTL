#ifndef _VECTOR_IMPL_H_
#define _VECTOR_IMPL_H_

/*
*   Author @linxiaoye
*/

namespace TinySTL {
	
	/*      ���졢��ֵ��������غ���    */
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
		vector_aux(first, last, std::is_integral<InputIterator>::type()); 
	}
	template<class T, class Alloc>
	template<class InputIterator>	            // ����������ֵĻ� 
	void vector<T, Alloc>::vector_aux(InputIterator first, InputIterator last, _false_type)
	{
		allocate_and_copy(first, last);
	}
	template<class T, class Alloc>
	template<class InputIterator>	            // ��������ֵĻ� 
	void vector<T, Alloc>::vector_aux(InputIterator size, InputIterator val, _true_type)
	{
		allocate_and_fill_n(size, val);
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
	vector<T, Alloc>::~vector()
	{
		destroy_and_deallacate_all(*this);
	}

 	/*          �Ƚϲ�����غ���         */
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
 	
	template<class T1, class Alloc_>
	bool operator == (const vector<T1, Alloc_>& v1, const vector<T1, Alloc_>& v2)
	{
		return v1.operator == (v2);
	}
	
	template<class T1, class Alloc_>
	bool operator != (const vector<T1, Alloc_>& v1, const vector<T1, Alloc_>& v2)
	{
		return !(v1 == v2);
	}
 	
 	
 	
 	/*        ������غ���       */





 	/*          tool functions           */ 
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
