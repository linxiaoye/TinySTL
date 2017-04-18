#ifndef _HASHTABLE_IMPL_H_
#define _HASHTABLE_IMPL_H_

/*
*    Author  @yyehl
*/

namespace TinySTL
{
	/****************************** strcut hashtable_iterator ****************************************/
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::operator ++ () -> iterator&
	{
		hashtable_node* old = cur;
		cur = cur->next;
		if (cur == nullptr)
		{
			size_type bucket = ht->bkt_num(old->val);  // ԭbucketֵ
			while (cur == nullptr && ++bucket < ht.buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::operator ++ (int)->iterator
	{
		iterator tmp = *this;
		++*this;
		return tmp;
	}

	/************************* class hashtable **************************************/
	
	const int buckets_prime_nums = 28;
	const unsigned long buckets_num_list[buckets_prime_nums] =
	{
		53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
		196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457, 1610612741, 3221225473ul,
		4294967291ul
	};


	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	inline unsigned long hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::next_prime_num(size_type n)
	{
		const unsigned long* first = buckets_num_list;
		const unsigned long* last = first + buckets_prime_nums;
		const unsigned long* pos = first;
		while (*pos < n && pos != last)
			++pos;
		return (pos == last) ? *(last - 1) : *pos;
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::new_node() -> node*
	{
		node* ret = node_allocator::allocate();
		ret->next = nullptr;
		construct(&(ret->val), value_type());
		return ret;
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::destroy_node(node* ptr)
	{
		destroy(&(ptr->val));
		node_allocator::deallocate(ptr);
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::init_buckets(size_type n)
	{
		const size_type buckets_size = next_prime_num(n);
		buckets.reserve(bucket_size);
		buckets.insert(buckets.end(), buckets.size, (node*)nullptr);
		num_elems = 0;
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::insert_unique(const value_type& val)->pair<iterator, bool>
	{
		resize(num_elems + 1);  
		return insert_unique_noresize(val);
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::insert_equal(const value_type& val)->iterator
	{
		resize(num_elems + 1);
		return insert_equal_noresize(val);
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::resize(size_type n)
	{
		// 判断标准，当元素的数量大于buckets的数量（即vector的大小）时，就重建vector
		// 所以元素个数的最大值也就等于buckets的数量，可以基本保证开链长度不过长影响性能
		const size_type old_n = buckets.size(); // 原buckets的大小
		if (n > old_n)     // 如果新的元素数量n比原buckets的size要大
			const size_type new_n = next_prime_num(n);  //则寻找新的下一个质数
		if (new_n > old_n)   // 如果新的质数比原来的大（当old_n已经是最大的质数了，那么new_n会等于old_n）
		{ 
			vector<node*> tmp(new_n, nullptr); // 新的vector
			for (size_type bucket = 0; bucket < old_n; ++bucket) // 循环处理原buckets的元素
			{
				node* first = buckets[bucket];
				size_type new_bucket = bkt_num(first->val, new_n);
				buckets[bucket] = first->next;
				first->next = tmp[new_bucket]; // 令将要插入进来的节点的next指向原本开链的头结点，才能连起来
				tmp[new_bucket] = first;
				first = buckets[bucket];
			}
			buckets.swap(tmp);  // vector的swap函数
			// 离开时 将会释放tmp的内存
		}
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::insert_unique_noresize(const value_type& val)
		->pair<iterator, bool>
	{
		size_type n = bkt_num(val, buckets_count());
		node* first = buckets[n];
		for (node* cur = first; cur != nullptr; ++cur)
		{
			if (equals(get_key(val), get_key(cur->val)))
				return pair<iterator, bool>(iterator(cur, this), bool);
		}
		node* tmp = new_node();
		tmp->val = val;
		tmp->next = first;
		buckets[n] = tmp;
		++num_elems;
		return pair<iterator, bool>(iterator(tmp, this), true);
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::insert_equal_noresize(const value_type& val)->iterator
	{
		size_type n = bkt_num(val, buckets_count());
		node* first = buckets[n];
		for (node* cur = first; cur != nullptr; ++cur)
		{
			if (equals(get_key(val), get_key(cur->val)))
			{
				node* tmp = new_node();
				tmp->val = val;
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elems;
				return iterator(tmp, this);
			}
		}
		node* tmp = new_node();
		tmp->val = val;
		tmp->next = first;
		buckets[n] = tmp;
		++num_elems;
		return iterator(tmp, this);
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::erase(iterator pos)
	{
		size_type n = bkt_num(*pos, buckets_count());
		node* cur = buckets[n];
		while (cur->next != pos.cur)   // cur is pos.cur's parent
			++cur;
		cur->next = pos.cur->next;
		destroy_node(pos.cur);
		--num_elems;
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::clear()
	{
		/* clear只清除开链，不回收buckets(vector) 的空间 */
		for (size_type i = 0; i != buckets.size(); ++i)
		{
			node* cur = buckets[i];
			while (cur != nullptr)
			{
				node* next = cur->next;
				destroy_node(cur);
				cur = next;
			}
			buckets[i] = nullptr;   // 将buckets的指针都置为nulltpr
		}
		num_elems = 0;  
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::copy_from(const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& ht)
	{
		buckets.clear();
		buckets.reserve(ht.buckets_count.size());
		buckets.insert(buckets.begin(), buckets.end(), nullptr);
		for (size_type i = 0; i < ht.buckets_count(); ++i)
		{
			if (const node* cur = ht.buckets[i])  // 当不为nullptr时才执行
			{
				node* copy = new_node();
				copy->val = cur->val;
				buckets[i] = copy;

				for (node* next = cur->next; next != nullptr; cur = next, next = cur->next)
				{
					copy->next = new_node();
					copy->val = next->val;
					copy = copy->next;
				}
			}
		}
		num_elems = ht.num_elems;
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	void hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::swap(hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x)
	{
		buckets.swap(x.buckets);
		swap(num_elems, x.num_elems);
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::find(const key_type& key)  -> iterator
	{
		size_type n = bkt_num_key(key);
		node* first = buckets[n];
		while (first != nullptr && !equals(key, first->val))
			first = first->next;
		if (first != nullptr)
			return iterator(first, this);
		else
			return end();
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::count(const key_type& key) const ->size_type
	{
		const size_type n = bkt_num_key(key);
		size_type ret = 0;
		node* first = buckets[n];
		for (node* cur = first; cur != nullptr; cur = cur->next)
			if (equals(key, get_key(cur->val)))
				++ret;
		return ret;
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::begin() -> iterator
	{
		node* ret = nullptr;
		for (int i = 0; i < buckets_count(); ++i)
		{
			if (buckets[i] != nullptr)
			{
				ret = buckets[i];
				break;
			}
		}
		return iterator(ret, this);
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::end() -> iterator
	{
		node* end = buckets[buckets_count()];
		while (end != nullptr)
			end = end->next;
		return iterator(end, this);
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	bool hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::operator== (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x) const
	{
		if (num_elems != x.num_elems || buckets != x.buckets)
			return false;
		for (size_type i = 0; i < buckets.size(); ++i)
		{
			node* p1 = buckets[i];
			node* p2 = buckets[i];
			while (p1->val == p2->val && p1 != nullptr && p2->nullptr)
			{
				p1 = p1->next;
				p2 = p2->next;
			}
			if (!(p1 == nullptr && p2 == nullptr))
				return false;
		}
		return true;
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	bool hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::operator!= (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x) const
	{
		return !(this->operator==(x));
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::~hashtable()
	{
		clear();
		buckets.clear();
		buckets.shrink_to_fit();
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::operator = (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x) -> self&
	{
		copy_from(x);
		return *this;
	}

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::bkt_num(const value_type& val, size_type n) -> size_type
	{
		return bkt_num_key(get_key(val), n);
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::bkt_num(const value_type& val)->size_type
	{
		return bkt_num_key(get_key(val));
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::bkt_num_key(const key_type& key)->size_type
	{
		return bkt_num_key(key, buckets.size());
	}
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	auto hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>::bkt_num_key(const key_type& key, size_type n)->size_type
	{
		return hash(key) % n;
	}

}  // namespace STL


#endif // !_HASHTABLE_IMPL_H_
