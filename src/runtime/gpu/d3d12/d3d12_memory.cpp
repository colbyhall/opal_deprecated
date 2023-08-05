// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/d3d12_memory.h"

EU_GPU_NAMESPACE_BEGIN

void D3D12DescriptorHeap::init(
	ComPtr<ID3D12Device1> device,
	D3D12_DESCRIPTOR_HEAP_TYPE type,
	usize cap,
	bool shader_visible
) {
	m_cap = cap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = (UINT)cap;
	desc.Type = type;

	// Use bool because
	// D3D12_DESCRIPTOR_HEAP_FLAG_NONE = 0
	// D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE = 1
	desc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)shader_visible;

	throw_if_failed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)));

	m_size = device->GetDescriptorHandleIncrementSize(type);
	m_free_slots.reserve(cap);
	// TODO: memcpy this
	for (usize i = 0; i < cap; ++i) {
		m_free_slots.push(false);
	}
}

D3D12DescriptorHandle D3D12DescriptorHeap::alloc() const {
	EU_ASSERT(m_cap > 0, "Heap needs to be initialized with D3D12DescriptorHeap::init");

	while (true) {
		for (usize i = 0; i < m_cap; ++i) {
			const auto slot = m_free_slots[i].load();
			if (!slot) {
				if (m_free_slots[i].compare_exchange_weak(false, true)) {
					D3D12_CPU_DESCRIPTOR_HANDLE handle = m_heap->GetCPUDescriptorHandleForHeapStart();
					handle.ptr += static_cast<SIZE_T>(i * m_size);
					return { handle, static_cast<u32>(i) };
				}
			}
		}
	}
}

void D3D12DescriptorHeap::free(const D3D12DescriptorHandle& handle) const {
	EU_ASSERT(m_cap > 0, "Heap needs to be initialized with D3D12DescriptorHeap::init");
	EU_ASSERT(m_free_slots[handle.index].load(), "Attempting to free a freed slot");

	m_free_slots[handle.index].store(false);
}

EU_GPU_NAMESPACE_END