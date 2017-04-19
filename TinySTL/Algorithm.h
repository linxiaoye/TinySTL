#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

/*
*   Author  @yyehl
*/

#include <cstring>
#include <utility>
#include "Allocator.h"
#include "Functional.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Utility.h"


namespace TinySTL 
{
	/********************[ equal ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ����ڶ������бȵ�һ�����г�����ô����Ĳ��ֽ����迼��
	// �����Ҫ�ж�����������ȫһ������ô��Ҫ�ڵ���equal����֮ǰ���ж��������еĳ����Ƿ�һ����
	template<class InputIterator1, class InputIterator2>
	inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2) 
		{
			if (*first1 != *first2)
				return false;
			return true;
		}
	}
	template<class InputIterator1, class InputIterator2, class BinaryPredicate>
	inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate eq)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!eq(*first1, *first2))
				return false;
			return true;
		}
	}

	/********************[ fill ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class ForwardIterator, class T>
	void fill(ForwardIterator first, ForwardIterator last, const T& val)
	{
		for (; first != last; ++first)
			*first = val;
	}

	/********************[ fill_n ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ���n���������еĳ��ȣ���ô������ɲ���Ԥ�ڵĽ��
	template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for ( ; n > 0; --n, ++first)
		{
			*first = value;
		}
		return first;
	}
	template<class Size>
	char* fill_n(char* first, Size n, const char& value) 
	{
		memset(first, static_cast<unsigned char>(value), n);
		return first + n;
	}
	template<class Size>
	wchar_t* fill_n(wchar_t* first, Size n, const wchar_t& value)
	{
		memset(first, static_cast<unsigned char>(value), n * sizeof(wchar_t));
		return first + n;
	}

	/********************[ iter_swap ]***********************************/
	/********************[Algorithm Time Complexity: O(1)]************/
	template<class ForwardIterator1, class ForwardIterator2>
	void iter_swap(ForwardIterator1 iter1, ForwardIterator2 iter2)
	{
		iterator_traits<ForwardIterator1>::value_type tmp = *iter1;
		*iter1 = *iter2;
		*iter2 = tmp;
	}

	/********************[ lexicographical_compare ]***********************************/
	/********************[Algorithm Time Complexity: O(n)]************/
	// �ֵ����з�ʽ����
	template<class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
			// ���������о�˵����ǰ���������
		}
		return first1 == last1 && first2 != last2;
	}
	template<class InputIterator1, class InputIterator2, class BinaryCompare>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryCompare cmp)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (cmp(*first1, *first2))
				return true;
			if (cmp(*first2, *first2))
				return false;
			// ���������о�˵����ǰ���������
		}
		return first1 == last1 && first2 != last2;
	}

	/********************[ max ]***********************************/
	/********************[Algorithm Time Complexity: O(1)]************/
	template<class T>
	inline const T& max(const T& x, const T& y)
	{
		return x < y ? y : x;
	}
	template<class T, class Compare>
	inline const T& max(const T& x, const T& y, Compare cmp)
	{
		return cmp(x, y) ? y : x;
	}

	/********************[ min ]***********************************/
	/********************[Algorithm Time Complexity: O(1)]************/
	template<class T>
	inline const T& min(const T& x, const T& y)
	{
		return x < y ? x : y;
	}
	template<class T, class Compare>
	inline const T& min(const T& x, const T& y, Compare cmp)
	{
		return cmp(x, y) ? x : y;
	}

	/********************[ less ]***********************************/
	/********************[Algorithm Time Complexity: O(1)]************/	
	template<class T>
	bool less(const T& lhs, const T& rhs)
	{
		return lhs < rhs;
	}

	/********************[ mismatch ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// �������������״γ��ֲ�ͬ�ĵط�������һ��pair
	template<class InputIterator1, class InputIterator2>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}
	template<class InputIterator1, class InputIterator2, class BinaryEqual>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryEqual eq)
	{
		while (first1 != last1 && eq(*first1, *first2))
		{
			++first1;
			++first2;
		}
		return pair<InputIterator1, InputIterator2>(first1, first2);
	}

	/********************[ copy ]***********************************/
	/********************[Algorithm Time Complexity: O(n)]************/
	


	/****************************************************************************************[heap���]****************************************/
	/********************[ down_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(logN)]************/
	template<class RandomIterator>                                              // down_heap���ڶ���Ԫ����ĩβԪ�ؽ�����λ��֮�������Чִ�е�
	void down_heap(RandomIterator first, RandomIterator last, RandomIterator head)  // [first, last]
	{
		if (first != last)
		{
			size_t index = first - head;
			size_t right_child_index = 2 * index + 2;
			while (right_child_index < (last - head))  // ִ����˷�������ض����ڱ�pop��һ�����ֵʱ��������Ϊ��
			{
				if (*(head + right_child_index) < *(head + right_child_index - 1))
				{
					--right_child_index;
				}
				if (*(head + index > *(head + right_child_index)))  break;
				*(head + index) = *(head + right_child_index);
				index = right_child_index;
				right_child_index = 2 * index + 2;
				if (right_child_index == (last - head))  // �������µ�ֻ����Ҷ�ӣ�û����Ҷ��
				{
					*(head + index) = *(head + right_child_index - 1);
				}
			}
		}
	}

	/********************[ up_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(logN)]************/
	template<class RandomIterator, class Compare>
	void up_heap(RandomIterator first, RandomIterator last, RandomIterator head)      // [first, last]
	{                                                                                               
		if (first != last)
		{
			size_t index = last - head;
			size_t parent_index = (index - 1) / 2;
			while (index > 0 && *(head + parent_index) < *(head + index))
			{
				swap(*(head + parent_index), *(head + index));
				index = parent_index;
				parent_index = (index - 1) / 2;
			}
		}
	}

	/********************[ push_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(logN)]************/
	template<class RandomIterator>                               // push_heap֮ǰ��Ԫ�����Ѿ���ӵ�������ĩβ��
	void push_heap(RandomIterator first, RandomIterator last)    // [first, last)
	{
		if (first != last)
		{
			up_heap(first, last - 1, first);
		}
	}

	/********************[ pop_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(logN)]************/
	template<class RandomIterator>
	void pop_heap(RandomIterator first, RandomIterator last)   // [first, last)
	{
		if (first != last)
		{
			swap(*first, *(last - 1));
			down_heap(first, last - 2, first);
		}

	}
	/********************[ make_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(NlogN)]************/
	template<class RandomIterator>
	void make_heap(RandomIterator first, RandomIterator last)  // [first, last)
	{
		auto index = last - first;
		if (index < 2) return; // �������Ϊ0��1���Ͳ���Ҫ������
		auto parent_index = (index - 2) / 2;
		while (parent_index)
		{
			down_heap(first + parent_index, last - 1, first);
			--parent_index;
		}
	}

	/********************[ sort_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(NlogN)]************/
	template<class RandomIterator>
	void sort_heap(RandomIterator first, RandomIterator last)  // [first, last)
	{
		if (last - first < 2) return;
		while (last - first > 1)
		{
			pop_heap(first, --last);
		}
	}
	/****************************************************************************************[heap���]*************************************/










	


}   // namespace TinySTL

/****************************************[��ֵ���㷨]***********************************************/

#include "Detail\Numeric.h"

#endif  // _ALGORITHM_H_
