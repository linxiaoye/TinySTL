#ifndef _NUMERIC_H_
#define _NUMERIC_H_

/*
*  Author  @yyehl
*/

#include "../Iterator.h"

namespace TinySTL
{
	/********************[ accumulate ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ���ʱ亯��
	// �������汾����һ���汾��������init��[first, last)��ÿ��Ԫ���ۼ�֮��
	// �ڶ����汾����һ����Ԫ��������������init��[first, last)֮���ÿ��Ԫ��
	// ��Ԫ�����������н����ɺͽ����
	template<class InputIterator, class T>
	T accumulate(InputIterator first, InputIterator last, T init)
	{
		for (; first != last; ++first)
			init += *first;
		return init;
	}
	template<class InputIterator, class T, class BinaryOperation>
	T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation f)
	{
		for (; first != last; ++first)
			init = f(init, *first);
		return init;
	}

	/********************[ adjacent_difference ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ���ʱ亯��
	// �������汾����һ���汾��������[first, last)������Ԫ�صĲ�ֵ��result����
	// �ڶ����汾����һ����Ԫ��������������[first, last)�ڵ�ÿ������Ԫ�أ�result����
	// ��Ԫ�����������н����ɺͽ����
	template<class InputIterator, class OutputIterator>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			iterator_traits<InputIterator>::value_type tmp = *first;
			*++result = tmp - value;
			value = tmp;
		}
		return ++result;
	}
	template<class InputIterator, class OutputIterator, class BinaryOperation>
	OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation f)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			iterator_traits<InputIterator>::value_type tmp = *first;
			*++result = f(tmp, value);
			value = tmp;
		}
		return ++result;
 	}

	/********************[ inner_product ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ���ʱ亯��
	// �������汾����һ���汾�������������������ж�ӦԪ�صĳ˻�������initΪ�ڻ���
	// �ڶ����汾����һ����Ԫ�������������������������еĶ�ӦԪ�أ�����initΪ���ú������ۼӺ�
	// ��Ԫ�����������н����ɺͽ����
	template<class InputIterator1, class InputIterator2, class T>
	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init)
	{
		for (; first1 != last1; ++first1, ++first2)
			init = init + (*first1 * *first2);
		return init;
	}
	template<class InputIterator1, class InputIterator2, class T, class BinaryOperation>
	T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation f)
	{
		for (; first1 != last1; ++first1, ++first2)
			init = init + f(*first1, *first2);
		return init;
	}

	/********************[ partial_sum ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	// ���ʱ亯��
	// �������汾����һ���汾��������[first, last)����Ԫ��֮�ͣ�����result
	// �ڶ����汾����һ����Ԫ��������������[first, last)����Ԫ�أ�����result
	// ��Ԫ�����������н����ɺͽ����
	template<class InputIterator, class OutputIterator>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			value = value + *first;
			*++result = value;
		}
		return ++result;
	}
	template<class InputIterator, class OutputIterator, class BinaryOperation>
	OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation f)
	{
		if (first == last) return result;
		*result = *first;
		iterator_traits<InputIterator>::value_type value = *first;
		while (++first != last)
		{
			value = f(value, *first);
			*++result = value;
		}
		return ++result;
	}

	/********************[ power ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	template<class T, class Integer>
	inline T power(T x, Integer n)
	{
		return power(x, n, multiplies<T>());
	}
	template<class T, class Integer, class Operation>
	T power(T x, Integer n, Operation f)
	{
		if (n == 0) return x;
		else
		{
			while ((n & 1) == 0) // nΪż��
			{
				n >>= 1;
				x = f(x, x);
			}
			T ret = x;
			n >>= 1;
			while (n != 0)
			{
				x = f(x, x);
				if ((n & 1) != 0)
					ret = f(ret, x);
				n >>= 1;
			}
			return ret;
		}

	}


}  // namespace TinySTL

#endif // !_NUMERIC_H_

