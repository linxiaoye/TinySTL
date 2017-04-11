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
		if (y == header || x != nil || key_compare(KeyOfValue()(val), key(x)))
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
		left(z) = right(z) = nil;
		// ��������������ά�ֺ����������
		rb_insert_balance(z, root());  // ��һ������Ϊ�����ڵ㣬�ڶ�������Ϊroot
		++node_count;
		return iterator(z);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_insert_balance(tree_node* x, tree_node*& root)
	{
		x->color = RED; // �½��ڵ�δ����ǰ��ԶΪ��
		while (x != root && x->parent->color == RED) // ��x��Ϊroot���½��ڵ�ĸ��ڵ�ΪRED�ǲ���Ҫ����
		{
			if (x->parent == x->parent->parent->left) // ��x�ĸ��ڵ�Ϊ�丸�ڵ����ڵ�ʱ
			{
				tree_node* y = x->parent->parent->right; //yΪx�Ĳ����ڵ�
				if (y != nil && y->color == RED)  //��������ڵ������Ϊ�죬��ֱ�ӵ�����ɫ������
				{
					x->parent->parent->color = RED;
					x->parent->color = BLACK;
					y->color = BLACK;
					x = x->parent->parent;
				}
				else  // ���򣬼������ڵ㲻���ڻ���Ϊ��
				{
					if (x == x->parent->right) // ���x���丸�ڵ���Ҷ��ӣ�������������Ϊ�����
					{
						x = x->parent;
						left_rotate(x, root);
					}
					x->parent->color = BLACK;  // ������֮�󣬾�ֻ��һ���������x���丸�ڵ㶼��������Ҷ���RED
					x->parent->parent->color = RED;
					right_rotate(x->parent->parent, root); // ������ɫ�������ͺ���
				}
			}
			else  // ���������Ļ����Գƣ���x�ĸ��ڵ�Ϊ�丸�ڵ���ҽڵ�
			{
				tree_node* y = x->parent->parent->left;
				if (y != nil && y->color == RED)  //�������ڵ�ΪRED
				{
					y->color = BLACK;
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					x = x->parent->parent;
				}
				else //�����ڵ㲻ΪRED
				{
					if (x == x->parent->left)
					{
						x = x->parent;
						right_rotate(x, root);
					}
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					left_rotate(x->parent->parent, root);
				}
			}
		}
		root->color = BLACK; //���ڵ���ԶΪ��
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
		if (x->left == nil && x->right == nil && x->color == RED) //1:���xΪҶ�ڵ���ΪRED��ֱ��ɾ��
		{
			if (x->parent->left == x) // �ж�x���丸�ڵ����ڵ㻹���ҽڵ㣬��ΪҪ�ı��丸�ڵ�ĺ���ָ��
				x->parent->left = nil;
			else
				x->parent->right = nil;
			destroy_node(x);
		} 
		else if (x->right == nil && x->right == nil && x->color == BLACK) //2:xλҶ�ڵ㣬��ΪBLACK����ôת��Ϊ˫������
		{
			bool left_or_right = true;   // left is true
			if (x == x->parent->left)
			{
				x->parent->left = nil;
			}
			else
			{
				x->parent->right = nil;
				left_or_right = false;
			}
			rb_erase_balance(left_or_right, x->parent, root());  // �������򣬵�һ������Ϊ����ɾ���ڵ�ĸ��ڵ�
			destroy_node(x);
		}
		else if (x->left != nil && x->right == nil) //3:xֻ����ڵ����ҽڵ㣬�����������xֻ��ΪBLACK
		{
			if (x->left->color == RED) // ���x������ΪRED����ôֱ�ӽ����Ӷ���x��λ�ã���������ΪBLACK
			{
				if (x->parent->left == x)
				{
					x->parent->left = x->left;
					x->left->parent = x->parent;
				}
				else
				{
					x->parent->right = x->left;
					x->left->parent = x->parent;
				}
				x->left->color = BLACK;
				destroy_node(x);
			}
			else  // �������ΪBLACK����ô���� ��˫�����⡱��Ҫɾ���ĺڽڵ�x��һ����ɫ�ĺ��ӽڵ�
			{
				bool left_or_right = true;   // left is true
				if (x->parent->left == x)
				{
					x->parent->left = x->left;
					x->left->parent = x->parent;
				}
				else
				{
					x->parent->right = x->left;
					x->left->parent = x->parent;
					left_or_right = false;
				}
				rb_erase_balance(left_or_right, x->parent, root());  // ��Ҫ����
				destroy_node(x);
			}
		}
		else if (x->right != nil && x->left == nil) // 4:xֻ���Һ��ӽڵ�û�����ӽڵ�
		{
			if (x->right->color == RED) // ����Һ��ӽڵ�ΪRED����ôֱ�ӽ��Һ��ӽڵ㶥��x��λ�ã�Ȼ����ΪBLACK
			{
				if (x == x->parent->left)
				{
					x->right->parent = x->parent;
					x->parent->left = x->right;
				}
				else
				{
					x->parent->right = x->right;
					x->right->parent = x->parent;
				}
				x->right->color = BLACK;
				destroy_node(x);
			}
			else  // ���x���Һ��ӽڵ�ΪBLACK����ô����˫�����⣬��Ҫɾ���ĺ�ɫ�ڵ�x��һ����ɫ�ĺ��ӽڵ�
			{
				bool left_or_right = true;  // left is true
				if (x == x->parent->left)
				{
					x->parent->left = x->right;
					x->right->parent = x->parent;
				}
				else
				{
					x->parent->right = x->right;
					x->right->parent = x->parent;
					left_or_right = false;
				}
				rb_erase_balance(left_or_right, x->parent, root());
				destroy_node(x);
			}
		}
		else  // 5��x�������������Һ��ӣ���ô���ö�����������ɾ����ʽ��������֧��С�ڵ㶥��x��ת��Ϊ1��2��3��4���
		{
			tree_node* y = nil;
			y = x->right;
			while (y->left != nil)           // Ѱ�Ҷ���x�ڵ��y�ڵ�
			{
				y = y->left;
			} 
			value_type tmp = x->value_field;   // ֻ����x��y��value����������ɫ
			x->value_field = y->value_field;
			y->value_field = tmp;
			_erase(y);                              // ���ˣ��Ѿ������5תΪ���1��2��3��4
		}

	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_erase_balance(bool l_or_r, tree_node* x, tree_node*& root)
	{
		bool complete = false;
		while (x != root && x->color == BLACK)  // ִ�е����ڵ����xΪRED����xΪRED����ôֱ�Ӱ�x��ΪBLACK�����ˡ�
		{
			if (l_or_r == true)  // left is true
			{
				tree_node* w = x->right;
				if (w->color == RED)  // 1:��������,������ת��Ϊ��������2��3��4
				{
					x->color = RED;
					x->right->color = BLACK;
					left_rotate(x, root);
					w = x->right;
				}
				            /* ����Ϊ�������� */
				if (w->color == BLACK)   
				{
					if (w->left->color == BLACK && w->right->color == BLACK) // 2:����ֵ�wΪBLACK�����������Һ��Ӿ�ΪBLACK������wΪRED
					{
						w->color = RED;   
					}
					else if (w->left->color == RED && w->right->color == BLACK)
					{
						w->color = RED;                         //3:���wΪBLACK������ֶΪRED����ֶΪBLACK
						w->left->color = BLACK;                      // ��תΪ4
						right_rotate(w);
					}
					else                                // 4:wΪBLACK����ֶΪBLACK����ֶΪRED
					{
						w->right->color = BLACK;
						w->color = x->color;
						x->color = BLACK;
						left_rotate(x, root);
						complete = true;
						break;	
					}
				}
				/* ���е����xҪôΪRED��ҪôΪBLACK������x�����������Ѿ�ƽ�� */
				/* ���xΪ�죬��ô�����˳�ѭ�����ڽ�����ִ��x����ɫ��ΪBLACK������ */
				/* ���Ϊ�ڣ���ô���ᵼ��xΪx���ֵ�ֱ�Ӻ�ɫ�߶Ȳ�1����ô��x��Ϊx��parent*/
				if (x->color == BLACK)
					x = x->parent;
			}
			else    // right
			{
				tree_node* w = x->left;
				if (w->color == RED)
				{
					w->color = BLACK;
					x->color = RED;
					left_rotate(x, root);
					w = x->left;
				}
				if (w->color == BLACK)
				{
					if (w->left->color == BLACK && w->right->color == BLACK)
					{
						w->color = RED;
					}
					else if (w->left->color == BLACK && w->right->color == RED)
					{
						w->color = RED;
						w->right->color = BLACK;
						left_rotate(w, root);
					}
					else
					{
						w->left->color = BLACK;
						w->color = x->color;
						x->color = BLACK;
						right_rotate(x, root);
						complete = true;
						break;
					}
				}
				if (x->color == BLACK)
					x = x->parent;
			}
			if (x == x->parent->left)
				l_or_r = true;
			else
				l_or_r = false;
		
		}   // ѭ������

		if (!complete)   //ִ�е�������complete��ȻΪfalse��ҪôxΪRED�� ҪôxΪroot
		{
			if (l_or_r)
			{
				x->right->color = RED;
				x->color = BLACK;
			}
			else
			{
				x->left->color = RED;
				x->color = BLACK;
			}

		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
	{
		init();
		tree_node* new_root = _copy(x.root(), header);
		header->parent = new_root;
		node_count = x.node_count;
		key_compare = x.key_compare;
		for (tree_node* cur = root(); cur->left != nil; cur = cur->left)
			header->left = cur;
		for (tree_node* cur = root(); cur->right != nil; cur = cur->right)
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
		while (x != nil)
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
		while (x != nil)
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
		while (x �� = nil)
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
		_erase(pos);
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
		if (root == nil) return;
		destroy_tree(root->left);
		destroy_tree(root->right);
		destroy_node(root);
	}

	
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::left_rotate(tree_node* x, tree_node*& root)
	{   // ֻ��ת����������ɫ��������ɫ������rb_tree_balance����
		tree_node* y = x->right;
		x->right = y->left;
		if (y->left != nil)
		{
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x == root)
		{
			root == y; // 
		}
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->left = x->parent;
		x->parent = y;
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::right_rotate(tree_node* x, tree_node*& root)
	{
		tree_node* y = x->left;
		x->left = y->right;
		if (y->right != nil)
		{
			y->right->parent = x;
		}
		y->parent = x->parent;

		if (x == root)
			root = y;
		else if (x == x->parent->left)
		{
			x->parent->left = y;
		}
		else
		{
			x->parent->right = y;
		}
		y->right = x;
		x->parent = y;
	}

}  // namespace Tiny STL


#endif // !_RB_TREE_IMPL_H_
