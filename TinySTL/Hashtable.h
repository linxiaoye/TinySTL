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

		/* ���ݽṹ */
		hashtable* ht;        // �������������ӹ�ϵ��hashfcn����ʼ����λ��
		node* cur;  // ��ǰ��ָ�Ľڵ�

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
		*  hashtable��ģ������ܶ࣬����Ū������ǵ��׶���ʲô����
		*  Value���ڵ��ʵֵ����    Key���ڵ�ļ�ֵ����
		*  Hashfcn��hash functions�������ͱ����ڼ���Ԫ�ش�ŵ�λ�ã��º���
		*  Extractkey���ӽڵ���ȡ��key�ķ������º���
		*  Equalkey������ڵ�keyֵ��ȵķ������º���
		*  Alloc�������ڵ�Ŀռ������
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
		

		typedef hashtable_node<Value>  node;  // �����Ľڵ�
		typedef Alloc                  node_allocator;

		typedef hashtable<Value, Key, Hashfcn, Extractkey, Equalkey, Alloc>  self

		// ������hashtable�����ݽṹ
		vector<node*> buckets;
		size_type num_elems;

		// ����Ϊ28�����������ڸ�buckets�����С
	   
		static const int buckets_prime_nums;
		static const unsigned long buckets_num_list[buckets_prime_nums];
		
	public:
		// ���캯�������ṩĬ�Ϲ��캯��
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
		unsigned long next_prime_num(size_type n);  // �ڲ�ʹ�ã�����buckets_num_list�д���n�ĵ�һ������
		// ���º��� ���� hashtable�������õ�����buckets�������ǲ�����size��Ŷ
		unsigned long max_buckets_count() const { return buckets_num_list[buckets_prime_nums - 1]; }
		node* new_node();   // ���䲢����һ����node�ڵ�
		void destroy_node(node* ptr);  // ����value�����ջ�node�ռ�
		void init_buckets(size_type n); //��ʼ��buckets��vector
		void resize(size_type n);    // �ж��費��Ҫ�ؽ�buckets����Ҫ�Ļ����ؽ�
		pair<iterator, bool> insert_unique_noresize(const value_type& val);  
		iterator insert_equal_noresize(const value_type& val);
		size_type bkt_num(const value_type& val, size_type n); // ����hash�����õ�Ԫ�ص�λ��
		size_type bkt_num(const value_type& val);
		size_type bkt_num_key(const key_type& key);
		size_type bkt_num_key(const key_type& key, size_type n);
	};



}   // namespace TinySTL

#include "Detail\Hashtable.impl.h"

#endif // !_HASHTABLE_H_

