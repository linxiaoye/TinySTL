#ifndef _HASH_FUNCTIONS_H_
#define _HASH_FUNCTIONS_H_

/*
*  Author   @yyehl
*/

/*
*     hash functions��Ҫ��Ϊ�˸�hashtable.h����hashfcnĬ�ϲ���ʱʹ�õģ������ڲ�ʹ��
*     hash functions �Ĺ�����Ϊ�˼���hashtable��Ԫ�ص�λ��   
*     ����Ҫ��������������������ͣ�����Ҫ�Զ���hash function
*/

namespace TinySTL
{
	template<typename Key>
	struct hash { };

	// �����ַ����������һ��ת���������������������ͣ�ֻ�Ƿ����䱾������ʲôҲ����

	inline size_t hash_fcn_string(const char* s)
	{
		unsigned long h = 0;
		for (; *s; ++s)
		{
			h = 5 * h + *s;
		}
		return h;
	}

	// ģ���������ĸ�ʽΪǰ����Ҫ�� �� template<> ��������������Ϊ��

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

