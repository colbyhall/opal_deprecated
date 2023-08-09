// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12.h"

#include "core/atomic.h"
#include "core/containers/vector.h"

GJ_GPU_NAMESPACE_BEGIN

struct D3D12DescriptorHandle {
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	u32 index;
};

class D3D12DescriptorHeap {
public:
	explicit D3D12DescriptorHeap() = default;
	void init(
		ComPtr<ID3D12Device1> device,
		D3D12_DESCRIPTOR_HEAP_TYPE type,
		usize cap,
		bool shader_visible
	);

	D3D12DescriptorHandle alloc() const;
	void free(const D3D12DescriptorHandle& handle) const;

	GJ_ALWAYS_INLINE ComPtr<ID3D12DescriptorHeap> the() const { return m_heap; }

private:
	ComPtr<ID3D12DescriptorHeap> m_heap;
	Vector<Atomic<bool>> m_free_slots;
	usize m_size = 0;
	usize m_cap = 0;
};

class D3D12ContextImpl;

class D3D12RootSignatureImpl {
public:
	static inline constexpr u32 rtv_heap_size = 2048;
	static inline constexpr u32 dsv_heap_size = 2048;
	static inline constexpr u32 bt2dv_heap_size = 2048;
	static inline constexpr u32 bt2dv_index = 1;

	D3D12RootSignatureImpl() = default;
	void init(const D3D12ContextImpl& context);

	GJ_ALWAYS_INLINE ComPtr<ID3D12RootSignature> the() const {
		return m_root_signature;
	}

	GJ_ALWAYS_INLINE const D3D12DescriptorHeap& rtv_heap() const {
		return m_rtv_heap;
	}
	GJ_ALWAYS_INLINE const D3D12DescriptorHeap& dsv_heap() const {
		return m_dsv_heap;
	}
	GJ_ALWAYS_INLINE const D3D12DescriptorHeap& bt2dv_heap() const {
		return m_bt2dv_heap;
	}

private:
	ComPtr<ID3D12RootSignature> m_root_signature;
	// Render Target View the
	D3D12DescriptorHeap m_rtv_heap;
	// Depth Stencil View the
	D3D12DescriptorHeap m_dsv_heap;
	// Bindless Texture2D View the
	D3D12DescriptorHeap m_bt2dv_heap;
};

GJ_GPU_NAMESPACE_END