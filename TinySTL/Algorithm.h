#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

/*
*   Author  @linxiaoye
*/

#include <cstring>
#include <utility>
#include "Allocator.h"
// #include "Functional.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Utility.h"


namespace TinySTL 
{
	
	/********************[ fill_n ]***********************************/
	/********************[Algorithm Time Complexity: O(n)]************/
	
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
	
	/********************[ less ]***********************************/
	/********************[Algorithm Time Complexity: O(1)]************/	
	template<class T>
	bool less(const T& lhs, const T& rhs)
	{
		return lhs < rhs;
	}

	/********************[ copy ]***********************************/
	/********************[Algorithm Time Complexity: O(n)]************/
	template<>



	
}   // namespace TinySTL

#endif  // _ALGORITHM_H_
