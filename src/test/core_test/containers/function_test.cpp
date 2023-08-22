// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/function.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

TEST_CASE("sf::core::Function, sf::core::FunctionRef") {
	FunctionRef<bool()> foo = []() { return false; };
	const auto a = foo();
	CHECK(a == false);

	Function<bool()> bar = []() { return true; };
	auto baz = sf::move(bar);
	const auto b = baz();
	CHECK(b == true);

	int c = 0;
	int d = 12;
	FunctionRef<void(int& d)> car = [&c](auto& d) { d += c; };
	car(d);
	CHECK(c == d);
}

SF_TEST_END
