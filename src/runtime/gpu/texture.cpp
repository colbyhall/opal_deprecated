// Copyright Colby Hall. All Rights Reserved.

#include "gpu/texture.h"
#include "gpu/context.h"

#include "gpu/d3d12/d3d12_texture.h"

GJ_GPU_NAMESPACE_BEGIN

usize format_size_in_bytes(Format format) {
	switch (format) {
	case Format::R_U8:
		return 1;
	case Format::RGBA_U8:
	case Format::RGBA_U8_SRGB:
		return 4;
	case Format::RGBA_F16:
		return 4 * 2;
	case Format::RGBA_F32:
		return 4 * 4;
	case Format::D24S8_U32:
		return 4;
	case Format::Undefined:
		GJ_INVALID_CODE_PATH;
		break;
	}

	return 0;
}

Texture Texture::make(Usage usage, Format format, const Vector3<u32>& size) {
	auto& context = Context::the();

	Option<Shared<ITexture>> interface;
	switch (context.backend()) {
	case Backend::D3D12: {
		interface = Shared<D3D12TextureImpl>::make(usage, format, size);
	} break;
	default:
		GJ_UNIMPLEMENTED;
		break;
	}

	return Texture{ interface.unwrap() };
}

GJ_GPU_NAMESPACE_END
