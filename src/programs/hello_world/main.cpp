// Copyright Colby Hall. All Rights Reserved.

#include "core/core.h"

#include "gpu/context.h"

using namespace eu;

auto main(int argc, char** argv) -> int {
	EU_UNUSED(argc);
	EU_UNUSED(argv);

	gpu::init();

	return 0;
}
