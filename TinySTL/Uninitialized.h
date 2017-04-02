#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

/*
*    Author  @yyehl
*/

#include "Iterator.h"
#include "Construct.h"
#include "TypeTraits.h"
#include "Algorithm.h" 


namespace TinySTL 
{
	
	/******      uninitialized_copy      ******/
	
	template<class InputIterator, class ForwardIterator>  // ����value_type()�õ���������value_type 
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) 
	{
		return _uninitialized_copy(first, last, result, value_type(result));
	} 
	
	template<class InputIterator, class ForwardIterator, class T>  // ����_type_traits�жϵ�������ָ�������������ͻ��������ͣ���ѡ��Ч����ߵĴ���ʽ 
	ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T* p) 
	{
		typedef typename _type_traits<T>::is_POD_type  is_POD;
		return _uninitialized_copy_aux(first, last, result, is_POD());
	}
	
	template<class InputIterator, class ForwardIterator>  // ����������ָ�����������ͣ���ֱ�ӵ���copy���� 
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type)
	{
		return copy(first, last, result);
	}
	template<class InputIterator, class ForwardIterator>  // ����������ָ���Ƿ��������ͣ������һ�����Ĺ��� 
	ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type)
	{
		ForwardIterator ret = result;
		for ( ; first != last; ++first, ++ret) 
			construct(&*ret, *first);
		return ret;
	}
	
	/******      uninitialized_fill      ******/	
	
	template<class ForwardIterator, class T>   // ����value_type()�õ���������value_type 
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last,const T& x) 
	{
		_uninitialized_fill(first, last, x, value_type(first)); 
	}
	
	template<class ForwardIterator, class T, class T1>   // ����_type_traits�жϵ�������ָ�������������ͻ��������ͣ���ѡ��Ч����ߵĴ���ʽ 
	inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1* t)
	{
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		_uninitialized_fill_aux(first, last, x, is_POD());
	}
	
	template<class ForwardIterator, class T>   // ����������ָ�����������ͣ���ֱ�ӵ���fill����
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _true_type) 
	{
		fill(first, last, x);
	}
	template<class ForwardIterator, class T>    // ����������ָ���Ƿ��������ͣ������һ�����Ĺ��� 
	inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _false_type)
	{
		ForwardIterator ret = first;
		for ( ; first != last; ++first, ++ret)
			construct(&*ret, x);
	}
	
	/******      uninitialized_fill_n      ******/            // ע��ͬ�� 
	template<class ForwardIterator, class size, class T, class T1>
	ForwardIterator _uninitialized_fill_n(ForwardIterator first, size n, const T& x, T1* t);
	template<class ForwardIterator, class size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& x, _true_type);
	template<class ForwardIterator, class size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& x, _false_type);
	
	template<class ForwardIterator, class size, class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, size n, const T& x) 
	{
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}
	
	template<class ForwardIterator, class size, class T, class T1>
	ForwardIterator _uninitialized_fill_n(ForwardIterator first, size n, const T& x, T1* t) 
	{
		typedef typename _type_traits<T1>::is_POD_type is_POD;
		return _uninitialized_fill_n_aux(first, n, x, is_POD());
	}
	
	template<class ForwardIterator, class size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& x, _true_type)
	{
		return fill_n(first, n, x);
	}
	template<class ForwardIterator, class size, class T>
	ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, size n, const T& x, _false_type) 
	{
		ForwardIterator ret;
		for ( ; n > 0; --n, ++ret) 
			construct(&*ret, x);
		return ret;
	}
	
}   // nemaspace TinySTL

#endif   // _UNINITIALIZED_H_
