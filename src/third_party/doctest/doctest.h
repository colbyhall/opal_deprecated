// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include "doctest_internal.h"
EU_SUPPRESS_WARNINGS_STD_END

#define EU_TEST_BEGIN                                                          \
	EU_SUPPRESS_WARNING_PUSH                                                   \
	EU_SUPPRESS_WARNINGS

#define EU_TEST_END EU_SUPPRESS_WARNING_POP

#define TEST_MAIN()                                                            \
	int main(int argc, char** argv) {                                          \
		return doctest::Context(argc, argv).run();                             \
	}
