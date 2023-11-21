// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring_view.h"
#include "doctest/doctest.h"

OP_TEST_BEGIN

TEST_CASE("op::core::WStringView") {
	WStringView str = L"Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

OP_TEST_END
