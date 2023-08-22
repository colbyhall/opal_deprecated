// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/shared.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

class SharedFoo : public SharedFromThis<SharedFoo> {
public:
	virtual int get_value() const { return -1; }
	virtual ~SharedFoo() {}
};

class SharedBar : public SharedFoo {
public:
	SharedBar() = default;
	int get_value() const override { return 100; }
};

TEST_CASE("sf::core::Shared") {
	Shared<SharedFoo> foo = Shared<SharedBar>::make();
	CHECK(foo->get_value() == 100);
	CHECK(foo.strong() == 1);
	CHECK(foo.weak() == 1);

	auto shared = foo->to_shared();
	CHECK(shared->get_value() == 100);
	CHECK(shared.strong() == 2);
	CHECK(shared.weak() == 1);

	auto bar = foo;
	CHECK(bar->get_value() == 100);
	CHECK(bar.strong() == 3);
	CHECK(bar.weak() == 1);
}

SF_TEST_END
