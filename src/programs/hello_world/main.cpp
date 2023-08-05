// Copyright Colby Hall. All Rights Reserved.

#include "core/core.h"

#include "gpu/buffer.h"
#include "gpu/context.h"
#include "gpu/texture.h"

using namespace gj;

auto main(int argc, char** argv) -> int {
	GJ_UNUSED(argc);
	GJ_UNUSED(argv);

	gpu::init();

	using namespace gpu;

	using Usage = Buffer::Usage;
	using Kind = Buffer::Kind;
	auto buffer = Buffer::make(Usage::Vertex | Usage::Constant, Kind::Upload, 256 * KB);

	auto texture = Texture::make(Texture::Usage::Color, Format::RGBA_U8, { 512, 512, 1 });

	return 0;
}
