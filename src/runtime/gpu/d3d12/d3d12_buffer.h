// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/buffer.h"
#include "gpu/d3d12/d3d12.h"

EU_GPU_NAMESPACE_BEGIN

class D3D12BufferImpl : public IBuffer {
public:
	explicit D3D12BufferImpl(Buffer::Usage usage, Buffer::Kind kind, usize len);

	inline D3D12BufferImpl(D3D12BufferImpl&& move) noexcept
		: m_usage(move.m_usage)
		, m_kind(move.m_kind)
		, m_len(move.m_len)
		, m_resource(eu::move(move.m_resource)) {
		move.m_resource = nullptr;
	}

	// IBuffer
	Buffer::Usage usage() const override { return m_usage; }
	Buffer::Kind kind() const override { return m_kind; }
	usize len() const override { return m_len; }
	void map(FunctionRef<void(Slice<u8>)>& func) override;
	// ~IBuffer

private:
	Buffer::Usage m_usage;
	Buffer::Kind m_kind;
	usize m_len;

	ComPtr<ID3D12Resource> m_resource;
};

EU_GPU_NAMESPACE_END