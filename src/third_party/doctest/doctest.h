// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN
#include "doctest_internal.h"
SF_SUPPRESS_WARNINGS_STD_END

#define SF_TEST_BEGIN                                                                                                  \
	SF_SUPPRESS_WARNING_PUSH                                                                                           \
	SF_SUPPRESS_WARNINGS                                                                                               \
	using namespace sf;

#define SF_TEST_END SF_SUPPRESS_WARNING_POP

#define TEST_MAIN()                                                                                                    \
	int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
