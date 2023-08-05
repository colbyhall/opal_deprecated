// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring_view.h"
#include "doctest/doctest.h"

GJ_TEST_BEGIN

using namespace gj;

TEST_CASE("gj::core::WStringView") {
	WStringView str = L"Hello World!";

	REQUIRE(str.len() == 12);
	REQUIRE(str[0] == 'H');
}

GJ_TEST_END
