// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string_view.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

TEST_CASE("gj::core::StringView") {
	const StringView str = "Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(*str.chars() == 'H');

	const StringView other = "Hello World!";
	REQUIRE(other == str);
}

GJ_TEST_END
