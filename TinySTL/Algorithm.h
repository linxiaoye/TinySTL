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
	// 如果第二个序列比第一个序列长，那么多余的部分将不予考虑
	// 如果需要判断两个序列完全一样，那么需要在调用equal函数之前先判断两个序列的长度是否一样长
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
	// 如果n大于了序列的长度，那么将会造成不可预期的结果
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
	// 字典排列方式排序
	template<class InputIterator1, class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first2 < *first1)
				return false;
			// 还继续进行就说明当前两序列相等
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
			// 还继续进行就说明当前两序列相等
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
	// 返回两个序列首次出现不同的地方，返回一个pair
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
	     copy将[first, last)里面的元素拷贝到[result, result + (last - first))之中，返回result + (last - first)
	     如果result位于[first, last) 之中，那么将会出现未可知的结果
	*/
	template<class InputIterator, class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}


	/****************************************************************************************[heap相关]****************************************/
	/********************[ down_heap ]***********************************/
	/********************[Algorithm Time Complexity: O(logN)]************/
	template<class RandomIterator>                                              // down_heap是在顶端元素与末尾元素交换了位置之后才能有效执行的
	void down_heap(RandomIterator first, RandomIterator last, RandomIterator head)  // [first, last]
	{
		if (first != last)
		{
			size_t index = first - head;
			size_t right_child_index = 2 * index + 2;
			while (right_child_index < (last - head))  // 执行下朔操作，必定是在被pop了一个最大值时，即顶部为空
			{
				if (*(head + right_child_index) < *(head + right_child_index - 1))
				{
					--right_child_index;
				}
				if (*(head + index > *(head + right_child_index)))  break;
				*(head + index) = *(head + right_child_index);
				index = right_child_index;
				right_child_index = 2 * index + 2;
				if (right_child_index == (last - head))  // 如果最底下的只有左叶子，没有右叶子
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
	template<class RandomIterator>                               // push_heap之前，元素是已经添加到了容器末尾的
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
		if (index < 2) return; // 如果长度为0或1，就不需要操作了
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
	/****************************************************************************************[heap相关]*************************************/


	/****************************************************************************************[set相关]*************************************/
	/********************[ set_union ]***********************************/
	/********************[Algorithm Time Complexity: O(N)]************/
	/*
		求两个有序set的并集，可重复，如果s1中x元素重复m次，s2中x元素重复m次，那么最终结果x元素的次数为max（m，n）
		输出结果中，并集中保持两个原集合的相对顺序
		返回值为一个指向输出区间尾端的迭代器
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
		求两个有序set的交集，可重复，如果s1中x元素重复m次，s2中x元素重复m次，那么最终结果x元素的次数为min（m，n）
		输出结果中，并集中保持两个原集合的相对顺序
		返回值为一个指向输出区间尾端的迭代器
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
		求两个有序set的差集，可重复，如果s1中x元素重复m次，s2中x元素重复m次，那么最终结果x元素的次数为max（m-n，0）
		输出结果中，并集中保持两个原集合的相对顺序
		返回值为一个指向输出区间尾端的迭代器
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
		求两个有序set的对称差集，可重复，如果s1中x元素重复m次，s2中x元素重复m次，那么最终结果x元素的次数为max（|m-n|，0）
		即在s1中出现但在s2中不出现，以及在s2中出现在s1中不出现的并集
		即s1，s2两个集合中都只出现了一次的元素，可重复
		输出结果中，并集中保持两个原集合的相对顺序
		返回值为一个指向输出区间尾端的迭代器
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
	// 计算[first, last)中元素值等于val的数量
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
	// 返回[first, last)中元素值等于val的第一个迭代器
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
		将[first, last)的元素重新排序，使的Pred为true在前半段，使得Pred为false的在后半段
		这个算法不保证元素的原始相对位置（比如说x1在x2前面，x1,x2两元素都从前半段移动到后半段
										不能保证x1还在x2的前面）
	
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
				else if (pred(*first))  // 满足不移动条件
					++first;
				else                  // 满足移动条件，*first将会被移动到后半段
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
		寻找序列[first2, last2)在序列[first1, last1)第一次出现的点
		返回第一次出现的点的末尾元素后一位置
	*/
	template<class ForwardIterator1, class ForwardIterator2>
	InputIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2)
	{
		int d1 = last1 - first1;
		int d2 = last2 - first2; 
		if (d1 < d2) return last1;  // 如果序列2比序列1都长，那不可能成为其子序列

		ForwardIterator1 cur1 = first1;
		ForwardIterator2 cur2 = first2;

		while (cur2 != last2)
		{
			if (*cur1 == *cur2)  // 如果当前元素相等。则比较下一元素
			{
				++cur1;
				++cur2;
			}
			else       // 如果当前元素不相等
			{
				if (d1 == d2)  // 并且d1，d2相等，那么就说明不可能成为其子序列了
					return last1;     // 如果没有找到，就返回序列1的last1
				else
				{
					cur1 = ++first1;
					cur2 = first2;       // 准备在新的起点再找一次，cur2必须要重新回到first2
					--d1;
				}
			}
		}
		return first1;    // 如果找到了，就返回序列1中第一次出现序列2的地方的首位置
	}














}   // namespace TinySTL

/****************************************[数值类算法]***********************************************/

#include "Detail\Numeric.h"

#endif  // _ALGORITHM_H_
