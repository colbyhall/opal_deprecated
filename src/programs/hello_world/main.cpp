// Copyright Colby Hall. All Rights Reserved.

#include "core/core.h"

#include "gpu/buffer.h"
#include "gpu/context.h"
#include "gpu/texture.h"

using namespace eu;

auto main(int argc, char** argv) -> int {
	EU_UNUSED(argc);
	EU_UNUSED(argv);

	gpu::init();

	using namespace gpu;

	using Usage = Buffer::Usage;
	using Kind = Buffer::Kind;
	auto buffer = Buffer::make(Usage::Vertex | Usage::Constant, Kind::Upload, 256 * KB);

	auto texture = Texture::make(Texture::Usage::Color, Format::RGBA_U8, { 512, 512, 1 });

	return 0;
}
