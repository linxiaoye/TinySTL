#ifndef _HASH_FUNCTIONS_H_
#define _HASH_FUNCTIONS_H_

/*
*  Author   @yyehl
*/

/*
*     hash functions主要是为了给hashtable.h传递hashfcn默认参数时使用的，属于内部使用
*     hash functions 的功能是为了计算hashtable中元素的位置   
*     若需要处理下述类型以外的类型，则需要自定义hash function
*/

namespace TinySTL
{
	template<typename Key>
	struct hash { };

	// 对于字符串，设计了一个转换函数，对于其他的类型，只是返回其本身，其他什么也不做

	inline size_t hash_fcn_string(const char* s)
	{
		unsigned long h = 0;
		for (; *s; ++s)
		{
			h = 5 * h + *s;
		}
		return h;
	}

	// 模板特例化的格式为前面需要加 “ template<> ”，尖括号里面为空

	template<>
	struct hash<char*>
	{
		size_t operator()(const char* s) { return hash_fcn_string(s); }
	};
	template<>
	struct hash<const char*>
	{
		size_t operator()(const char* s) { return hash_fcn_string(s); }
	};
	template<>
	struct hash<char>
	{
		size_t operator()(char c) { return c; }
	};
	template<>
	struct hash<unsigned char>
	{
		size_t operator()(unsigned char c) { return c; }
	};
	template<>
	struct hash<signed char>
	{
		size_t operator()(unsigned char c) { return c; }
	};
	template<>
	struct hash<short>
	{
		size_t operator()(short x) { return x; }
	};
	template<>
	struct hash<unsignde short>
	{
		size_t operator()(unsigned short x) { return x; }
	};
	template<>
	struct hash<long>
	{
		size_t operator() (long x) { return x; }
	};
	template<>
	struct hash<unsigned long>
	{
		size_t operator()(unsigned long x) { return x; }
	};
	template<>
	struct hash<int>
	{
		size_t operator()(int x) { return x; }
	};
	template<>
	struct hash<unsigned int>
	{
		size_t operator()(unsigned int x) { return x; }
	};




}  // namespace TinySTL



#endif // !_HASH_FUNCTIONS_H_

