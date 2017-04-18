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
#include "Algorithm.h"

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
		//typedef hashtable_const_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc> const_iterator;
		typedef hashtable_node<Value> node;

		typedef forward_iterator_tag  iterator_category;
		typedef Value                 value_type;
		typedef ptrdiff_t             difference_type;
		typedef size_t                size_type;
		typedef Value&                reference;
		typedef Value*                pointer;

		/* 数据结构 */
		hashtable* ht;        // 对容器保持连接关系，hashfcn的起始计算位置
		node* cur;  // 当前所指的节点

		hashtable_iterator() { }
		hashtable_iterator(node* x, hashtable* y) : ht(y), cur(x) { }
		reference operator * () const { return cur->val; }
		pointer operator -> () const { return &(operator*()); }

		iterator& operator ++ ();
		iterator operator ++ (int);

		bool operator == (const iterator& iter) const { return ht == iter.ht && cur == iter.cur; }
		bool operator != (const iterator& iter) const { return !((*this) == iter); }

	};

	/************************* class hashtable **************************************/

	template<class Value, class Key, class Hashfcn, class Extractkey, class Equalkey, class Alloc = allocator<hashtable_node<Value>>>
	class hashtable
	{
		/*
		*  hashtable的模板参数很多，先来弄清楚它们到底都是什么意义
		*  Value：节点的实值类型    Key：节点的键值类型
		*  Hashfcn：hash functions的类型型别，用于计算元素存放的位置，仿函数
		*  Extractkey：从节点中取出key的方法，仿函数
		*  Equalkey：定义节点key值相等的方法，仿函数
		*  Alloc：开链节点的空间分配器
		*/


	public:
		typedef Hashfcn     hasher;
		typedef Equalkey    key_equal;
		typedef Value       value_type;
		typedef Key         key_type;
		typedef size_t      size_type;
		typedef value_type& reference;
		typedef hashtable_iterator<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc> iterator;
	private:
		typedef hasher      hash;
		typedef key_equal   equals;
		typedef Extractkey  get_key;
		

		typedef hashtable_node<Value>  node;  // 开链的节点
		typedef Alloc                  node_allocator;

		typedef hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>  self

		// 以下是hashtable的数据结构
		vector<node*> buckets;
		size_type num_elems;

		// 以下为28个质数，用于给buckets定义大小
	   
		static const int buckets_prime_nums;
		static const unsigned long buckets_num_list[buckets_prime_nums];
		
	public:
		// 构造函数，不提供默认构造函数
		hashtable(size_type n, const Hashfcn& hf, const Equalkey& eql) 
			:hasher(hf), key_equal(eql), get_key(Extractkey()), num_elems(0) 
			{ init_buckets(n); }
		~hashtable();

		self& operator = (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x);
		
		pair<iterator, bool> insert_unique(const value_type& val);
		iterator insert_equal(const value_type& val);
		void erase(iterator pos);
		void clear();
		void copy_from(const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& ht);
		void swap(hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x);
		iterator find(const key_type& key);
		size_type count(const key_type& key) const;
		
		iterator begin() const;
		iterator end() const;

		bool operator == (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x) const;
		bool operator != (const hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>& x) const;

		size_type size() const { return num_elems; }
		size_type buckets_count() const { return buckets.size(); }

	private:
		unsigned long next_prime_num(size_type n);  // 内部使用，返回buckets_num_list中大于n的第一个质数
		// 以下函数 返回 hashtable可以设置的最大的buckets数，但是并不是size数哦
		unsigned long max_buckets_count() const { return buckets_num_list[buckets_prime_nums - 1]; }
		node* new_node();   // 分配并构造一个新node节点
		void destroy_node(node* ptr);  // 析构value对象并收回node空间
		void init_buckets(size_type n); //初始化buckets的vector
		void resize(size_type n);    // 判断需不需要重建buckets，需要的话就重建
		pair<iterator, bool> insert_unique_noresize(const value_type& val);  
		iterator insert_equal_noresize(const value_type& val);
		size_type bkt_num(const value_type& val, size_type n); // 调用hash函数得到元素的位置
		size_type bkt_num(const value_type& val);
		size_type bkt_num_key(const key_type& key);
		size_type bkt_num_key(const key_type& key, size_type n);
	};



}   // namespace TinySTL

#include "Detail\Hashtable.impl.h"

#endif // !_HASHTABLE_H_

