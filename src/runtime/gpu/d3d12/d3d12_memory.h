// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12.h"

#include "core/atomic.h"
#include "core/containers/array.h"

EU_GPU_NAMESPACE_BEGIN

struct D3D12DescriptorHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	u32 index;
};

class D3D12DescriptorHeap {
public:
	explicit D3D12DescriptorHeap() = default;
	void init(ComPtr<ID3D12Device1> device, D3D12_DESCRIPTOR_HEAP_TYPE type, usize cap, bool shader_visible);

	D3D12DescriptorHandle alloc() const;
	void free(const D3D12DescriptorHandle& handle) const;

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;
	Array<Atomic<bool>> m_free_slots;
	usize m_size = 0;
	usize m_cap = 0;
};

EU_GPU_NAMESPACE_END