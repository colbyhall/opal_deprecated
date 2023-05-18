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
	eu::Unique<Foo> foo = eu::make_unique<Bar>();
	CHECK(foo->get_value() == 100);
}

EU_TEST_END
