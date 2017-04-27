#ifndef _STRING_H_
#define _STRING_H_

/*
*	Author  @yyehl
*/


#include "Allocator.h"
#include "Construct.h"
#include "Iterator.h"
#include <cstring>
#include "Algorithm.h"
#include <iostream>



namespace TinySTL
{
	class string
	{
	public:
		typedef char          value_type;
		typedef char*         iterator;
		typedef const char*   const_iterator;
		typedef char&         reference;
		typedef const char&   const_reference;
		typedef size_t        size_type;
		typedef ptrdiff_t     difference_type;
	private:
		typedef TinySTL::allocator<char>  data_allocator;
		static const size_type npos = -1;   // max value of size_type

	private:
		char* _start;
		char* _finish;
		char* _end_of_storage;

	public:
		string() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) { }
		string(const string& str);
		string(string&& str);   // 
		string(const string& str, size_type pos, size_type len);
		string(const char* s);
		string(const char* s, size_type n);
		string(size_type n, char c);
		template<class InputIterator>
		string(InputIterator first, InputIterator last);

		~string();

		string& operator= (const string& str);
		string& operator= (string&& str);
		string& operator= (const char* s);
		string& operator= (char c);

		char& operator[] (size_type n) { return *(_start + n); }
		const char& operator[] (size_type n) const { return *(_start + n); }

		string& operator+= (const string& str);
		string& operator+= (const char* s);
		string& operator+= (char c);

		iterator begin() { return _start; }
		iterator end() { return _finish; }
		const_iterator begin() const { return _start; }
		const_iterator end() const { return _finish; }
		const_iterator cbegin() const { return _start; }
		const_iterator cend() const { return _finish; }

		char& front() { return *_start; }
		char& back() { return *(_finish - 1); }
		const char& front() const { return *_start; }
		const char& back() const { return *(_finish - 1); }

		size_type size() const { return _finish - _start; }
		size_type length() const { return size(); }
		size_type capacity() const { return _end_of_storage - _start; }
		bool empty() const { return size() == 0; }

		void push_back(char c) { insert(size(), 1, c); }

		string& insert(size_type pos, const string& str);
		string& insert(size_type pos, const string& str, size_type subpos, size_type sublen);
		string& insert(size_type pos, const char* c);
		string& insert(size_type pos, const char* c, size_type n);
		string& insert(size_type pos, size_type n, char c);
		template<class InputIterator>
		iterator insert(iterator pos, InputIterator first, InputIterator last);

		string& append(const string& str);
		string& append(const string& str, size_type subpos, size_type sublen);
		string& append(const char* s);
		string& append(const char* s, size_type n);
		string& append(size_type n, char c);

		void pop_back() { erase(end() - 1); }

		string& erase(size_type pos, size_type len);
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);

		void swap(string& str);

		void clear();
		void resize(size_type n);
		void reserve(size_type n);
		void shink_to_fit();

	public:
		friend std::ostream& operator << (std::ostream& os, const string& str);
		friend std::istream& operator >> (std::istream& is, string& str);
		friend std::istream& getline(std::istream& is, string& str);
		friend std::istream& getline(std::istream& is, string& str, char c);
		friend string operator+ (const string& lhs, const string& rhs);
		friend string operator+ (const string& lhs, const char* rhs);
		friend string operator+ (const char* lhs, const string& rhs);
		friend string operator+ (const string& lhs, char rhs);
		friend string operator+ (char lhs, const string& rhs);
		friend bool operator== (const string& lhs, const string& rhs);
		friend bool operator== (const string& lhs, const char* rhs);
		friend bool operator== (const char* lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const string& rhs);
		friend bool operator!= (const string& lhs, const char* rhs);
		friend bool operator!= (const char* lhs, const string& rhs);
		friend bool operator< (const string& lhs, const string& rhs);
		friend bool operator< (const string& lhs, const char* rhs);
		friend bool operator< (const char* lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const string& rhs);
		friend bool operator<= (const string& lhs, const char* rhs);
		friend bool operator<= (const char* lhs, const string& rhs);
		friend bool operator> (const string& lhs, const string& rhs);
		friend bool operator> (const string& lhs, const char* rhs);
		friend bool operator> (const char* lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const string& rhs);
		friend bool operator>= (const string& lhs, const char* rhs);
		friend bool operator>= (const char* lhs, const string& rhs);

	private:
		template<class InputIterator>
		void string_aux(InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		void string_aux(InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		iterator insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type);
		template<class InputIterator>
		iterator insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type);
		template<class InputIterator>
		void allocate_and_copy(InputIterator first, InputIterator last);
		void deallocate_and_destroy();

};




}  // namespace TinySTL


#endif // !_STRING_H_
