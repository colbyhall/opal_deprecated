// Copyright Colby Hall. All Rights Reserved.

#include "core/os/file_system.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <iostream>
EU_SUPPRESS_WARNINGS_STD_END

auto main(int argc, char** argv) -> int {
	EU_UNUSED(argc);
	EU_UNUSED(argv);

	auto path = eu::cwd();
	EU_UNUSED(path);

	return 0;
}
