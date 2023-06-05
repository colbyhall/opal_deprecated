// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring_view.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

using namespace eu;

TEST_CASE("eu::core::WStringView") {
	WStringView str = L"Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

EU_TEST_END
