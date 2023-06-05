// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

using namespace eu;

TEST_CASE("eu::core::WString") {
	WString str;
	str.push(L"Hello World!");

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

EU_TEST_END
