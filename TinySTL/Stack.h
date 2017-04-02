#ifndef _STACK_H_
#define _STACK_H_

/*
*   Author @yyehl
*/

namespace TinySTL
{
	/*友元函数定义*/
	template<class T, class sequence>
	bool operator == (const stack<T, sequence>& lhs, const stack<T, sequence>& rhs) { return lhs.c == rhs.c; }
	template<class T, class sequence>
	bool operator != (const stack<T, sequence>& lhs, const stack<T, sequence>& rhs) { return lhs.c != rhs.c; }
	template<class T, class sequence>
	void swap(stack<T, sequence>& lhs, stack<T, sequence>& rhs) { return swap(lhs.c, rhs.c); }

	template<class T, class sequence = deque<T>>
	class stack
	{
	public:  
		friend bool operator == (const stack<T>& lhs, const stack<T>& rhs);
		friend bool operator != (const stack<T>& lhs, const stack<T>& rhs);

		typedef typename sequence::value_type       value_type;
		typedef typename sequence::size_type        size_type;
		typedef typename sequence::reference        reference;
		typedef typename sequence::const_reference  const_reference;
		// stack不需要迭代器，因为不允许遍历

	private:
		sequence c;

	public:
		stack() : c(sequence()) { }
		stack(size_type n, const value_type& val) : c(sequence(n, val)) { }
		stack(const stack& x) : c(sequence(x.c) { }
		~stack() { c.~sequence(); }
		stack& operator = (const stack& x) { c = x.c; return *this; }

		bool operator == (const stack& x) const { return c == x.c; }
		bool operator != (const stack& x) const { return c != x.c; }

		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference top() { return c.front(); }
		const_reference top() const { return c.front(); }
		
		void push(const value_type& val) { c.push_back(val); }
		void pop() { c.pop_back(); }

	};


}  //  namespace TinySTL
#endif // !_STACK_H_

