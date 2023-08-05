// Copyright Colby Hall. All Rights Reserved.

#include "gpu/texture.h"
#include "gpu/context.h"

#include "gpu/d3d12/d3d12_texture.h"

EU_GPU_NAMESPACE_BEGIN

Texture Texture::make(Usage usage, Format format, const Vec3u32& size) {
	auto& context = Context::the();

	Option<Shared<ITexture>> interface;
	switch (context.backend()) {
	case Backend::D3D12: {
		interface = Shared<D3D12TextureImpl>::make(usage, format, size);
	} break;
	default:
		EU_UNIMPLEMENTED;
		break;
	}

	return Texture{ interface.unwrap() };
}

EU_GPU_NAMESPACE_END