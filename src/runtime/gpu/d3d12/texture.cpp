// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/texture.h"
#include "gpu/d3d12/device.h"

OP_GPU_NAMESPACE_BEGIN

DXGI_FORMAT format_to_dxgi(Format format) {
	DXGI_FORMAT dxgi_format = DXGI_FORMAT_UNKNOWN;

	switch (format) {
	case Format::Undefined:
		OP_INVALID_CODE_PATH;
		break;
	case Format::R_U8:
		dxgi_format = DXGI_FORMAT_R8_UNORM;
		break;
	case Format::RGBA_U8:
		dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case Format::RGBA_U8_SRGB:
		dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		break;
	case Format::RGBA_F16:
		dxgi_format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	case Format::RGBA_F32:
		dxgi_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case Format::D24S8_U32:
		dxgi_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	}

	return dxgi_format;
}

D3D12Texture::D3D12Texture(
	const D3D12Device& context,
	TextureUsage usage,
	Format format,
	const Vector3<u32>& size,
	ComPtr<ID3D12Resource> resource
)
	: m_context(context.to_shared())
	, m_usage(usage)
	, m_format(format)
	, m_size(size) {
	OP_ASSERT(size.x > 0);
	OP_ASSERT(size.y > 0);
	OP_ASSERT(size.z > 0);

	D3D12_RESOURCE_DIMENSION dimension = D3D12_RESOURCE_DIMENSION_UNKNOWN;
	if (size.x > 1) {
		dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		if (size.y > 1) {
			dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			if (size.z > 1) {
				dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}
		}
	}
	OP_ASSERT(dimension != D3D12_RESOURCE_DIMENSION_UNKNOWN);

	const DXGI_FORMAT dxgi_format = format_to_dxgi(format);

	const bool color_attachment = (usage & TextureUsage::Color) == TextureUsage::Color;
	const bool depth_attachment = (usage & TextureUsage::Depth) == TextureUsage::Depth;
	const bool sampled = (usage & TextureUsage::Sampled) == TextureUsage::Sampled;

	if (resource == nullptr) {
		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = dimension;
		desc.Width = size.x;
		desc.Height = size.y;
		desc.DepthOrArraySize = (UINT16)size.z;
		desc.MipLevels = 1;
		desc.Format = dxgi_format;
		desc.SampleDesc.Count = 1;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (color_attachment) {
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}
		if (depth_attachment) {
			desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_GENERIC_READ;
		if (depth_attachment) {
			initial_state = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		}

		D3D12_HEAP_PROPERTIES heap = {};
		heap.Type = D3D12_HEAP_TYPE_DEFAULT;

		bool optimized_clear = color_attachment;
		optimized_clear |= depth_attachment;

		D3D12_CLEAR_VALUE clear = {};
		D3D12_CLEAR_VALUE* pclear = nullptr;
		if (optimized_clear) {
			clear.Color[3] = 1.f;
			clear.Format = dxgi_format;
			clear.DepthStencil.Depth = 1.f;
			pclear = &clear;
		}

		throw_if_failed(context.device()->CreateCommittedResource(
			&heap,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			initial_state,
			pclear,
			IID_PPV_ARGS(&m_resource)
		));
	} else {
		m_resource = resource;
	}

	if (color_attachment) {
		m_rtv_handle = context.root_signature().rtv_heap().alloc();
		context.device()->CreateRenderTargetView(m_resource.Get(), nullptr, m_rtv_handle.handle);
	}
	if (depth_attachment) {
		m_dsv_handle = context.root_signature().dsv_heap().alloc();
		context.device()->CreateDepthStencilView(m_resource.Get(), nullptr, m_dsv_handle.handle);
	}
	if (sampled) {
		m_bt2dv_handle = context.root_signature().bt2dv_heap().alloc();
		context.device()->CreateShaderResourceView(m_resource.Get(), nullptr, m_bt2dv_handle.handle);
	}
}

D3D12Texture::~D3D12Texture() {
	const auto& context = static_cast<const D3D12Device&>(*m_context);
	auto& root_signature = context.root_signature();

	if (m_rtv_handle.handle.ptr) {
		root_signature.rtv_heap().free(m_rtv_handle);
		m_rtv_handle = {};
	}
	if (m_dsv_handle.handle.ptr) {
		root_signature.dsv_heap().free(m_dsv_handle);
		m_dsv_handle = {};
	}
	if (m_bt2dv_handle.handle.ptr) {
		root_signature.bt2dv_heap().free(m_bt2dv_handle);
		m_bt2dv_handle = {};
	}
}

OP_GPU_NAMESPACE_END
