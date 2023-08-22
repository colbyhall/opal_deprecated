// Copyright Colby Hall. All Rights Reserved.

#include "gpu/texture.h"

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

GJ_GPU_NAMESPACE_END
