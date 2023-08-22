// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

TEST_CASE("sf::core::WString") {
	WString str;
	str.push(L"Hello World!");

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

SF_TEST_END
