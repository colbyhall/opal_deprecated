// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/buffer.h"
#include "gpu/d3d12/d3d12.h"

GJ_GPU_NAMESPACE_BEGIN

class D3D12BufferImpl final : public IBuffer {
public:
	using Usage = Buffer::Usage;
	using Kind = Buffer::Kind;

	explicit D3D12BufferImpl(Usage usage, Kind kind, usize len);

	// IBuffer
	Usage usage() const final { return m_usage; }
	Kind kind() const final { return m_kind; }
	usize len() const final { return m_len; }
	void map(FunctionRef<void(Slice<u8>)>& func) final;
	// ~IBuffer

	GJ_ALWAYS_INLINE ComPtr<ID3D12Resource> resource() const {
		return m_resource;
	};

private:
	Usage m_usage;
	Kind m_kind;
	usize m_len;

	ComPtr<ID3D12Resource> m_resource;
};

GJ_GPU_NAMESPACE_END