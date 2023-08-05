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
	using FnSerializeRootSignature = PFN_D3D12_SERIALIZE_ROOT_SIGNATURE;
	using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;

	explicit D3D12ContextImpl();

	// IContext
	Backend backend() const final { return Backend::D3D12; }
	// ~IContext

	EU_ALWAYS_INLINE FnSerializeRootSignature serialize_root_signature() const { return m_serialize_root_signature; }

	EU_ALWAYS_INLINE ComPtr<ID3D12Device1> device() const { return m_device; }
	EU_ALWAYS_INLINE ComPtr<ID3D12CommandAllocator> command_allocator() const { return m_command_allocator; }
	EU_ALWAYS_INLINE const D3D12RootSignatureImpl& root_signature() const { return m_root_signature; }

private:
	Option<core::Library> m_d3d12;
	FnCreateDevice m_create_device;
	FnSerializeRootSignature m_serialize_root_signature;

#if EU_GPU_DEBUG
	using FnGetDebugInterface = PFN_D3D12_GET_DEBUG_INTERFACE;
	FnGetDebugInterface m_get_debug_interface;
	ComPtr<ID3D12Debug> m_debug_interface;
#endif

	ComPtr<IDXGIFactory4> m_factory;
	ComPtr<ID3D12Device1> m_device;
	ComPtr<ID3D12CommandAllocator> m_command_allocator;

	D3D12RootSignatureImpl m_root_signature;
};

EU_GPU_NAMESPACE_END