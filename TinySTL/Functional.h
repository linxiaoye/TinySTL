#ifndef _FUNCTIONAL_H_
#define _FUNCTIONAL_H_

/*
*    Author  @yyehl
*/

/* 
*    �º�����Ҳ���Ǻ������� function object����Ҫ����STL�ĸ����㷨��
*    ��Ȼ����ָ��Ҳ��������Ҫ�󣬵��ǲ�������������
*    ���Ǿ��÷º���������STL���������
*    �º����ı�����ʵ����һ���࣬������ () �����������һ�����ƺ����Ķ���
*
*    �º����ķ��ࣺ����������������Ҫ��һԪ�º�������Ԫ�º�����
*                 �����ܷ�����Է�Ϊ �㷨���㣬��ϵ���㣬�߼�����
*/

namespace TinySTL
{
	/********************** һԪ�º��� ******************************/

	template<class arg, class ret>
	struct unary_function 
	{
		typedef arg  argument_type;
		typedef ret  result_type;
	};

	/********************** ��Ԫ�º��� ******************************/

	template<class arg1, class arg2, class ret>
	struct binary_function
	{
		typedef arg1 first_argument_type;
		typedef arg2 second_argument_type;
		typedef ret  result_type;
	};

	/********************** �㷨��º��� ******************************/
	// plus : �ӷ�����
	template<class T>
	struct plus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x + y; }
	};
	// minus : ��������
	template<class T>
	struct minus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x - y; }
	};
	// multiples : �˷�����
	template<class T>
	struct multiples : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x * y; }
	};
	// divide : ��������
	template<class T>
	struct divide : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x / y; }
	};
	// modulus : ȡ������
	template<class T>
	struct modulus : public binary_function<T, T, T>
	{
		T operator () (const T& x, const T& y) const { return x % y; }
	};
	// negate : ȡ������
	template<class T>
	struct negate : public unary_function<T, T>
	{
		T operator () (const T& x) { return -x; }
	};

	/********************** ��ϵ��º��� ******************************/
	//  ���
	template<class T>
	struct equal_to : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x == y; }
	};
	//  �����
	template<class T>
	struct not_equal_to : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x != y; }
	};
	//  ����
	template<class T>
	struct greater : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x > y; }
	};
	//  С��
	template<class T>
	struct less : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x < y; }
	};
	//  ���ڵ���
	template<class T>
	struct greater_equal : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x >= y; }
	};
	//  С�ڵ���
	template<class T>
	struct less_equal : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x <= y; }
	};

	/********************** �߼���º��� ******************************/
	//  �߼���
	template<class T>
	struct logical_and : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x && y; }
	};
	//  �߼���
	template<class T>
	struct logical_or : public binary_function<T, T, bool>
	{
		bool operator () (const T& x, const T& y) const { return x || y; }
	};
	//  �߼���
	template<class T>
	struct logical_not : public unary_function<T, bool>
	{
		bool operator () (const T& x, const T& y) const { return !x; }
	};


	// ֤ͬ�������κ�ֵͨ���˺�����ֵ�������κθı�
	template<class T>
	struct identity : public unary_function<T, T>
	{
		const T& operator () (const T& x) const { return x; }
	};

	// ȡ��һ��
	template<class T1, class T2>
	struct select1st : public binary_function<T1, T1, T1>
	{
		const T1& operator () (const pair<T1, T2>& x) const { return x.first; }
	};

	// ȡ�ڶ���
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
