// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/function.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

TEST_CASE("eu::core::Function, eu::core::FunctionRef") {
	using namespace eu;
	FunctionRef<bool()> foo = []() { return false; };
	const auto a = foo();
	CHECK(a == false);

	Function<bool()> bar = []() { return true; };
	auto baz = eu::move(bar);
	const auto b = baz();
	CHECK(b == true);
}

EU_TEST_END
