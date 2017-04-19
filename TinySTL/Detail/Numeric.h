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
	// 非质变函数
	// 有两个版本，第一个版本用来计算init和[first, last)内每个元素累计之和
	// 第二个版本传入一个二元操作符，作用在init和[first, last)之间的每个元素
	// 二元操作符不具有交换律和结合律
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
	// 非质变函数
	// 有两个版本，第一个版本用来计算[first, last)内相邻元素的差值，result返回
	// 第二个版本传入一个二元操作符，作用在[first, last)内的每个相邻元素，result返回
	// 二元操作符不具有交换律和结合律
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
	// 非质变函数
	// 有两个版本，第一个版本用来计算两个输入序列对应元素的乘积，返回init为内积和
	// 第二个版本传入一个二元操作符，作用在两个输入序列的对应元素，返回init为作用后结果的累加和
	// 二元操作符不具有交换律和结合律
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
	// 非质变函数
	// 有两个版本，第一个版本用来计算[first, last)相邻元素之和，返回result
	// 第二个版本传入一个二元操作符，作用在[first, last)相邻元素，返回result
	// 二元操作符不具有交换律和结合律
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
			while ((n & 1) == 0) // n为偶数
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

