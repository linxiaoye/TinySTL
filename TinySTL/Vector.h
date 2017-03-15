#ifndef _VECTOR_H_
#define _VECTOR_H_

/*
*  Author  @linxiaoye
*/

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Uninitialized.h"
#include "Algorithm.h"

namespace TinySTL 
{
	template<class T, class Alloc = Allocator<T>>
	class vector 
	{
	private:   
	    /*  vector�����ݽṹ  */       
		T* start;             // Ŀǰ�ѱ�ռ�ÿռ����ʼλ�� 
		T* finish;            // Ŀǰ�ѱ�ռ�ÿռ��βԪ�غ�һλ 
		T* end_of_storage;    // Ŀǰ���ÿռ��βԪ�غ�һλ 
		
	public:
		typedef T           value_type;
		typedef T*          pointer;
		typedef T*          iterator;
		typedef const T*    const_iterator;
		typedef T&          reference;
		typedef const T&    const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
	
	public:
		/*   ���졢��ֵ��������غ���    */
		vector() : start(0), finish(0), end_of_storage(0) { }
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		vector(const vector& v);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);
		// vector(vector&& v);          // move����������ϸѧϰһ����д 
		vector& operator = (const vector& v);
		// vector& operator = (const vector&& v);
		~vector();
		
		/*    �Ƚϲ�����غ���        */
		bool operator == (const vector& v) const;
		bool operator != (const vector& v) const;
		
		/*    ������С��غ���       */
		different_type size() const { return finish - start; }
		disserent_type capacity() const { return end_of_storage - start; }
		bool empty() const { return start == finish; }
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		void shrink_to_fit();
		
		/*     ��������غ���       */
		
		/*     ����Ԫ����غ���     */  
		
		/*     �����޸���غ���     */
		
		/*     �����Ŀռ�������غ���     */ 
		
		
		
	};  //  end of class vector 

	
}   // namespace TinySTL

#endif   //  _VECTOR_H_
