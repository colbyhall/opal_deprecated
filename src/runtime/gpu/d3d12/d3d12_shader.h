// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/d3d12.h"
#include "gpu/shader.h"

EU_GPU_NAMESPACE_BEGIN

class D3D12VertexShaderImpl final : public IVertexShader {
public:
	explicit D3D12VertexShaderImpl(Vector<u8>&& binary, Vector<gpu::InputParameter>&& input_parameters)
		: m_binary(eu::move(binary))
		, m_input_parameters(eu::move(input_parameters)) {}

	// IVertexShader
	Slice<u8 const> binary() const final { return m_binary; }
	Slice<gpu::InputParameter const> input_parameters() const final { return m_input_parameters; }
	// ~IVertexShader

private:
	Vector<u8> m_binary;
	Vector<gpu::InputParameter> m_input_parameters;
};

class D3D12PixelShaderImpl : public IPixelShader {
public:
	explicit D3D12PixelShaderImpl(Vector<u8>&& binary) : m_binary(eu::move(binary)) {}

	// IPixelShader
	Slice<u8 const> binary() const final { return m_binary; }
	// ~IPixelShader

private:
	Vector<u8> m_binary;
};

EU_GPU_NAMESPACE_END