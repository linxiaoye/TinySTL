#ifndef _VECTOR_H_
#define _VECTOR_H_

/*
*  Author  @yyehl
*/
#include <type_traits>

#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
#include "Algorithm.h"
#include "Uninitialized.h"


namespace TinySTL 
{
	template<class T, class Alloc>
	class vector;
	
	template<class T, class Alloc = allocator<T>>
	bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
	template<class T, class Alloc = allocator<T>>
	bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
	
	
	template<class T, class Alloc = allocator<T>>
	class vector 
	{
	public:                                       //  ��Ԫ�������� 
		friend bool operator == <T, Alloc>(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
		friend bool operator != <T, Alloc>(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
		
	private:   
	    /*************vector�����ݽṹ*************************/       
		T* _start;             // Ŀǰ�ѱ�ռ�ÿռ����ʼλ�� 
		T* _finish;            // Ŀǰ�ѱ�ռ�ÿռ��βԪ�غ�һλ 
		T* _end_of_storage;    // Ŀǰ���ÿռ��βԪ�غ�һλ 
		
	public:
		typedef T           value_type;
		typedef T*          pointer;
		typedef T*          iterator;
		typedef const T*    const_iterator;
		typedef T&          reference;
		typedef const T&    const_reference;
		typedef size_t      size_type;
		typedef ptrdiff_t   difference_type;
		
		typedef Alloc       dataAllocator;
	
	public:
		/***************���졢��ֵ��������غ���**********************/
		vector() : _start(0), _finish(0), _end_of_storage(0) { }
		explicit vector(const size_type n);
		vector(const size_type n, const value_type& value);
		vector(const vector& v);
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);   
		vector(vector&& v);
		vector& operator = (const vector& v);
		vector& operator = (const vector&& v);
		~vector();
		
		/*****************�Ƚϲ�����غ��� ***************************/ 
		bool operator == (const vector& v) const;
		bool operator != (const vector& v) const;
		

		
		/*****************������С��غ��� **************************/
		size_type size() const { return _finish - _start; }
		size_type capacity() const { return _end_of_storage - _start; }
		bool empty() const { return _start == _finish; }
		void resize(size_type n, value_type val = value_type());
		void reserve(size_type n);
		void shrink_to_fit();
		
		/******************��������غ���****************************/
		iterator begin() { return _start; }
		const_iterator begin() const { return _start; }
		const_iterator cbegin() const { return _start; }
		iterator end() { return _finish; }
		const_iterator end() const { return _finish; }
		const_iterator cend() const { return _finish; }
			
		/****************����Ԫ����غ���*****************************/  
		reference front() { return *(begin()); }
		reference back() { return *(end() - 1); }
		reference operator [] (const difference_type i) { return *(begin() + i); }
		const_reference operator [] (const difference_type i) const { return *(cbegin() + i); }
		
		/******************�����޸���غ���****************************/
		void push_back(const value_type& val);
		iterator insert(iterator pos, const value_type& val);
		void insert(iterator pos, const size_type n, const value_type& val);
		template<class InputIterator>
		void insert(iterator pos, InputIterator first, InputIterator last);
		void pop_back();
		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		void swap(vector& v);
		
		/******************�����ռ�������غ���*******************/ 

	
	private:
		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);  
		template<class InputIterator>  
		void vector_aux(InputIterator size, InputIterator val, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);
		void allocate_and_fill_n(const size_type size, const value_type& val);
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		void destroy_and_deallocate_all();
		size_type get_new_capacity()
		{
			return (capacity() == 0) ? 1 : 2 * capacity(); 
		}
				
	};  //  end of class vector 

	
}   // namespace TinySTL

#include "Detail\Vector.impl.h"          // ����ʵ�� 

#endif   //  _VECTOR_H_
