#ifndef _RB_TREE_IMPL_H_
#define _RB_TREE_TMPL_H_

/*
*  Author  @yyehl
*/

namespace TinySTL
{
	/*************** class rb_tree_iterator ******************************/
	template<class Value, class Ref, class Ptr>
	void rb_tree_iterator<Value, Ref, Ptr>::increase()
	{
		if (node->right != nullptr)  // 1���ҽڵ���ڣ������ҷ�֧����С�ڵ㣨����ڵ㣩
		{
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else                        // �ҽڵ㲻����
		{
			tree_node* p = node->parent;
			if (p->parent == node)
				node = p;          // 2����nodeΪ���ڵ㣬����header
			else if (p->left == node)
				node = p;          // 3����nodeΪ���ڵ����ڵ㣬�����丸�ڵ�
			else
			{
				while (p->right == node)
				{
					node = p;
					p = p->parent;  
				}                    
				node = p;           // 4:��nodeΪ���ڵ���ҽڵ㣬��������nodeΪ���ڵ����ڵ�Ϊֹ
			}
		}
	}
	template<class Value, class Ref, class Ptr>
	void rb_tree_iterator<Value, Ref, Ptr>::decrease()
	{
		if (node->color == RED && node->parent->parent == node)
			node = node->right;  // 1:���nodeΪheader����ô��rightmost
		if (node->left != nullptr)
		{
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}                        // 2�������ڵ���ڣ���ô�����֧�����ڵ㣨����ڵ㣩
		else     // ������ڵ�
		{
			tree_node* p = node->parent;
			if (p->right == node)
				node = p;     // 3�����nodeΪ���ڵ���ҽڵ㣬�����丸�ڵ�
			else
			{
				while (p->left == node)
				{
					node = p;
					p = p->parent;
				}
				node = p;    // 4�����nodeΪ���ڵ����ڵ㣬��������nodeΪ���ڵ���ҽڵ�Ϊֹ
			}
		}
	}

	/******************** class rb_tree **************************************/

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(tree_node* x, tree_node* y, const value_type& x) -> iterator
	{
		tree_node* z;
		if (y == header || x != nullptr || key_compare(KeyOfValue()(val), key(x)))
		{ // ���Ҫ����ڵ�ĸ��ڵ�Ϊheader����Ҫ����Ľڵ㲻Ϊ�ջ���val��key��x��keyС
			z = create_node(val);
			left(z) = x;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
			}
			else if (y = leftmost())
				leftmost() = z;
		}
		else
		{
			z = create_node(val);
			right(y) = z;
			if (y == rightmost())
				rightmost = z;
		}
		parent(z) = y;
		left(z) = right(z) = nullptr;
		// ��������������ά�ֺ����������
		rb_tree_balance(z, root());  // ��һ������Ϊ�����ڵ㣬�ڶ�������Ϊroot
		++node_count;
		return iterator(z);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(tree_node* x, tree_node* y) -> tree_node*
	{   // xΪ��copy�Ľڵ㣬 y��copy���½ڵ�ĸ��ڵ�
		if (x == nullptr) return nullptr;
		tree_node* new_left;
		tree_node* new_right;
		tree_node* new_root;
		new_left = _copy(x->left, x);
		new_right = _copy(x->right, x);
		new_root = clone_node(x);
		new_root->left = new_left;
		new_root->right = new_right;
		new_root->parent = y;
		return new_root;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(tree_node* x)
	{
		
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
	{
		init();
		tree_node* new_root = _copy(x.root(), header);
		header->parent = new_root;
		node_count = x.node_count;
		key_compare = x.key_compare;
		for (tree_node* cur = root(); cur->left != nullptr; cur = cur->left)
			header->left = cur;
		for (tree_node* cur = root(); cur->right != nullptr; cur = cur->right)
			header->right = cur;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator = (const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x) -> rb_tree&
	{
		clear();
		rb_tree(x);
		return *this;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::contain(const key_type& keyl) const
	{
		return (find(key) == end()) ? false : true;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& key) -> iterator
	{
		tree_node* y = header;
		tree_node* x = root();
		while (x != nullptr)
		{
			if (!key_compare(key(x), key)) //���keyС��x�ڵ��key
			{
				y = x;
				x = x->left;
			}
			else    // ���key���ڵ���x�ڵ��key
			{
				x = x->right;
			}
		}   
		iterator j = iterator(y);
		return (j == end() || key_compare(key, key(j.node))) ? end() : j;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& val) -> iterator
	{
		tree_node* y = header;
		tree_node* x = root();
		while (x != nullptr)
		{
			y = x;
			x = key_compare(KeyOfValue()(val), key(x)) ? left(x) : right(x);
		}
		return _insert(x, y, v); // xΪҪ����Ľڵ�λ�ã�yΪҪ����Ľڵ�ĸ��ڵ�
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& val) -> pair<iterator, bool>
	{
		tree_node* x = root();
		tree_node* y = header;
		bool comp = true;
		while (x �� = nullptr)
		{
			y = x;
			comp = key_compare(KeyOfValue()(val), key(x));
			x = comp ? left(x) : right(x);
		}
		iterator j = iterator(y);
		if (comp) 
		{
			if (j = begin())
				return pair<iterator, bool>(_insert(x, y, val), true);
			else
				--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(val)))
			return pair<iterator, bool>(_insert(x, y, val), true);
		return pair<iterator, bool>(j, false);
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(tree_node* pos)
	{

	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear()
	{
		destroy_tree(root());
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
	{
		tree_node* tmp_header = x.header;
		size_type tmp_count = x.node_count;
		Compare tmp_comp = x.key_compare;
		x.header = header;
		x.node_count = node_count;
		x.key_compare = key_compare;
		header = tmp_header;
		node_count = tmp_count;
		key_compare = tmp_comp;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::destroy_tree(tree_node* root)
	{
		if (root == nullptr) return;
		destroy_tree(root->left);
		destroy_tree(root->right);
		destroy_node(root);
	}

}  // namespace Tiny STL


#endif // !_RB_TREE_IMPL_H_
