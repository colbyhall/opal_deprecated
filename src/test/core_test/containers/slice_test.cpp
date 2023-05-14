// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/slice.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

TEST_CASE("eu::core::Slice") {
	int test_array[] = {1, 2, 3, 4, 5};
	eu::usize array_len = sizeof(test_array) / sizeof(test_array[0]);

	eu::Slice<int> s(test_array, array_len);

	SUBCASE("Testing constructors and copy assignment") {
		eu::Slice<int> s_copy(s);
		CHECK(s_copy.begin() == s.begin());
		CHECK(s_copy.len() == s.len());
	}

	SUBCASE("Testing move assignment") {
		eu::Slice<int> s_moved(eu::move(s));
		CHECK(s_moved.begin() == test_array);
		CHECK(s_moved.len() == array_len);
	}

	SUBCASE("Testing len() and is_empty()") {
		CHECK(s.len() == array_len);
		CHECK(!s.is_empty());
	}

	SUBCASE("Testing is_valid_index()") {
		CHECK(s.is_valid_index(0));
		CHECK(!s.is_valid_index(array_len));
	}

	SUBCASE("Testing explicit operator bool()") { CHECK(static_cast<bool>(s)); }

	SUBCASE("Testing range accessors") {
		CHECK(s.begin() == test_array);
		CHECK(s.end() == test_array + array_len);
		CHECK(s.cbegin() == test_array);
		CHECK(s.cend() == test_array + array_len);
	}

	SUBCASE("Testing element access") {
		for (eu::usize i = 0; i < array_len; ++i) {
			CHECK(s[i] == test_array[i]);
		}
	}

	SUBCASE("Testing const slice") {
		eu::Slice<const int> s_const(test_array, array_len);
		CHECK(s_const.len() == array_len);
		CHECK(!s_const.is_empty());
		CHECK(s_const.begin() == test_array);
		CHECK(s_const.end() == test_array + array_len);
		CHECK(s_const.cbegin() == test_array);
		CHECK(s_const.cend() == test_array + array_len);

		for (eu::usize i = 0; i < array_len; ++i) {
			CHECK(s_const[i] == test_array[i]);
		}
	}

	SUBCASE("Testing initializer_list constructor") {
		eu::Slice<const int> s_initializer({1, 2, 3, 4, 5});
		CHECK(s_initializer.len() == 5);
		CHECK(!s_initializer.is_empty());

		for (eu::usize i = 0; i < s_initializer.len(); ++i) {
			CHECK(s_initializer[i] == test_array[i]);
		}
	}
}

EU_TEST_END
