// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/array.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

TEST_CASE("eu::core::Array") {
	eu::Array<int> arr;

	CHECK(arr.len() == 0); // eu::Array default initialization is to zero

	SUBCASE("Adding elements") {
		arr.push(5);
		arr.push(10);
		arr.push(15);

		REQUIRE(arr.len() == 3); // We've added three elements
		CHECK(arr[0] == 5); // The elements should be in the order we added them
		CHECK(arr[1] == 10);
		CHECK(arr[2] == 15);
	}

	SUBCASE("Removing elements") {
		arr.push(5);
		arr.push(10);
		arr.push(15);

		auto popped = arr.pop();
		REQUIRE(popped.is_set()); // Option could be null and cause assert

		const auto popped_value = popped.unwrap();
		CHECK(popped_value == 15);
		CHECK(arr.len() == 2);

		const auto first = arr.remove(0);
		CHECK(first == 5);
		CHECK(arr.len() == 1);

		arr.reset();
		CHECK(arr.is_empty());
	}

	SUBCASE("using range") {
		arr.push(5);
		arr.push(10);
		arr.push(15);
		REQUIRE(arr.len() == 3); // We've added three elements

		int index = 0;
		for (int value : arr) {
			CHECK(arr[index] == value);
			index += 1;
		}
	}
}

EU_TEST_END
