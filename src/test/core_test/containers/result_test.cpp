// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/result.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

enum class Error : u32 { Foo, Bar, Baz };

TEST_CASE("op::core::Result") {
	SUBCASE("Value") {
		Result<u32, Error> result = 42;
		REQUIRE(result.is_ok());
		const auto value = result.unwrap();
		CHECK(value == 42);
	}

	SUBCASE("Error") {
		Result<u32, Error> result = Error::Foo;
		CHECK(!result.is_ok());
		const auto value = result.unwrap_err();
		CHECK(value != Error::Bar);
		CHECK(value != Error::Baz);
		CHECK(value == Error::Foo);
	}
}

OP_TEST_END
