// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12_memory.h"
#include "gpu/texture.h"

GJ_GPU_NAMESPACE_BEGIN

DXGI_FORMAT format_to_dxgi(Format format);

class D3D12TextureImpl final : public ITexture {
public:
	explicit D3D12TextureImpl(
		const D3D12DeviceImpl& context,
		TextureUsage usage,
		Format format,
		const Vector3<u32>& size,
		ComPtr<ID3D12Resource> resource = nullptr
	);

	// ITexture
	TextureUsage usage() const final { return m_usage; }
	Format format() const final { return m_format; }
	Vector3<u32> size() const final { return m_size; }
	u32 bindless() const final { return m_bt2dv_handle.index; }
	~D3D12TextureImpl() final;
	// ~ITexture

	GJ_ALWAYS_INLINE const ComPtr<ID3D12Resource>& resource() const { return m_resource; };

	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& rtv_handle() const { return m_rtv_handle; }
	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& dsv_handle() const { return m_dsv_handle; }
	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& bt2dv_handle() const { return m_bt2dv_handle; }

private:
	Shared<IDevice> m_context;

	ComPtr<ID3D12Resource> m_resource;
	TextureUsage m_usage;
	Format m_format;
	Vector3<u32> m_size;

	D3D12DescriptorHandle m_rtv_handle = {};
	D3D12DescriptorHandle m_dsv_handle = {};
	D3D12DescriptorHandle m_bt2dv_handle = {};
};

GJ_GPU_NAMESPACE_END
