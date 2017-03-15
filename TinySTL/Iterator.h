#ifndef _ITERATOR_H_
#define _ITERATOR_H_

/*
*     Author  @linxiaoye
*/

namespace TinySTL {
	
	struct input_iterator_tag { };
	struct output_iterator_tag { };
	struct forward_iterator_tag : public input_iterator_tag { };
	struct bidirectional_iterator_tag : public forward_iterator_tag { };
	struct random_access_iterator_tag : public bidirectional_iterator_tag { };
	
	/*  iterator   */
	template<class Category, class T, class Distance = ptrdiff_t, class point = T*, 
	         class Reference = T&>
	struct iterator {
		typedef Category    iterator_category;
		typedef T           value_type;
		typedef Distance    different_type;
		typedef T*          pointer;
		typedef T&          reference;
	};
	
	template<class T, class Distance>
	struct input_iterator {             /* input_iterator */ 
		typedef input_iterator_tag  iterator_category;
		typedef T                   value_type;
		typedef Distance            difference_type;
		typedef T*                  pointer;
		typedef T&                  reference;
	};
	template<class T, class Distance>
	struct output_iterator {             /* output_iterator */ 
		typedef output_iterator_tag  iterator_category;
		typedef void                 value_type;
		typedef void                 difference_type;
		typedef void                 pointer;
		typedef void                 reference;
	};
	template<class T, class Distance>
	struct forward_iterator {             /* forward_iterator */ 
		typedef forward_iterator_tag  iterator_category;
		typedef T                     value_type;
		typedef Distance              difference_type;
		typedef T*                    pointer;
		typedef T&                    reference;
	};
	
	template<class T, class Distance>
	struct bidirectional_iterator {        /* bidirectional_iterator */ 
		typedef bidirectional_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef Distance                    difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	
	template<class T, class Distance>
	struct random_access_iterator {        /* random_access_iterator */
		typedef random_access_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef Distance                    difference_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	
	template<class I>
	struct iterator_traits {             /*  iterator_traits  */
		typedef typename I::iterator_category   iterator_category;
		typedef typename I::value_type          value_type;
		typedef typename I::different_type      different_type;
		typedef typename I::pointer             pointer;
		typedef typename I::reference           reference;
	};
	template<class T>            /* 针对原生指针的特化版本 */
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef ptrdiff_t                   different_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	template<class T>
	struct iterator_traits<const T*> {  /* 针对常量原始指针，为萃取出非const版本型别*/
		typedef random_access_iterator_tag  iterator_category;
		typedef T                           value_type;
		typedef ptrdiff_t                   different_type;
		typedef T*                          pointer;
		typedef T&                          reference;
	};
	
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator& It) {
			typedef typename iterator_traits<Iterator>::iterator_category category;
			return category();
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::different_type* different_type(const Iterator& It) {
		return static_cast<typename iterator_traits<Iterator>::different_type*>(0);
	}
		
}   //  namespace TinySTL

#endif  // _ITERATOR_H_
