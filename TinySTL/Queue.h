#ifndef _QUEUE_H_
#define _QUEUE_H_

/*
*   Author  @yyehl
*/

#include "Deque.h"

namespace TinySTL
{

	template<class T, class sequence = deque<T>>
	bool operator == (const queue<T, sequence>& lhs, const queue<T, sequence>& rhs) { return lhs.c == rhs.c; }
	template<class T, class sequence = deque<T>>
	bool operator != (const queue<T, sequence>& lhs, const queue<T, sequence>& rhs) { return lhs.c != rhs.c; }
	template<class T, class sequence = deque<T>>
	void swap(queue<T, sequence>& lhs, queue<T, sequence>& rhs) { swap(lhs.c, rhs.c); }

	template<class T, class sequence = deque<T>>
	class queue
	{
	public:
		friend bool operator == (const queue& lhs, const queue& rhs);
		friend bool operator != (const queue& lhs, const queue& rhs);
		friend void swap();

		typedef typename sequence::value_type          value_type;
		typedef typename sequence::size_type           size_type;
		typedef typename sequence::reference           reference;
		typedef typename sequence::const_reference     const_reference;
		// queue不需要迭代器，因为不允许遍历

	private:
		sequence c;

	public:
		queue() : c(sequence()) { }
		queue(size_type n, const value_type& val) : c(sequence(n, val)) { }
		queue(const queue& x) : c(sequence(x.c)) { }
		~queue() { c.~sequence(); }
		queue& operator = (const queue& x) { c = x.c; return *this; }

		bool operator == (const queue& x) const { return c == x.c; }
		bool operator != (const queue& x) const { return c != x.c; }

		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { const c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }

		void push(const value_type& val) { c.push_back(val); }
		void pop() { c.pop_front(); }

	};


}  // namespace TinySTL

#endif // !_QUEUE_H_
