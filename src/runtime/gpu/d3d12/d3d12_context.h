// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/context.h"
#include "gpu/d3d12/d3d12.h"

#include "core/containers/option.h"
#include "core/os/library.h"

EU_GPU_NAMESPACE_BEGIN

class D3D12ContextImpl : public IContext {
public:
	explicit D3D12ContextImpl();

	// IContext
	Backend backend() const override { return Backend::D3D12; }
	// ~IContext

	EU_NO_DISCARD EU_ALWAYS_INLINE ComPtr<ID3D12Device1> device() const { return m_device; }

private:
	Option<core::Library> m_d3d12;

	using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;
	FnCreateDevice m_create_device;

#if EU_GPU_DEBUG
	using FnGetDebugInterface = PFN_D3D12_GET_DEBUG_INTERFACE;
	FnGetDebugInterface m_get_debug_interface;
	ComPtr<ID3D12Debug> m_debug_interface;
#endif

	ComPtr<IDXGIFactory4> m_factory;
	ComPtr<ID3D12Device1> m_device;
};

EU_GPU_NAMESPACE_END