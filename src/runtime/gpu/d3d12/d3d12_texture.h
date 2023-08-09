// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12_memory.h"
#include "gpu/texture.h"

GJ_GPU_NAMESPACE_BEGIN

DXGI_FORMAT format_to_dxgi(Format format);

class D3D12TextureImpl final : public ITexture {
public:
	using Usage = Texture::Usage;

	explicit D3D12TextureImpl(
		Usage usage,
		Format format,
		const Vec3u32& size,
		ComPtr<ID3D12Resource> resource = nullptr
	);

	// ITexture
	Usage usage() const final { return m_usage; }
	Format format() const final { return m_format; }
	Vec3u32 size() const final { return m_size; }
	u32 bindless() const final { return m_bt2dv_handle.index; }
	~D3D12TextureImpl() final;
	// ~ITexture

	GJ_ALWAYS_INLINE ComPtr<ID3D12Resource> resource() const {
		return m_resource;
	};

	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& rtv_handle() const {
		return m_rtv_handle;
	}
	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& dsv_handle() const {
		return m_dsv_handle;
	}
	GJ_ALWAYS_INLINE const D3D12DescriptorHandle& bt2dv_handle() const {
		return m_bt2dv_handle;
	}

private:
	ComPtr<ID3D12Resource> m_resource;
	Texture::Usage m_usage;
	Format m_format;
	Vec3u32 m_size;

	D3D12DescriptorHandle m_rtv_handle = {};
	D3D12DescriptorHandle m_dsv_handle = {};
	D3D12DescriptorHandle m_bt2dv_handle = {};
};

GJ_GPU_NAMESPACE_END