#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

/*
*     Author @yyehl
*/

namespace TinySTL {
	
	namespace {
		template<bool, class T1, class T2>
		struct IfThenElse;
		template<class T1, class T2>
		struct IfThenElse<true, T1, T2> {
			using result = T1;
		};
		template<class T1, class T2>
		struct IfThenElse<false, T1, T2> {
			using result = T2;
		};
	}
	
	
	
	struct _true_type { };
	struct _false_type { };
	
	
	/*  为所有的内置类型先统一设置一个最保守的值  */
	template<class T> 
	struct _type_traits 
	{
		typedef _false_type    has_trivial_default_constructor;
		typedef _false_type    has_trivial_copy_constructor;
		typedef _false_type    has_trivial_assignment_operator;
		typedef _false_type    has_trivial_destructor;
		typedef _false_type    is_POD_type;
	};
	
	/*  为每个内置类型设置特化版本  */ 
	/* 以提升内置类型在构造、赋值、赋值、析构等的效率 */ 
	template<>
	struct _type_traits<bool> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<char> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<wchar_t> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<short> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned short> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<int> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned int> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<long> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<long long> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned long long> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<float> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<double> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<long double> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<class T>
	struct _type_traits<T*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<class T>
	struct _type_traits<const T*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<unsigned char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<signed char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<const char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<const unsigned char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	template<>
	struct _type_traits<const signed char*> 
	{
		typedef _true_type     has_trivial_default_constructor;
		typedef _true_type     has_trivial_copy_constructor;
		typedef _true_type     has_trivial_assignment_operator;
		typedef _true_type     has_trivial_destructor;
		typedef _true_type     is_POD_type;
	};
	
}  //  namespace TinySTL

#endif  // _TYPE_TRAITS_H_
