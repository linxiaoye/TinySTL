#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_

/*
*    Author  @yyehl
*/

/* 
*    仿函数，也就是函数对象 function object，主要用于STL的各种算法中
*    虽然函数指针也可以满足要求，但是不够灵活，不够抽象
*    于是就用仿函数来搭配STL的其他组件
*    仿函数的本质其实就是一个类，重载了 () 运算符，创建一个类似函数的对象
*
*    仿函数的分类：按操作操作个数主要有一元仿函数，二元仿函数，
*                 按功能分类可以分为 算法运算，关系运算，逻辑运算
*/

namespace TinySTL
{
	/********************** 一元仿函数 ******************************/

	template<class arg, class ret>
	struct unary_function 
	{
		typedef arg  argument_type;
		typedef ret  result_type;
	};

	/********************** 二元仿函数 ******************************/

	template<class arg1, class arg2, class ret>
	struct binary_function
	{
		typedef arg1 first_argument_type;
		typedef arg2 second_argument_type;
		typedef ret  result_type;
	};

	/********************** 算法类仿函数 ******************************/
	// plus : 加法运算
	template<class T>
	struct plus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x + y; }
	};
	// minus : 减法运算
	template<class T>
	struct minus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x - y; }
	};
	// multiples : 乘法运算
	template<class T>
	struct multiples : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x * y; }
	};
	// divide : 除法运算
	template<class T>
	struct divide : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x / y; }
	};
	// modulus : 取余运算
	template<class T>
	struct modulus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x % y; }
	};
	// negate : 取反运算
	template<class T>
	struct negate : public unary_function<T, T>
	{
		T operator () (const T& x) { return -x; }
	};

	/********************** 关系类仿函数 ******************************/
	//  相等
	template<class T>
	struct equal_to : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x == y; }
	};
	//  不相等
	template<class T>
	struct not_equal_to : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x != y; }
	};
	//  大于
	template<class T>
	struct greater : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x > y; }
	};
	//  小于
	template<class T>
	struct less : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x < y; }
	};
	//  大于等于
	template<class T>
	struct greater_equal : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x >= y; }
	};
	//  小于等于
	template<class T>
	struct less_equal : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x <= y; }
	};

	/********************** 逻辑类仿函数 ******************************/
	//  逻辑与
	template<class T>
	struct logical_and : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x && y; }
	};
	//  逻辑或
	template<class T>
	struct logical_or : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x || y; }
	};
	//  逻辑非
	template<class T>
	struct logical_not : public unary_function<T, bool>
	{
		bool operator () (const T& x, const T& y) const { return !x; }
	};


	// 证同函数，任何值通过此函数，值不会有任何改变
	template<class T>
	struct identity : public unary_function<T, T>
	{
		const T& operator () (const T& x) const { return x; }
	};

	// 取第一个
	template<class T1, class T2>
	struct select1st : public binary_function<T1, T1, T1>
	{
		const T1& operator () (const pair<T1, T2>& x) const { return x.first; }
	};

	// 取第二个
	template<class T1, class T2>
	struct select2st : public binary_function<T1, T2, T2>
	{
		const T2& operator () (const pair<T1, T2>& x) const { return x.second; }
	};

	//
	//
	//
	//
	//




}   // namespace TinySTL


#endif   //  !_FUNCTIONAL_H_
