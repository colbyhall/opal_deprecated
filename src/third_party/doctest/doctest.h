// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

OP_SUPPRESS_WARNINGS_STD_BEGIN
#include "doctest_internal.h"
OP_SUPPRESS_WARNINGS_STD_END

#define OP_TEST_BEGIN                                                                                                  \
	OP_SUPPRESS_WARNING_PUSH                                                                                           \
	OP_SUPPRESS_WARNINGS                                                                                               \
	using namespace op;

#define OP_TEST_END OP_SUPPRESS_WARNING_POP

#define TEST_MAIN()                                                                                                    \
	int main(int argc, char** argv) { return doctest::Context(argc, argv).run(); }
