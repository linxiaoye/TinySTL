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
		if (node->right != nullptr)  // 1：右节点存在，则至右分支的最小节点（最左节点）
		{
			node = node->right;
			while (node->left != nullptr)
				node = node->left;
		}
		else                        // 右节点不存在
		{
			tree_node* p = node->parent;
			if (p->parent == node)
				node = p;          // 2：若node为根节点，则至header
			else if (p->left == node)
				node = p;          // 3：若node为父节点的左节点，那至其父节点
			else
			{
				while (p->right == node)
				{
					node = p;
					p = p->parent;  
				}                    
				node = p;           // 4:若node为父节点的右节点，则向上至node为父节点的左节点为止
			}
		}
	}
	template<class Value, class Ref, class Ptr>
	void rb_tree_iterator<Value, Ref, Ptr>::decrease()
	{
		if (node->color == RED && node->parent->parent == node)
			node = node->right;  // 1:如果node为header，那么至rightmost
		if (node->left != nullptr)
		{
			node = node->left;
			while (node->right != nullptr)
				node = node->right;
		}                        // 2：如果左节点存在，那么至左分支的最大节点（最左节点）
		else     // 若无左节点
		{
			tree_node* p = node->parent;
			if (p->right == node)
				node = p;     // 3：如果node为父节点的右节点，则至其父节点
			else
			{
				while (p->left == node)
				{
					node = p;
					p = p->parent;
				}
				node = p;    // 4：如果node为父节点的左节点，则向上至node为父节点的右节点为止
			}
		}
	}

	/******************** class rb_tree **************************************/

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(tree_node* x, tree_node* y, const value_type& x) -> iterator
	{
		tree_node* z;
		if (y == header || x != nil || key_compare(KeyOfValue()(val), key(x)))
		{ // 如果要插入节点的父节点为header或者要插入的节点不为空或者val的key比x的key小
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
		// 下面调整红黑树，维持红黑树的特性
		rb_insert_balance(z, root());  // 第一个参数为新增节点，第二个参数为root
		++node_count;
		return iterator(z);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_insert_balance(tree_node* x, tree_node*& root)
	{
		x->color = RED; // 新建节点未调整前永远为红
		while (x != root && x->parent->color == RED) // 当x不为root且新建节点的父节点为RED是才需要调整
		{
			if (x->parent == x->parent->parent->left) // 当x的父节点为其父节点的左节点时
			{
				tree_node* y = x->parent->parent->right; //y为x的伯父节点
				if (y != nil && y->color == RED)  //如果伯父节点存在且为红，则直接调整颜色就行了
				{
					x->parent->parent->color = RED;
					x->parent->color = BLACK;
					y->color = BLACK;
					x = x->parent->parent;
				}
				else  // 否则，即伯父节点不存在或者为黑
				{
					if (x == x->parent->right) // 如果x是其父节点的右儿子，则先左旋调整为左儿子
					{
						x = x->parent;
						left_rotate(x, root);
					}
					x->parent->color = BLACK;  // 到这里之后，就只有一种情况，即x和其父节点都是在左边且都是RED
					x->parent->parent->color = RED;
					right_rotate(x->parent->parent, root); // 调整颜色并右旋就好了
				}
			}
			else  // 情况和上面的基本对称，当x的父节点为其父节点的右节点
			{
				tree_node* y = x->parent->parent->left;
				if (y != nil && y->color == RED)  //当伯父节点为RED
				{
					y->color = BLACK;
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					x = x->parent->parent;
				}
				else //伯父节点不为RED
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
		root->color = BLACK; //根节点永远为黑
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(tree_node* x, tree_node* y) -> tree_node*
	{   // x为被copy的节点， y已copy的新节点的父节点
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
		if (x->left == nil && x->right == nil && x->color == RED) //1:如果x为叶节点且为RED，直接删除
		{
			if (x->parent->left == x) // 判断x是其父节点的左节点还是右节点，因为要改变其父节点的孩子指针
				x->parent->left = nil;
			else
				x->parent->right = nil;
			destroy_node(x);
		} 
		else if (x->right == nil && x->right == nil && x->color == BLACK) //2:x位叶节点，且为BLACK，那么转化为双黑问题
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
			rb_erase_balance(left_or_right, x->parent, root());  // 调整程序，第一个参数为，被删除节点的父节点
			destroy_node(x);
		}
		else if (x->left != nil && x->right == nil) //3:x只有左节点无右节点，单孩子情况，x只能为BLACK
		{
			if (x->left->color == RED) // 如果x的左孩子为RED，那么直接将左孩子顶替x的位置，并将其置为BLACK
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
			else  // 如果左孩子为BLACK，那么就是 “双黑问题”：要删除的黑节点x有一个黑色的孩子节点
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
				rb_erase_balance(left_or_right, x->parent, root());  // 需要调整
				destroy_node(x);
			}
		}
		else if (x->right != nil && x->left == nil) // 4:x只有右孩子节点没有左孩子节点
		{
			if (x->right->color == RED) // 如果右孩子节点为RED，那么直接将右孩子节点顶替x的位置，然后置为BLACK
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
			else  // 如果x的右孩子节点为BLACK，那么又是双黑问题，即要删除的黑色节点x有一个黑色的孩子节点
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
		else  // 5：x既有左孩子又有右孩子，那么先用二叉搜索树的删除方式，即找右支最小节点顶替x，转化为1，2，3，4情况
		{
			tree_node* y = nil;
			y = x->right;
			while (y->left != nil)           // 寻找顶替x节点的y节点
			{
				y = y->left;
			} 
			value_type tmp = x->value_field;   // 只交换x，y的value，不交换颜色
			x->value_field = y->value_field;
			y->value_field = tmp;
			_erase(y);                              // 至此，已经把情况5转为情况1，2，3，4
		}

	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_erase_balance(bool l_or_r, tree_node* x, tree_node*& root)
	{
		bool complete = false;
		while (x != root && x->color == BLACK)  // 执行到根节点或者x为RED，若x为RED，那么直接把x变为BLACK就行了。
		{
			if (l_or_r == true)  // left is true
			{
				tree_node* w = x->right;
				if (w->color == RED)  // 1:红兄问题,将问题转化为黑兄问题2，3，4
				{
					x->color = RED;
					x->right->color = BLACK;
					left_rotate(x, root);
					w = x->right;
				}
				            /* 以下为黑兄问题 */
				if (w->color == BLACK)   
				{
					if (w->left->color == BLACK && w->right->color == BLACK) // 2:如果兄弟w为BLACK，且其左孩子右孩子均为BLACK，则置w为RED
					{
						w->color = RED;   
					}
					else if (w->left->color == RED && w->right->color == BLACK)
					{
						w->color = RED;                         //3:如果w为BLACK，且左侄为RED，右侄为BLACK
						w->left->color = BLACK;                      // 则转为4
						right_rotate(w);
					}
					else                                // 4:w为BLACK，左侄为BLACK，右侄为RED
					{
						w->right->color = BLACK;
						w->color = x->color;
						x->color = BLACK;
						left_rotate(x, root);
						complete = true;
						break;	
					}
				}
				/* 进行到这里，x要么为RED，要么为BLACK，但是x的左右子树已经平衡 */
				/* 如果x为红，那么将会退出循环，在接下来执行x的颜色变为BLACK，结束 */
				/* 如果为黑，那么将会导致x为x的兄弟直接黑色高度差1，那么将x置为x的parent*/
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
		
		}   // 循环结束

		if (!complete)   //执行到这里，如果complete仍然为false，要么x为RED， 要么x为root
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
			if (!key_compare(key(x), key)) //如果key小于x节点的key
			{
				y = x;
				x = x->left;
			}
			else    // 如果key大于等于x节点的key
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
		return _insert(x, y, v); // x为要插入的节点位置，y为要插入的节点的父节点
	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& val) -> pair<iterator, bool>
	{
		tree_node* x = root();
		tree_node* y = header;
		bool comp = true;
		while (x ！ = nil)
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
	{   // 只旋转，不进行颜色调整，颜色调整在rb_tree_balance里面
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
