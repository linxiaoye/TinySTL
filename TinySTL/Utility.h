#ifndef _UTILITY_H_
#define _UTILITY_H_

/*
*  Author   @yyehl
*/

namespace TinySTL
{
	/************************[ swap ]*********************************/
	template<class T>
	void swap(T& lhs, T& rhs)
	{
		T tmp(lhs);
		lhs = rhs;
		rhs = tmp;
	}

	/************************[ pair ]*********************************/
	template<class T1, class T2>
	struct pair
	{
		typedef T1   first_type;
		typedef T2   second_type;

		T1 first;
		T2 second;

		pair() : first(first_type()), second(second_type()) { }
		pair(const T1& x, const T2& y) : first(x), second(y) { }
	};
	
	
}    // namespace TinySTL

#endif  // _UTILITY_H_

