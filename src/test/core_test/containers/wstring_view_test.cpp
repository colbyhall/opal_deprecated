// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring_view.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

TEST_CASE("sf::core::WStringView") {
	WStringView str = L"Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

SF_TEST_END
