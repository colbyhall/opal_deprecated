// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string.h"
#include "doctest/doctest.h"

SF_TEST_BEGIN

TEST_CASE("sf::core::String") {
	String str;
	str.push("Hello World!");

	REQUIRE(str.len() == 12);
	REQUIRE(*str.chars() == 'H');

	const StringView other = "Hello World!";
	REQUIRE(other == str);
}

SF_TEST_END
