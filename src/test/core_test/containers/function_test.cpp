// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/function.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

TEST_CASE("gj::core::Function, gj::core::FunctionRef") {
	FunctionRef<bool()> foo = []() { return false; };
	const auto a = foo();
	CHECK(a == false);

	Function<bool()> bar = []() { return true; };
	auto baz = gj::move(bar);
	const auto b = baz();
	CHECK(b == true);

	int c = 0;
	int d = 12;
	FunctionRef<void(int& d)> car = [&c](auto& d) { d += c; };
	car(d);
	CHECK(c == d);
}

GJ_TEST_END
