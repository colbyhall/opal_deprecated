// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string_view.h"
#include "doctest/doctest.h"

EU_TEST_BEGIN

using namespace eu;

TEST_CASE("eu::core::StringView") {
	const StringView str = "Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(*str.chars() == 'H');

	const StringView other = "Hello World!";
	REQUIRE(other == str);
}

EU_TEST_END
