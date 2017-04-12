#ifndef _MAP_H_
#define _MAP_H_

/*
*  Author @yyehl
*/

#include "RB_tree.h"
#include "Functional.h"


namespace TinySTL
{
	template<class Key, class T, class Compare = less<Key>>
	class map
	{
	public:
		typedef Key              key_type;
		typedef T                data_type;
		typedef pair<Key, T>     value_type;
		typedef T                mapped_type;
		typedef Compare          key_compare;

	private:
		typedef rb_tree<key_type, value_type, selece1st<value_type>, key_compare> rep_type;
		rep_type t;   // 底层数据结构  rb_tree

		typedef typename rep_type::iterator            iterator;
		typedef typename rep_type::const_iterator      const_iterator;
		typedef typename rep_type::pointer             pointer;
		typedef typename rep_type::const_pointer       const_pointer;
		typedef typename rep_type::reference           reference;
		typedef typename rep_type::const_reference     const_reference;
		typedef typename rep_type::size_type           size_type;
		typedef typename rep_type::difference_type     difference_type;

	public:
		map() : t(key_compare()) { }
		explicit map(const Compare& cmp) : t(cmp) { }
		map(const map<key_type, data_type, Compare>& x) : t(x.t) { }

		map<Key, T, Compare>& operator = (const map<Key, T, Compare>& x) { t = x.t; return *this; }

		key_compare key_comp() const { return t.key_comp(); }
		iterator begin() const { return t.begin(); }
		iterator end() const { return t.end(); }
		bool empty() const { return t.empty(); }
		size_type size() const { return t.size(); }
		size_type max_size() const { return t.max_size(); }
		void swap(map<Key, T, Compare>& x) { t.swap(x.t); }

		T& operator [] (const key_type& k) { return (*((insert(value_type(k, T()))).first)).second; }

		pair<iterator, bool> insert(const value_type& x) { return t.insert_unique(x); }
		void erase(iterator pos) { t.erase(pos); }
		void clear() { t.clear(); }
		iterator find(const key_type& k) const { return t.find(k); }

		bool operator == (const map<Key, T, Compare>& y) const
		{
			return t == y.t;
		}
		bool operator != (const map<Key, T, Compare>& y) const
		{
			return !(t == y.t);
		}

		friend bool operator == <> (const map<Key, T, Compare>& x, const map<Key, T, Compare>& y);
		friend bool operator != <> (const map<Key, T, Compare>& x, const map<Key, T, Compare>& y);

	};

	template<class Key, class T, class Compare>
	bool operator == (const map<Key, T, Compare>& x, const map<Key, T, Compare>& y)
	{
		return x.operator==(y);
	}
	template<class Key, class T, class Compare>
	bool operator != (const map<Key, T, Compare>& x, const map<Key, T, Compare>& y)
	{
		return x.operator!=(y);
	}



}  // namespace TinySTL




#endif // !_MAP_H_

