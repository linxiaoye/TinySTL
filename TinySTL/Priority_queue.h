#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

/*
*   Author @yyehl
*/

#include "Vector.h"
#include "Algorithm.h"


namespace TinySTL
{
	template<class T, class Sequence, class Compare>
	bool operator == (const priority_queue<T, Sequence, Compare>& lhs, const priority_queue<T, Sequence, Compare>& rhs)
	{
		return lhs.c == rhs.c;
	}
	template<class T, class Sequence, class Compare>
	bool operator != (const priority_queue<T, Sequence, Compare>& lhs, const priority_queue<T, Sequence, Compare>& rhs)
	{
		return lhs.c != rhs.c;
	}
	template<class T, class Sequence, class Compare>
	void swap(priority_queue<T, Sequence, Compare>& lhs, priority_queue& rhs)
	{
		swap(lhs.c, rhs.c);
	}

	template<class T, class Sequence = vector<T>, class Compare = less<typename Sequence::value_type>>
	class priority_queue
	{
	public:
		friend bool operator == (const priority_queue& lhs, const priority_queue& rhs);
		friend bool operator != (const priority_queue& lhs, const priority_queue& rhs);
		friend void swap(priority_queue& lhs, priority_queue& rhs);

		typedef typename Sequence::value_type            value_type;
		typedef typename Sequence::size_type             size_type;
		typedef typename Sequence::reference             reference;
		typedef typename Sequence::const_reference       const_reference;
		typedef typename Sequence::difference_type       difference_type;

	private:
		Sequence c;

	public:
		priority_queue() : c() { }
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) : c(first, last)
		{
			make_heap(c.begin(), c.end());
		}
		priority_queue(const priority_queue& x) : c(x.c) { }

		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		const_reference top() const { return c.front(); }

		void push(const value_type& val)
		{
			c.push_back(val);
			up_heap(c.begin(), c.end() - 1, c.begin());
		}
		void pop()
		{
			pop_heap(c.begin(), c.end() - 1, c.begin());
			c.pop_back();
		}

		bool operator == (const priority_queue& x) const { return c == x.c; }
		bool operator != (const priority_queue& x) const { return c != x.c; }
		void swap(priority_queue& x)
		{
			swap(c, x.c);
		}


	};   // end of class priority_queue



}  // namespace TinySTL




#endif // !_PRIORITY_QUEUE_H_

