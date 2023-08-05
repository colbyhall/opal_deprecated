// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/unique.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

class Foo {
public:
	virtual int get_value() const { return -1; }
	virtual ~Foo() {}
};

class Bar : public Foo {
public:
	Bar() = default;
	int get_value() const override { return 100; }
};

TEST_CASE("eu::core::Unique") {
	auto foo = eu::Unique<Bar>::make();
	CHECK(foo->get_value() == 100);

	auto bar = foo;
	CHECK(bar->get_value() == 100);
}

EU_TEST_END
