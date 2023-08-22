// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/buffer.h"
#include "gpu/d3d12/d3d12.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12BufferImpl final : public IBuffer {
public:
	explicit D3D12BufferImpl(BufferUsage usage, Heap kind, usize size, ComPtr<ID3D12Resource>&& resource)
		: m_usage(usage)
		, m_kind(kind)
		, m_size(size)
		, m_resource(sf::move(resource)) {}

	// IBuffer
	BufferUsage usage() const final { return m_usage; }
	Heap heap() const final { return m_kind; }
	usize size() const final { return m_size; }
	void map(FunctionRef<void(Slice<u8>)> func) final;
	// ~IBuffer

	SF_ALWAYS_INLINE const ComPtr<ID3D12Resource>& resource() const { return m_resource; };

private:
	BufferUsage m_usage;
	Heap m_kind;
	usize m_size;

	ComPtr<ID3D12Resource> m_resource;
};

SF_GPU_NAMESPACE_END
