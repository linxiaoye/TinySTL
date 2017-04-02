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
	
	
	
}    // namespace TinySTL

#endif  // _UTILITY_H_

