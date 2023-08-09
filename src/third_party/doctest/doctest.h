// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

GJ_SUPPRESS_WARNINGS_STD_BEGIN
#include "doctest_internal.h"
GJ_SUPPRESS_WARNINGS_STD_END

#define GJ_TEST_BEGIN                                                                                                  \
	GJ_SUPPRESS_WARNING_PUSH                                                                                           \
	GJ_SUPPRESS_WARNINGS                                                                                               \
	using namespace gj;

#define GJ_TEST_END GJ_SUPPRESS_WARNING_POP

#define TEST_MAIN()                                                                                                    \
	int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
