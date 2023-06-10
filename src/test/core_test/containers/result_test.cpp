// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/result.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

using namespace eu;

enum class Error : u32 { Foo, Bar, Baz };

TEST_CASE("eu::core::Result") {
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

EU_TEST_END
