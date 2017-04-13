#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

/*
*   Author   @yyehl
*/

#include "Functional.h"
#include "Allocator.h"
#include "Construct.h"
#include "Utility.h"
#include "Vector.h"
#include "Iterator.h"

namespace TinySTL
{
   /*********************** strcut hashtable_node *****************************/
	template<class T>
	struct hashtable_node
	{
		hashtable_node* next;
		T val;
	};

	/*********************** strcut hashtable_iterator *****************************/
	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc>
	struct hashtable_iterator
	{
		typedef hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>  hashtable;
		typedef hashtable_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc> iterator;
		typedef hashtable_const_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc> const_iterator;
		typedef hashtable_node<Value> node;

		typedef forward_iterator_tag  iterator_category;
		typedef Value                 value_type;
		typedef ptrdiff_t             difference_type;
		typedef size_t                size_type;
		typedef Value&                reference;
		typedef Value*                pointer;

		/* 数据结构 */
		hashtable* ht;        // 对容器保持连接关系，hashfcn的起始计算位置
		hashtable_node* cur;  // 当前所指的节点

		hashtable_iterator() { }
		hashtable_iterator(hashtable_node* x, hashtable* y) : ht(y), cur(x) { }
		reference operator * () const { return cur->val; }
		pointer operator -> () const { return &(operator*()); }

		iterator& operator ++ ();
		iterator operator ++ (int);

		bool operator == (const iterator& iter) const { return ht == iter.ht && cur == iter.cur; }
		bool operator != (const iterator& iter) const { return !((*this) == iter); }

	};





}   // namespace TinySTL

#endif // !_HASHTABLE_H_

