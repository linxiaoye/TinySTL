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

	}
	template<class Value, class Ref, class Ptr>
	void rb_tree_iterator<Value, Ref, Ptr>::decrease()
	{

	}

	/******************** class rb_tree **************************************/

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_insert(tree_node* x, tree_node* y, const value_type& x) -> iterator
	{

	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	auto rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_copy(tree_node* x, tree_node* y) -> tree_node*
	{

	}
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_erase(tree_node* p)
	{

	}


}  // namespace Tiny STL


#endif // !_RB_TREE_IMPL_H_
