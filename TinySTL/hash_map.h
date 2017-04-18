#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_

/*
*  Author @yyehl
*/

#include "Hashtable.h"

/*
*  map以rb_tree为底层，hash_map以hashtable为底层
*  map为有序，不重复集合，hash_map为无序，不重复集合
*  map查找效率为O(logN)， hash_map查找效率平均为O(1)
*  hash_map的使用方法和接口，与map几乎完全相同
*  hash_map与map一样，拥有一个键值与一个实值
*  hash_map存的是pair<key, value>, hash_set存的直接是value
*/

namespace TinySTL
{
	template<class Key, class Value, class Hashfcn = hash<Key>, class Equalkey = equal_to<Key> >
	class hash_map
	{
	private:
		typedef hashtable<pair<Key, Value>, Key, Hashfcn, select1st<pair<const Key, Value>>, Equalkey>    hashtable;

		hashtable rep;
	public:
		typedef Value  data_type;
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
		hash_map() : rep(100, hasher(), key_equal()) { }
		explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) { }
		hash_map(size_type n, const hasher& hf, const key_equal& eqk) : rep(n, hf, eqk) { }
		hash_map(size_type n, const hasher& hf) : rep(n, hf, key_equal()) { }

	public:
		size_type size() const { return rep.size(); }
		bool empty() const { return size() == 0; }

		iterator begin() const { return rep.begin(); }
		iterator end() const { return rep.end(); }

		void swap(const hash_map& hs) { rep.swap(hs.rep); }
		bool operator == (const hash_map& x) const { return rep.operator==(x.rep); }
		bool operator != (const hash_map& x) const { return !(*this == x); }
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
		data_type& operator [] (const key_type& key)
		{
			if (find(key) == end())  // 没找到
			{
				pair<iterator, bool> tmp = insert(pair<key, data_type()>);
				return ((tmp.first.cur)->val).second;
			}
			else
				return (((find(key)).cur)->val).second;
		}
	};

	template<class Key, class Value, class Hashfcn, class Equalkey>
	bool operator == (const hash_map<Key, Value, Hashfcn, Equalkey>& lhs, const hash_map<Key, Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.operator==(rhs);
	}
	template<class Key, class Value, class Hashfcn, class Equalkey>
	bool operator != (const hash_map<Key, Value, Hashfcn, Equalkey>& lhs, const hash_map<Key, Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.operator!=(rhs);
	}
	template<class Key, class Value, class Hashfcn, class Equalkey>
	void swap(hash_map<Key, Value, Hashfcn, Equalkey>& lhs, hash_map<Key, Value, Hashfcn, Equalkey>& rhs)
	{
		return lhs.swap(rhs);
	}


}  // namespace TinySTL


#endif // !_HASH_MAP_H_

