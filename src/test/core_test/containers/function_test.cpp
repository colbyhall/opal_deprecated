// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/function.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

TEST_CASE("op::core::Function, op::core::FunctionRef") {
	FunctionRef<bool()> foo = []() { return false; };
	const auto a = foo();
	CHECK(a == false);

	Function<bool()> bar = []() { return true; };
	auto baz = op::move(bar);
	const auto b = baz();
	CHECK(b == true);

	int c = 0;
	int d = 12;
	FunctionRef<void(int& d)> car = [&c](auto& d) { d += c; };
	car(d);
	CHECK(d == 12);
}

OP_TEST_END
