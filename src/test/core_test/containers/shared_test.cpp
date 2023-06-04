// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/shared.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

using namespace eu;

class SharedFoo : public SharedFromThis<SharedFoo, SMode::NonAtomic> {
public:
	virtual int get_value() const { return -1; }
	virtual ~SharedFoo() {}
};

class SharedBar : public SharedFoo {
public:
	SharedBar() = default;
	int get_value() const override { return 100; }
};

TEST_CASE("eu::core::Shared") {
	Shared<SharedFoo> foo = Shared<SharedBar>::make();
	CHECK(foo->get_value() == 100);

	auto shared = foo->to_shared();
	CHECK(shared->get_value() == 100);
}

EU_TEST_END
