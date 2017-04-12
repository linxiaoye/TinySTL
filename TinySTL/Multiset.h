#ifndef _MULTISET_H_
#define _MULTISET_H_

/*
*    Autor  @yyehl
*/

#include "RB_tree.h"
#include "Functional.h"

// 性质： set中key即是value，set已排序，set不允许重复，set不允许通过迭代器修改值

namespace TinySTL
{
	template<class Key, class Compare = less<Key>>
	class multiset
	{
	public:
		typedef Key      key_type;           // 在set中，key即是value
		typedef Key      value_type;
		typedef Compare  key_compare;
		typedef Compare  value_compare;

	private:
		typedef rb_tree<Key, Key, identity<Key>, Compare>     rep_type;

		rep_type t;   // set的数据结构，内置的rb_tree
	public:
		typedef typename rep_type::const_iterator     iterator;
		typedef typename rep_type::const_iterator     const_iterator;
		typedef typename rep_type::const_pointer      pointer;
		typedef typename rep_type::const_pointer      const_pointer;
		typedef typename rep_type::const_reference    cosnt_reference;
		typedef typename rep_type::const_reference    reference;

		typedef typename rep_type::size_type          size_type;
		typedef typename rep_type::difference_type    difference_type;
	private:
		typedef typename rep_type::iterator           rep_iterator;

	public:
		multiset() : t(Compare()) { }
		explicit multiset(const Compare& cmp) : t(cmp) { }
		multiset(const multiset<Key, Compare>& x) : t(x.t) { }

		multiset<Key, Compare>& operator=(const multiset<Key, Compare>& x) { t = x.t; return *this; }

		key_compare key_comp() const { return t.key_comp(); }
		value_compare value_comp() const { return t.key_comp(); }
		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		void swap(multiset<Key, Compare>& x) { t.swap(x.t); }

		pair<iterator, bool> insert(const value_type& val)
		{
			pair<rep_iterator, bool> p = t.insert_equal(val);
			return pair<iterator, bool>(p.first, p.second);
		}
		void erase(iterator pos) { return t.erase(rep_iterator(pos.node)); }
		void clear() { return t.clear(); }

		iterator find(const value_type& val) const { return t.find(val); }

		bool operator == (const multiset<Key, Compare>& y) const
		{
			return t == y.t;
		}
		bool operator != (const multiset<Key, Compare>& y) const
		{
			return !(t == y.t);
		}

		friend bool operator == <> (const multiset<Key, Compare>& x, const multiset<Key, Compare>& y);
		friend bool operator != <> (const multiset<Key, Compare>& x, const multiset<Key, Compare>& y);

	};  // end of class set;

	template<class Key, class Compare>
	bool operator == (const multiset<Key, Compare>& x, const multiset<Key, Compare>& y)
	{
		return x.operator==(y);
	}
	template<class Key, class Compare>
	bool operator != (const multiset<Key, Compare>& x, const multiset<Key, Compare>& y)
	{
		return x.operator!=(y);
	}

}   // namespace TinySTL


#endif // !_MULTISET_H_