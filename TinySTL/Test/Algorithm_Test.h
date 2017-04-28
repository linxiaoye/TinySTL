#ifndef _ALGORITHM_TEST_H_
#define _ALGORITHM_TEST_H_




#include "../Algorithm.h"
#include "test_fun.h"
#include "../Vector.h"
#include "../List.h"
#include <algorithm>

#include <array>
#include <cctype>
#include <cstring>
#include <cassert>
#include <functional>
#include <list>
#include <random>
#include <vector>


namespace TinySTL
{
		void test_equal();
		void test_fill();
		void test_fill_n();
		void test_max();
		void test_min();
		void test_mismatch();
		void test_copy();
		void test_heap();
		void test_count();
		void test_find();
		void test_for_each();
		void test_search();
		void test_sort();
		void test_advance();

		void testAll_algorithm_Cases();
}

#endif // !_ALGORITHM_TEST_H_
