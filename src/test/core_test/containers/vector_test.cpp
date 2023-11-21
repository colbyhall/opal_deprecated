// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/vector.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

TEST_CASE("op::core::Vector") {
	Vector<int> vec;

	CHECK(vec.len() == 0); // op::Vector default initialization is to zero

	SUBCASE("Adding elements") {
		vec.push(5);
		vec.push(10);
		vec.push(15);

		REQUIRE(vec.len() == 3); // We've added three elements
		CHECK(vec[0] == 5);		 // The elements should be in the order we added them
		CHECK(vec[1] == 10);
		CHECK(vec[2] == 15);

		auto foo = vec;
		OP_UNUSED(foo);
	}

	SUBCASE("Removing elements") {
		vec.push(5);
		vec.push(10);
		vec.push(15);

		auto popped = vec.pop();
		REQUIRE(popped.is_set()); // Option could be null and cause assert

		const auto popped_value = popped.unwrap();
		CHECK(popped_value == 15);
		CHECK(vec.len() == 2);

		const auto first = vec.remove(0);
		CHECK(first == 5);
		CHECK(vec.len() == 1);

		vec.reset();
		CHECK(vec.is_empty());
	}

	SUBCASE("Range operators") {
		vec.push(5);
		vec.push(10);
		vec.push(15);
		REQUIRE(vec.len() == 3); // We've added three elements

		int index = 0;
		for (int value : vec) {
			CHECK(vec[index] == value);
			index += 1;
		}
	}
}

OP_TEST_END
