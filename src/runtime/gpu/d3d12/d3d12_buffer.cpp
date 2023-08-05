// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/d3d12_buffer.h"
#include "gpu/d3d12/d3d12_context.h"

EU_GPU_NAMESPACE_BEGIN

D3D12BufferImpl::D3D12BufferImpl(Usage usage, Kind kind, usize len) : m_usage(usage), m_kind(kind), m_len(len) {
	auto& context = Context::the().interface<D3D12ContextImpl>();

	D3D12_HEAP_PROPERTIES heap = {};
	switch (kind) {
	case Kind::Storage:
		heap.Type = D3D12_HEAP_TYPE_DEFAULT;
		break;
	case Kind::Upload:
		heap.Type = D3D12_HEAP_TYPE_UPLOAD;
		break;
	case Kind::Download:
		heap.Type = D3D12_HEAP_TYPE_READBACK;
		break;
	}

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = len;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	const D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_GENERIC_READ;

	throw_if_failed(context.device()->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		initial_state,
		nullptr,
		IID_PPV_ARGS(&m_resource)
	));
}

void D3D12BufferImpl::map(FunctionRef<void(Slice<u8>)>& func) {
	D3D12_RANGE range = {};
	void* ptr;
	throw_if_failed(m_resource->Map(0, &range, &ptr));
	auto slice = Slice{ reinterpret_cast<u8*>(ptr), m_len };
	func(slice);
	m_resource->Unmap(0, nullptr);
}

EU_GPU_NAMESPACE_END