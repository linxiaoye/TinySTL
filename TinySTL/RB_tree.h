#ifndef _RB_TREE_H_
#define _RE_TREE_H_

/*
*   Author  @yyehl
*/

#include "Iterator.h"
#include "Allocator.h"


namespace TinySTL
{
	typedef bool rb_tree_color;
	const rb_tree_color RED = true;
	const rb_tree_color BLACK = false;

	/******************** class rb_tree_node **********************/
	template<class Value>
	struct rb_tree_node
	{
		typedef rb_tree_color               tree_color;
		typedef rb_tree_node<Value>*        node_ptr;
		typedef Value                       value_type;

		Value value_field;
		tree_color color;
		node_ptr left;
		node_ptr right;
		node_ptr parent;

		rb_tree_node() : value_field(value_type()), color(RED), left(nullptr), right(nullptr), parent(nullptr) { }
		explicit rb_tree_node(const value_type& val) { rb_tree_node(), value_field = val; }
		rb_tree_node(const rb_tree_node<Value>& x) : value_field(x.value_field), color(x.color), left(nullptr), 
			right(nullptr), parent(nullptr) { }
	
		static node_ptr min_value(node_ptr p)
		{
			while (p->left != nullptr)
			{
				p = p->left;
			}
			return p;
		}
		static node_ptr max_value(node_ptr p)
		{
			while (p->right != nullptr)
			{
				p = p->right;
			}
			return p;
		}

	};

	/****************** class rb_tree_iterator **********************/

	template<class Value, class Ref, class Ptr>
	bool operator == (const rb_tree_iterator<Value, Ref, Ptr>& lhs, const rb_tree_iterator<Value, Ref, Ptr>& rhs)
	{
		return lhs.node == rhs.node;
	}
	template<class Value, class Ref, class Ptr>
	bool operator != (const rb_tree_iterator<Value, Ref, Ptr>& lhs, const rb_tree_iterator<Value, Ref, Ptr>& rhs)
	{
		return lhs.node != rhs.node;
	}

	template<class Value, class Ref, class Ptr>
	struct rb_tree_iterator
	{
		typedef bidirectional_iterator_tag    iterator_category;
		typedef Value                         value_type;
		typedef Ref                           reference;
		typedef Ptr                           pointer;
		typedef size_t                        size_type;
		typedef ptrdiff_t                     difference_type;
		
		typedef rb_tree_iterator<Value, Value&, Value*>                     iterator;
		typedef rb_tree_iterator<const Value, const Value&, const Value*>   const_iterator;
		typedef rb_tree_iterator<Value, Ref, Ptr>                           self;
		typedef rb_tree_node<Value>                                         tree_node;

		tree_node* node;

		rb_tree_iterator() { }
		rb_tree_iterator(tree_node* x) : node(x) { }
		rb_tree_iterator(const iterator& x) : node(x.node) { }

		reference operator * () { return node->value_field; }
		pointer operator -> () { return &(*this); }

		self& operator ++ () { increase(); return *this; }
		self operator ++ (int) { self tmp = *this; ++(*this); return tmp; }
		self& operator -- () { decrease(); return *this; }
		self operator -- (int) { self tmp = *this; --(*this); return tmp; }

		bool operator == (const iterator& x) const { return node == x.node; }
		bool operator != (const iterator& x) const { return !(*this == x); }

		void increase();
		void decrease();

	};

	/************************** class rb_tree ****************************/

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	class rb_tree;

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator == (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
	{
		return lhs.operator==(rhs);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator != (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
	{
		return lhs.operator!=(rhs);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void swap(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&lhs, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs)
	{
		return lhs.swap(rhs);
	}


	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc = allocator<rb_tree_node<value>>>
	class rb_tree
	{
	public:
		friend bool operator == (const rb_tree& lhs, const rb_tree& rhs);
		friend bool operator != (const rb_tree& lhs, const rb_tree& rhs);
		friend void swap(rb_tree& lhs, rb_tree& rhs);
	protected:
		typedef void*                  void_pointer;
		typedef rb_tree_node<Value>    tree_node;
		typedef Alloc                  tree_node_allocator;
		typedef rb_tree_color          color_type;

	public:
		typedef Key                    key_type;
		typedef Value                  value_type;
		typedef value_type*            pointer;
		typedef const value_type*      const_pointer;
		typedef value_type&            reference;
		typedef const value_type&      const_reference;
		typedef size_t                 size_type;
		typedef ptrdiff_t              difference_type;
		
		typedef rb_tree_iterator<Value, Value&, Value*>                       iterator;
		typedef rb_tree_iterator<const Value, const Value&, const Value*>     const_iterator;

	protected:    /* ����rb_tree_node �Ĳ���*/
		tree_node* get_node() { return tree_node_allocator::allocate(); }  // ����һ��tree_node�Ŀռ�
		void put_node(tree_node* p) { tree_node_allocator::deallocate(p); }  // ����һ��tree_node�Ŀռ�
		tree_node* create_node(const value_type& val)     // ����һ��ֵΪval��tree_node���ȷ���ռ䣬�ٹ������
			{ tree_node* p = get_node(); construct(p, val); return p; }  
		tree_node* clone_node(const tree_node* x)     // ����һ��tree_node�ڵ㣬ֻ����value_field��color
			{ tree_node* p = create_node(x->value_field); p->color = x->color; x->left = x->right = x->parent = nullptr; }
		void destroy_node(tree_node* p) { destory(&(p->value_field)); put_node(p); } // ����һ��tree_node�� �����������ٻ��տռ�
	
	protected:   // rb_tree�����ݽṹ
		size_type node_count; // �ڵ��������Ҳ����size
		tree_node* header;    // header��һ����Ƽ��ɣ�parentΪ���ڵ㣬leftΪ����ֵ��rightΪ����ֵ��colorΪRED
		Compare key_compare;  // �ڵ��key�ıȽ�׼����һ����������

	protected: 
		tree_node*& root() const { return (tree_node*&)header->parent; }  
		tree_node*& leftmost() const { return (tree_node*&)header->left; }
		tree_node*& rightmost() const { return (tree_node*&)header->right; }

		/* ���º�����������ȡ�ýڵ�x�ĳ�Ա */
		static tree_node*& left(tree_node* p) { return (tree_node*&)p->left; }
		static tree_node*& right(tree_node* p) { return (tree_node*&)p->right; }
		static tree_node*& parent(tree_node* p) { return (tree_node*&)p->parent; }
		static value_type& value(tree_node* p) { return p->value_field; }
		static const key_type& key(tree_node* p) { return KeyOfValue()(value(p)); }
		static color_type& color(tree_node* p) { return (color_type&)p->color; }

		static tree_node* min_value_node(tree_node* p) { return rb_tree_node<Value>::min_value(p); }
		static tree_node* max_value_node(tree_node* p) { return rb_tree_node<Value>::max_value(p); }

	private:  
		void init() { header = get_node(); color(header) = RED; root() = nullptr; leftmost() = rightmost() = header; }
		iterator _insert(tree_node* x, tree_node* y, const value_type& val);
		void _erase(tree_node* p);
		tree_node* _copy(tree_node* x, tree_node* y);

	public:
		rb_tree(const Compare& cmp = Compare()) : key_compare(cmp) { }
		rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);
		~rb_tree() { clear(); put_node(header); }

		rb_tree& operator = (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

		Compare key_comp() const { return key_compare; }
		iterator begin() { return iterator(header->left); }
		iterator end() { return iterator(header); }
		const_iterator begin() const { return const_iterator(header->left); }
		const_iterator end() const { return const_iterator(header); }
		bool empty() const { return size() == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return size_type(-1); }
		bool contain(const value_type& val) const;

		bool operator == (const rb_tree& x) { return header == x.header && key_compare = x.key_compare && node_count = x.node_count; }
		bool operator != (const rb_tree& x) { return !(*this == x); }

		iterator insert_equal(const value_type& val);
		pair<iterator, bool> insert_unique(const value_type& val);
		void erase(iterator pos);
		iterator find(const value_type& val); 
		void clear();
		void swap(rb_tree& x);
		


	};














} // namespace TinySTL

#include "./Detail/RB_tree.impl.h"

#endif // !_RB_TREE_H_

