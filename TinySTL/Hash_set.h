#ifndef _HASH_SET_H_
#define _HASH_SET_H_

/*
*  Author  @yyehl
*/


/*
*  set以rb_tree为底层，hash_set以hashtable为底层
*  set为有序，不重复集合，hash_set为无序，不重复集合
*  set查找效率为O(logN)， hash_set查找效率平均为O(1)
*  hash_set的使用方法和接口，与set几乎完全相同
*  hash_set 实值即为键值，键值就是实值
*/


#include "Hashtable.h"

namespace TinySTL
{
	template<class Value, class Hashfcn = hash<Value>, class Equalkey = equal_to<Value> >
	class hash_set
	{
	private:
		typedef hashtable<Value, Value, Hashfcn, identity<Value>, Equalkey>    hashtable;

		hashtable rep;
	public:
		typedef typename hashtable::value_type     value_type;
		typedef typename hashtable::key_type       key_type;
		typedef typename hashtable::hasher         hasher;
		typedef typename hashtable::key_equal      key_equal;
		typedef typename hashtable::size_type      size_type;
		typedef typename hashtable::iterator       iterator;
		typedef typename hashtable::reference      reference;

		hasher hash_funct() const { return Hashfcn(); }
		hasher key_eq() const { return Equalkey(); }

	public:
		hash_set() : rep(100, hasher(), key_equal()) { }
		explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) { }
		hash_set(size_type n, const hasher& hf, const key_equal& eqk) : rep(n, hf, eqk) { }
		hash_set(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }

	public:
		size_type size() const { return rep.size(); }
		bool empty() const { return size() == 0; }

		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }

		void swap(const hash_set& hs) { rep.swap(hs.rep); }
		bool operator == (const hash_set& x) const { return rep.operator==(x.rep); }
		bool operator != (const hash_set& x) const { return !(*this == x); }
	public:
		pair<iterator, bool> insert(const value_type& val)
		{
			pair<typename hashtable::iterator, bool> tmp = rep.insert_unique(val);
			return pair<iterator, bool>(tmp.first, tmp.second);
		}
		iterator find(const key_type& key) const { return rep.find(key); }
		size_type count(const key_type& key) const { return rep.count(key); }
		void erase(iterator pos) { rep.erase(pos); }
		void clear() { rep.clear(); }

		void resize(size_type n) { rep.resize(n); }
		size_type buckets_count() const { return buckets_count(); }	
	};

	template<class Value, class Hashfcn, class Equalkey>
	bool operator == (const hash_set<Value, Hashfcn, Equalkey>& lhs, const hash_set<Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.operator==(rhs);
	}
	template<class Value, class Hashfcn, class Equalkey>
	bool operator != (const hash_set<Value, Hashfcn, Equalkey>& lhs, const hash_set<Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.operator!=(rhs);
	}
	template<class Value, class Hashfcn, class Equalkey>
	void swap (hash_set<Value, Hashfcn, Equalkey>& lhs, hash_set<Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.swap(rhs);
	}

}  // namespace TinySTL


#endif // !_HASH_SET_H_

