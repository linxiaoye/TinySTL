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
	/*
	     copy��[first, last)�����Ԫ�ؿ�����[result, result + (last - first))֮�У�����result + (last - first)
	     ���resultλ��[first, last) ֮�У���ô�������δ��֪�Ľ��
	*/
	template<class InputIterator, class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}


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


	/****************************************************************************************[set���]*************************************/
	/********************[ set_union ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		����������set�Ĳ��������ظ������s1��xԪ���ظ�m�Σ�s2��xԪ���ظ�m�Σ���ô���ս��xԪ�صĴ���Ϊmax��m��n��
		�������У������б�������ԭ���ϵ����˳��
		����ֵΪһ��ָ���������β�˵ĵ�����
	*/
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
				++first2;
			}
			++result;
		}
		return copy(first1, last1, copy(first2, last2, result));
	}

	/********************[ set_intersection ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		����������set�Ľ��������ظ������s1��xԪ���ظ�m�Σ�s2��xԪ���ظ�m�Σ���ô���ս��xԪ�صĴ���Ϊmin��m��n��
		�������У������б�������ԭ���ϵ����˳��
		����ֵΪһ��ָ���������β�˵ĵ�����
	*/
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_insertsection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
				++first1;
			else if (*first2 < *first1)
				++first2;
			else
			{
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
		}
		return result;
	}

	/********************[ set_difference ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		����������set�Ĳ�����ظ������s1��xԪ���ظ�m�Σ�s2��xԪ���ظ�m�Σ���ô���ս��xԪ�صĴ���Ϊmax��m-n��0��
		�������У������б�������ԭ���ϵ����˳��
		����ֵΪһ��ָ���������β�˵ĵ�����
	*/
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1)
				++first2;
			else
			{
				++first1; 
				++first2;
			}
		}
		return copy(first1, last1, result);
	}

	/********************[ set_symmetric_difference ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		����������set�ĶԳƲ�����ظ������s1��xԪ���ظ�m�Σ�s2��xԪ���ظ�m�Σ���ô���ս��xԪ�صĴ���Ϊmax��|m-n|��0��
		����s1�г��ֵ���s2�в����֣��Լ���s2�г�����s1�в����ֵĲ���
		��s1��s2���������ж�ֻ������һ�ε�Ԫ�أ����ظ�
		�������У������б�������ԭ���ϵ����˳��
		����ֵΪһ��ָ���������β�˵ĵ�����
	*/
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = first1;
				++first1;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				++first1;
				++first2;
			}
		}
		return copy(first1, last1, copy(first2, last2, result));
	}


	/********************[ count ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ����[first, last)��Ԫ��ֵ����val������
	template<class InputIterator, class T>
	size_t count(InputIterator first, InputIterator last, const T& val)
	{
		if (first == last) return 0;
		size_t i = 0;
		for (; first != last; ++first)
		{
			if (*first == val)
				++i;
		}
		return i;
	}
	
	/********************[ find ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ����[first, last)��Ԫ��ֵ����val�ĵ�һ��������
	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& val)
	{
		while (first != last && *first != val)
			++first;
		return first;
	}


	/********************[ for_each ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class InputIterator, class Function>
	Function for_each(InputIterator first, InputIterator last, Function f)
	{
		for (; first != last; ++first)
			f(*first);
		return f;
	}

	/********************[ includes ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class InputIterator1, class InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1) return false;
			else if (*first1 < *first2) ++first1;
			else {
				++first1;
				++first2;
			}
		}
		return first2 == last2;
	}

	/********************[ max_element ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last)  return first;
		ForwardIterator ret = first;
		for (++first; first != last; ++first)
		{
			if (*first > *ret)
				ret = first;
		}
		return ret;
	}

	/********************[ min_element ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last)  return first;
		ForwardIterator ret = first;
		while (++first != last)
		{
			if (*first < ret)
				ret = first;
		}
		return ret;
	}

	/********************[ merge ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator ret)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*ret = *first1;
				++first1;
				++ret;
			}
			else
			{
				*ret = *first2;
				++first2;
				++ret;
			}
		}
		return copy(first1, last1, copy(first2, last2, ret));
	}


	/********************[ partitiom ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		��[first, last)��Ԫ����������ʹ��PredΪtrue��ǰ��Σ�ʹ��PredΪfalse���ں���
		����㷨����֤Ԫ�ص�ԭʼ���λ�ã�����˵x1��x2ǰ�棬x1,x2��Ԫ�ض���ǰ����ƶ�������
										���ܱ�֤x1����x2��ǰ�棩
	
	*/
	template<class BidirectionIterator, class Pred>
	BidirectionIterator partition(BidirectionIterator first, BidirectionIterator last, Pred pred)
	{
		while (1)
		{
			while (1)
			{
				if (first == last)
					return first;
				else if (pred(*first))  // ���㲻�ƶ�����
					++first;
				else                  // �����ƶ�������*first���ᱻ�ƶ�������
					break;  
			}
			--last;
			while (1)
			{
				if (first == last)
					return first;
				else if (!pred(*last))
					--last;
				else
					break;
			}
			iter_swap(first, last);
			++first;
		}
	}


	/********************[ search ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		Ѱ������[first2, last2)������[first1, last1)��һ�γ��ֵĵ�
		���ص�һ�γ��ֵĵ��ĩβԪ�غ�һλ��
	*/
	template<class ForwardIterator1, class ForwardIterator2>
	InputIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		int d1 = last1 - first1;
		int d2 = last2 - first2; 
		if (d1 < d2) return last1;  // �������2������1�������ǲ����ܳ�Ϊ��������

		ForwardIterator1 cur1 = first1;
		ForwardIterator2 cur2 = first2;

		while (cur2 != last2)
		{
			if (*cur1 == *cur2)  // �����ǰԪ����ȡ���Ƚ���һԪ��
			{
				++cur1;
				++cur2;
			}
			else       // �����ǰԪ�ز����
			{
				if (d1 == d2)  // ����d1��d2��ȣ���ô��˵�������ܳ�Ϊ����������
					return last1;     // ���û���ҵ����ͷ�������1��last1
				else
				{
					cur1 = ++first1;
					cur2 = first2;       // ׼�����µ��������һ�Σ�cur2����Ҫ���»ص�first2
					--d1;
				}
			}
		}
		return first1;    // ����ҵ��ˣ��ͷ�������1�е�һ�γ�������2�ĵط�����λ��
	}














}   // namespace TinySTL

/****************************************[��ֵ���㷨]***********************************************/

#include "Detail\Numeric.h"

#endif  // _ALGORITHM_H_
