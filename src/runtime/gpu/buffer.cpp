// Copyright Colby Hall. All Rights Reserved.

#include "gpu/buffer.h"
#include "gpu/context.h"

#include "gpu/d3d12/d3d12_buffer.h"

GJ_GPU_NAMESPACE_BEGIN

Buffer Buffer::make(Usage usage, Kind kind, usize len) {
	auto& context = Context::the();

	Option<Shared<IBuffer>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Shared<D3D12BufferImpl>::make(usage, kind, len);
		break;
	default:
		GJ_UNIMPLEMENTED;
		break;
	}

	return Buffer{ interface.unwrap() };
}

GJ_GPU_NAMESPACE_END