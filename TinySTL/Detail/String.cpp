/*
*	Author @yyehl
*/

#include "../String.h"
namespace TinySTL
{
	string::string(const string& str)
	{
		allocate_and_copy(str.begin(), str.end());
	}
	string::string(string&& str)
	{
		_start = str._start;
		_finish = str._finish;
		_end_of_storage = str._end_of_storage;
	}
	string::string(const string& str, size_type pos, size_type len)
	{
		allocate_and_copy(str.begin() + pos, str.begin() + pos + len);
	}
	string::string(const char* s)
	{
		allocate_and_copy(s, s + strlen(s));
	}
	string::string(const char* s, size_type n)
	{
		char* c = data_allocator::allocate(n * strlen(s));
		_start = c;
		for (size_type i = 0; i < n; ++i)
		{
			for (size_type j = 0; j < strlen(s); ++j)
			{
				*(c + i * strlen(s) + j) = *(s + j);
			}
		}
		_finish = _end_of_storage = _start + n * strlen(s);
	}
	string::string(size_type n, char c)
	{
		_start = data_allocator::allocate(n);
		char* tmp = _start;
		while (n > 0)
		{
			*tmp = c;
			++tmp;
			--n;
		}
		_finish = _end_of_storage = _start + n;
	}
	template<class InputIterator>
	string::string(InputIterator first, InputIterator last)
	{
		string_aux(first, last, typename std::is_integral<InputIterator>::type());
	}
	template<class InputIterator>
	void string::string_aux(InputIterator first, InputIterator last, std::true_type)
	{
		string(size_type(first), char(last));
	}
	template<class InputIterator>
	void string::string_aux(InputIterator first, InputIterator last, std::false_type)
	{
		allocate_and_copy(first, last);
	}

	string::~string()
	{
		deallocate_and_destroy();
	}

	string& string::operator= (const string& str)
	{
		clear();
		if (capacity() < str.size())
			resize(str.size());
		TinySTL::copy(str.begin(), str.end(), begin());
		_finish = _start + str.size();
		return *this;
	}
	string& string::operator= (string&& str)
	{
		_start = str._start;
		_finish = str._finish;
		_end_of_storage = str._end_of_storage;
		return *this;
	}
	string& string::operator= (const char* s)
	{
		clear();  // 不回收空间
		if (capacity() < strlen(s))
			resize(strlen(s));
		char* tmp = _start;
		for (size_type i = 0; i < strlen(s); ++i)
		{
			*tmp = *(s + i);
			++tmp;
		}
		return *this;
	}
	string& string::operator= (char c)
	{
		clear();
		if (capacity() < 1)
			resize(sizeof(c));
		*_start = c;
		_finish = _start + 1;
		return *this;
	}

	string& string::operator+=(const string& str)
	{
		if (capacity() - size() < str.size())
			resize(size() + str.size());
		char* tmp = _finish;
		for (size_type i = 0; i < str.size(); ++i)
		{
			*tmp = *(str.begin() + i);
			++tmp;
		}
		_finish = tmp;
		return *this;
	}

	string& string::operator+=(const char* s)
	{
		if (capacity() - size() < strlen(s))
			resize(size() + strlen(s));
		char* tmp = _finish;
		for (size_type i = 0; i < strlen(s); ++i)
		{
			*tmp = *(s + i);
			++tmp;
		}
		_finish = tmp;
		return *this;
	}

	string& string::operator+=(char c)
	{
		if (capacity() - size() < sizeof(c))
			resize(size() + sizeof(c));
		*_finish = c;
		++_finish;
		return *this;
	}


	string& string::append(const string& str)
	{
		(*this) += str;
		return *this;
	}
	string& string::append(const string& str, size_type subpos, size_type len)
	{
		string tmp(str.begin() + subpos, str.begin() + subpos + len);
		(*this) += tmp;
		return *this;
	}
	string& string::append(const char* s)
	{
		(*this) += s;
		return *this;
	}
	string& string::append(const char* s, size_type n)
	{
		string tmp(s, n);
		(*this) += tmp;
		return *this;
	}
	string& string::append(size_type n, char c)
	{
		string tmp(n, c);
		(*this) += tmp;
		return *this;
	}

	string& string::insert(size_type pos, const string& str)
	{
		if (capacity() < size() + str.size())
			resize(size() + str.size());
		copy_backward(begin() + pos, end(), end() + str.size());
		copy(str.begin(), str.end(), begin() + pos);
		_finish = _finish + str.size();
		return *this;
	}
	string& string::insert(size_type pos, const string& str, size_type subpos, size_type sublen)
	{
		string tmp(str.begin() + subpos, str.begin() + subpos + sublen);
		insert(pos, tmp);
		return *this;
	}

	string& string::insert(size_type pos, const char* c)
	{
		string tmp(c);
		insert(pos, tmp);
		return *this;
	}
	string& string::insert(size_type pos, const char* c, size_type n)
	{
		string tmp(c, n);
		insert(pos, tmp);
		return *this;
	}
	string& string::insert(size_type pos, size_type n, char c)
	{
		string tmp(n, c);
		insert(pos, tmp);
		return *this;
	}

	template<class InputIterator>
	typename string::iterator string::insert(iterator pos, InputIterator first, InputIterator last)
	{
		return insert_aux(pos, first, last, typename std::is_integral<InpurIterator>::type());
	}
	template<class InputIterator>
	typename string::iterator string::insert_aux(iterator pos, InputIterator first, InputIterator last, std::true_type)
	{
		insert(pos, size_type(first), last);
	}
	template<class InputIterator>
	typename string::iterator string::insert_aux(iterator pos, InputIterator first, InputIterator last, std::false_type)
	{
		int i = pos - first;
		string tmp(first, last);
		insert(i, tmp);
		return begin() + i + (last - first);
	}

	string& string::erase(size_type pos, size_type len)
	{
		copy(begin() + pos + len, end(), begin() + pos);
		_finish = _finish - len;
		return *this;
	}

	typename string::iterator string::erase(iterator pos)
	{
		size_type len = pos - _start;
		erase(len, 1);
		return begin() + len;
	}

	typename string::iterator string::erase(iterator first, iterator last)
	{
		size_t len = first - _start;
		erase(first - begin(), last - first);
		return begin() + len;
	}

	void string::swap(string& str)
	{
		char* tmp1 = _start;
		char* tmp2 = _finish;
		char* tmp3 = _end_of_storage;
		_start = str._start;
		_finish = str._finish;
		_end_of_storage = str._end_of_storage;
		str._start = tmp1;
		str._finish = tmp2;
		str._end_of_storage = tmp3;
	}


	void string::clear()
	{
		data_allocator::destroy(_start, _finish);
		_finish = _start;
	}

	void string::resize(size_type n)
	{
		size_type new_capacity = max(2 * size(), n);
		char* new_start = data_allocator::allocate(new_capacity);
		char* tmp = new_start;
		for (char* i = _start; i != _finish; ++i)
		{
			*tmp = *i;
			++tmp;
		}
		deallocate_and_destroy();
		_start = new_start;
		_finish = tmp;
		_end_of_storage = _start + new_capacity;
	}

	void string::reserve(size_type n)
	{
		if (n > capacity())
		{
			resize(n);
		}
		else if (n > size())
		{
			data_allocator::deallocate(_start + n, capacity() - n);
			_end_of_storage = _start + n;
		}
		else
		{
			data_allocator::destroy(_start + n, _start + size());
			data_allocator::deallocate(_start + n, capacity() - n);
			_finish = _start + n;
			_end_of_storage = _start + n;
		}
	}

	std::ostream& operator << (std::ostream& os, const string& str)
	{
		for (auto c : str)
			os << c;

		return os;
	}

	std::istream& operator >> (std::istream& is, string& str)
	{
		char ch;
		string::size_type old_size = str.size(), index = 0;
		bool has_prev_black = false;
		while (is.get(ch))    // 跳过前面的空格和换行
		{
			if (ch == ' ' || ch == '\n')
				has_prev_black = true;
			else
				break;
		}
		is.putback(ch);
		str.clear();
		while (is.get(ch))
		{
			if (ch != EOF && ch != ' ' && ch != '\n')
				str.push_back(ch);
			else
				break;
		}
		return is;
	}

	std::istream& getline(std::istream& is, string& str)
	{
		return getline(is, str, '\n');
	}
	std::istream& getline(std::istream& is, string& str, char d)
	{
		char c;
		while (is.get(c))
		{
			if (c != d)
				str.push_back(c);
			else
				break;
		}
		return is;
	}
	string operator+ (const string& lhs, const string& rhs)
	{
		string tmp(lhs);
		tmp.operator+=(rhs);
		return tmp;
	}
	string operator+ (const string& lhs, const char* rhs)
	{
		string tmp(lhs);
		tmp.operator+=(rhs);
		return tmp;
	}
	string operator+ (const char* lhs, const string& rhs)
	{
		string tmp(lhs);
		tmp.operator+=(rhs);
		return tmp;
	}
	string operator+ (const string& lhs, char rhs)
	{
		string tmp(lhs);
		tmp.operator+=(rhs);
		return tmp;
	}
	string operator+ (char lhs, const string& rhs)
	{
		string tmp(1, lhs);
		tmp.operator+=(rhs);
		return tmp;
	}
	bool operator== (const string& lhs, const string& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); ++i)
		{
			if (*(lhs.begin() + i) != *(rhs.begin() + i))
				return false;
		}
		return true;
	}
	bool operator== (const string& lhs, const char* rhs)
	{
		string tmp(rhs);
		return lhs == tmp;
	}
	bool operator== (const char* lhs, const string& rhs)
	{
		string tmp(lhs);
		return tmp == rhs;
	}
	bool operator!= (const string& lhs, const string& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator!= (const string& lhs, const char* rhs)
	{
		string tmp(rhs);
		return !(lhs == tmp);
	}
	bool operator!= (const char* lhs, const string& rhs)
	{
		string tmp(lhs);
		return !(tmp == rhs);
	}
	bool operator< (const string& lhs, const string& rhs)
	{
		string::const_iterator iter1 = lhs.begin(), iter2 = rhs.begin();
		for (; iter1 != lhs.end() && iter2 != rhs.end(); ++iter1, ++iter2)
		{
			if (*iter2 < *iter1)
				return false;
			if (*iter1 < *iter2)
				return true;
		}
		if (iter1 == lhs.end() && iter2 != rhs.end())
			return true;
		else
			return false;
	}
	bool operator< (const string& lhs, const char* rhs)
	{
		string tmp(rhs);
		return lhs < tmp;
	}
	bool operator< (const char* lhs, const string& rhs)
	{
		string tmp(lhs);
		return tmp < rhs;
	}
	bool operator<= (const string& lhs, const string& rhs)
	{
		string::const_iterator iter1 = lhs.begin(), iter2 = rhs.begin();
		for (; iter1 != lhs.end() && iter2 != rhs.end(); ++iter1, ++iter2)
		{
			if (*iter2 < *iter1)
				return false;
			if (*iter1 < *iter2)
				return true;
		}
		if ((iter1 == lhs.end() && iter2 != rhs.end()) || (iter1 == lhs.end() && iter2 == rhs.end()))
			return true;
		else
			return false;
	}
	bool operator<= (const string& lhs, const char* rhs)
	{
		string tmp(rhs);
		return lhs <= tmp;
	}
	bool operator<= (const char* lhs, const string& rhs)
	{
		string tmp(lhs);
		return tmp <= rhs;
	}
	bool operator> (const string& lhs, const string& rhs)
	{
		return !(lhs <= rhs);
	}
	bool operator> (const string& lhs, const char* rhs)
	{
		return !(lhs <= rhs);
	}
	bool operator> (const char* lhs, const string& rhs)
	{
		return !(lhs <= rhs);
	}
	bool operator>= (const string& lhs, const string& rhs)
	{
		return !(lhs < rhs);
	}
	bool operator>= (const string& lhs, const char* rhs)
	{
		return !(lhs < rhs);
	}
	bool operator>= (const char* lhs, const string& rhs)
	{
		return !(lhs < rhs);
	}

	void string::shink_to_fit()
	{
		data_allocator::deallocate(_start + size(), capacity() - size());
		_end_of_storage = _finish;
	}

	void string::deallocate_and_destroy()
	{
		data_allocator::destroy(_start, _finish);
		data_allocator::deallocate(_start, _end_of_storage - _start);
		_end_of_storage = _finish = _start = nullptr;
	}

	template<class InputIterator>
	void string::allocate_and_copy(InputIterator first, InputIterator last)
	{
		size_type len = last - first;
		char* c = data_allocator::allocate(len);
		_start = c;
		for (auto i = first; i != last; ++i)
		{
			*c = *i;
			++c;
		}
		_finish = _end_of_storage = _start + len;

	}



}  // namespace TinySTL