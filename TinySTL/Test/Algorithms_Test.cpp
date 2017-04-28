#include "Algorithm_Test.h"

namespace TinySTL
{
		void test_equal()
		{
			int myints[] = { 20, 40, 60, 80, 100 };
			std::vector<int>v(myints, myints + 5);     //20 40 60 80 100
			assert(TinySTL::equal(v.begin(), v.end(), myints));

			v[3] = 81;
			assert(!TinySTL::equal(v.begin(), v.end(), myints, [](int i, int j) {return i == j; }));

		}
		void test_fill()
		{
			std::vector<int> v1(8), v2(8);
			std::fill(v1.begin(), v1.begin() + 4, 5);   //5 5 5 5 0 0 0 0
			std::fill(v1.begin() + 3, v1.end() - 2, 8);   //5 5 5 8 8 8 0 0
			TinySTL::fill(v2.begin(), v2.begin() + 4, 5);   //5 5 5 5 0 0 0 0
			TinySTL::fill(v2.begin() + 3, v2.end() - 2, 8);   //5 5 5 8 8 8 0 0

			assert(TinySTL::Test::container_equal(v1, v2));
		}
		void test_fill_n()
		{
			std::vector<int> v1(8, 10), v2(8, 10);
			std::fill_n(v1.begin(), 4, 20);     //20 20 20 20 10 10 10 10
			std::fill_n(v1.begin() + 3, 3, 33);   //20 20 20 33 33 33 10 10
			TinySTL::fill_n(v2.begin(), 4, 20);     //20 20 20 20 10 10 10 10
			TinySTL::fill_n(v2.begin() + 3, 3, 33);   //20 20 20 33 33 33 10 10

			assert(TinySTL::Test::container_equal(v1, v2));
		}
		void test_max()
		{
			assert(TinySTL::max(1, 2) == 2);
			assert(TinySTL::max(2, 1) == 2);
			assert(TinySTL::max('a', 'z') == 'z');
			assert(TinySTL::max(3.14, 2.73) == 3.14);
		}
		void test_min()
		{
			assert(TinySTL::min(1, 2) == 1);
			assert(TinySTL::min(2, 1) == 1);
			assert(TinySTL::min('a', 'z') == 'a');
			assert(TinySTL::min(3.14, 2.72) == 2.72);
		}
		void test_mismatch()
		{
			std::vector<int> v;
			for (int i = 1; i<6; i++) v.push_back(i * 10); //10 20 30 40 50
			int myints[] = { 10, 20, 80, 320, 1024 };
			TinySTL::pair<std::vector<int>::iterator, int*> mypair;
			mypair = TinySTL::mismatch(v.begin(), v.end(), myints);
			assert(*mypair.first == 30 && *mypair.second == 80);

			++mypair.first; ++mypair.second;
			mypair = TinySTL::mismatch(mypair.first, v.end(), mypair.second, [](int i, int j) {return i == j; });
		}
		void test_copy()
		{
			char arr1[] = "hello", res1[6] = { 0 };
			TinySTL::copy(std::begin(arr1), std::end(arr1), res1);
			assert(TinySTL::Test::container_equal(arr1, res1));

			wchar_t arr2[] = L"hello", res2[6] = { 0 };
			TinySTL::copy(std::begin(arr2), std::end(arr2), res2);
			assert(TinySTL::Test::container_equal(arr2, res2));

			int arr3[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }, res3[10] = { 0 };
			TinySTL::copy(std::begin(arr3), std::end(arr3), res3);
			assert(TinySTL::Test::container_equal(arr3, res3));

			std::string arr4[3] = { "1", "2", "3" }, res4[3];
			TinySTL::copy(std::begin(arr4), std::end(arr4), res4);
			assert(TinySTL::Test::container_equal(arr4, res4));
		}
		void test_heap()
		{
			int myints[] = { 10, 20, 30, 5, 15 };
			std::vector<int> v1(myints, myints + 5);
			std::vector<int> v2(myints, myints + 5);

			std::make_heap(v1.begin(), v1.end());
			TinySTL::make_heap(v2.begin(), v2.end());
			assert(TinySTL::Test::container_equal(v1, v2));

			std::pop_heap(v1.begin(), v1.end()); v1.pop_back();
			TinySTL::pop_heap(v2.begin(), v2.end()); v2.pop_back();
			assert(TinySTL::Test::container_equal(v1, v2));

			v1.push_back(99); std::push_heap(v1.begin(), v1.end());
			v2.push_back(99); TinySTL::push_heap(v2.begin(), v2.end());
			assert(TinySTL::Test::container_equal(v1, v2));

			std::sort_heap(v1.begin(), v1.end());
			TinySTL::sort_heap(v2.begin(), v2.end());
			assert(TinySTL::Test::container_equal(v1, v2));
		}
		void test_count()
		{
			int myints[] = { 10, 20, 30, 30, 20, 10, 10, 20 };   // 8 elements
			int mycount = TinySTL::count(myints, myints + 8, 10);
			assert(mycount == 3);

		}
		void test_find()
		{
			std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			assert(TinySTL::find(v.begin(), v.end(), 5) != v.end());
			assert(TinySTL::find(v.begin(), v.end(), 10) == v.end());
		}
		void test_for_each()
		{
			std::vector<int> myvector{ 10, 20, 30 };
			std::vector<int> temp{ 11, 21, 31 };
			TinySTL::for_each(myvector.begin(), myvector.end(), [&myvector](int& i) {
				++i;
			});
		}
		void test_search()
		{
			std::vector<int> v;
			for (int i = 1; i<10; i++) v.push_back(i * 10);
			int needle1[] = { 40, 50, 60, 70 };
			auto it = TinySTL::search(v.begin(), v.end(), needle1, needle1 + 4);
			assert(it == v.begin() + 3);

			int needle2[] = { 20, 30, 50 };
			it = std::search(v.begin(), v.end(), needle2, needle2 + 3, [](int i, int j) {return i == j; });
			assert(it == v.end());
		}
		void test_sort()
		{
			int arr1[1] = { 0 };
			TinySTL::sort(std::begin(arr1), std::end(arr1));
			assert(std::is_sorted(std::begin(arr1), std::end(arr1)));

			int arr2[2] = { 1, 0 };
			TinySTL::sort(std::begin(arr2), std::end(arr2));
			assert(std::is_sorted(std::begin(arr2), std::end(arr2)));

			int arr3[3] = { 2, 1, 3 };
			TinySTL::sort(std::begin(arr3), std::end(arr3));
			assert(std::is_sorted(std::begin(arr3), std::end(arr3)));

			int arr4[100];
			std::random_device rd;
			for (auto i = 0; i != 10; ++i) {
				for (auto& n : arr4) {
					n = rd() % 65536;
				}
				TinySTL::sort(std::begin(arr4), std::end(arr4));
				assert(std::is_sorted(std::begin(arr4), std::end(arr4)));
			}
		}
		void test_advance()
		{
			TinySTL::vector<int> v;
			TinySTL::list<int> l;
			for (auto i = 0; i != 10; ++i) {
				v.push_back(i);
				l.push_back(i);
			}
			auto vit = v.begin();
			auto lit = l.begin();

			TinySTL::advance(vit, 5);
			TinySTL::advance(lit, 5);
			assert(*vit == 5 && *lit == 5);

			TinySTL::advance(vit, -5);
			TinySTL::advance(lit, -5);
			assert(*vit == 0 && *lit == 0);
		}

		void testAll_algorithm_Cases()
		{
			test_equal();
			test_fill();
			test_fill_n();
			test_max();
			test_min();
			test_mismatch();
			test_copy();
			//test_heap();
			test_count();
			test_find();
			test_for_each();
			test_search();
			test_sort();
		    test_advance();
		}

}