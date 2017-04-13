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








}  // namespace STL


#endif // !_HASHTABLE_IMPL_H_
