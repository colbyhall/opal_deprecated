// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/slice.h"
#include "doctest/doctest.h"

using namespace eu;

TEST_CASE("eu::core::Slice") {
	// Can only hold a const reference to whats in the initializer list
	Slice<const int> foo = {1, 2, 3, 4};
	REQUIRE(foo.len() == 4);
	REQUIRE(!foo.is_empty());

	bool success = true;
	for (usize index = 0; index < 4; ++index) {
		if (foo[index] != static_cast<int>(index) + 1) {
			success = false;
		}
	}
	CHECK(success);
}
