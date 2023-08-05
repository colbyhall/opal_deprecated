// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/context.h"
#include "gpu/d3d12/d3d12_memory.h"

#include "core/containers/option.h"
#include "core/os/library.h"

EU_GPU_NAMESPACE_BEGIN

typedef HRESULT(__stdcall* PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)(
	const D3D12_ROOT_SIGNATURE_DESC* root_signature_desc,
	D3D_ROOT_SIGNATURE_VERSION version,
	ID3DBlob** blob,
	ID3DBlob** error_blob
);

class D3D12ContextImpl final : public IContext {
public:
	explicit D3D12ContextImpl();

	// IContext
	Backend backend() const final { return Backend::D3D12; }
	// ~IContext

	EU_ALWAYS_INLINE ComPtr<ID3D12Device1> device() const { return m_device; }
	EU_ALWAYS_INLINE const D3D12DescriptorHeap& rtv_heap() const { return m_rtv_heap; }
	EU_ALWAYS_INLINE const D3D12DescriptorHeap& dsv_heap() const { return m_dsv_heap; }
	EU_ALWAYS_INLINE const D3D12DescriptorHeap& btv_heap() const { return m_btv_heap; }

private:
	Option<core::Library> m_d3d12;

	using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;
	FnCreateDevice m_create_device;

	using FnSerializeRootSignature = PFN_D3D12_SERIALIZE_ROOT_SIGNATURE;
	FnSerializeRootSignature m_serialize_root_signature;

#if EU_GPU_DEBUG
	using FnGetDebugInterface = PFN_D3D12_GET_DEBUG_INTERFACE;
	FnGetDebugInterface m_get_debug_interface;
	ComPtr<ID3D12Debug> m_debug_interface;
#endif

	ComPtr<IDXGIFactory4> m_factory;
	ComPtr<ID3D12Device1> m_device;

	ComPtr<ID3D12RootSignature> m_root_signature;

	// Render Target View heap
	D3D12DescriptorHeap m_rtv_heap;

	// Depth Stencil View heap
	D3D12DescriptorHeap m_dsv_heap;

	// Bindless Texture View heap
	D3D12DescriptorHeap m_btv_heap;
};

EU_GPU_NAMESPACE_END