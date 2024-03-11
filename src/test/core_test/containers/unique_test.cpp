// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/unique.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

class Foo {
public:
	virtual int get_value() const { return -1; }
	virtual ~Foo() {}
};

class Bar : public Foo {
public:
	Bar() = default;
	Bar(const Bar&) {}
	int get_value() const override { return 100; }
};

TEST_CASE("op::core::Unique") {
	auto foo = op::Unique<Bar>::make();
	CHECK(foo->get_value() == 100);

	auto bar = foo;
	CHECK(bar->get_value() == 100);
}

OP_TEST_END
