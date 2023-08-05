// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

using namespace gj;

TEST_CASE("gj::core::String") {
	String str;
	str.push("Hello World!");

	REQUIRE(str.len() == 12);
	REQUIRE(*str.chars() == 'H');

	const StringView other = "Hello World!";
	REQUIRE(other == str);
}

GJ_TEST_END
