// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/buffer.h"

OP_GPU_NAMESPACE_BEGIN

void D3D12Buffer::map(FunctionRef<void(Slice<u8>)> func) {
	D3D12_RANGE range = {};
	void* ptr;
	throw_if_failed(m_resource->Map(0, &range, &ptr));
	auto slice = Slice{ reinterpret_cast<u8*>(ptr), m_size };
	func(slice);
	m_resource->Unmap(0, nullptr);
}

OP_GPU_NAMESPACE_END
