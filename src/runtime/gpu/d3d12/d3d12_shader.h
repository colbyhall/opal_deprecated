// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/d3d12.h"
#include "gpu/shader.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12VertexShaderImpl final : public IVertexShader {
public:
	explicit D3D12VertexShaderImpl(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters)
		: m_binary(sf::move(binary))
		, m_input_parameters(sf::move(input_parameters)) {}

	// IVertexShader
	Slice<u8 const> binary() const final { return m_binary; }
	Slice<gpu::InputParameter const> input_parameters() const final { return m_input_parameters; }
	// ~IVertexShader

private:
	Vector<u8> m_binary;
	Vector<InputParameter> m_input_parameters;
};

class D3D12PixelShaderImpl : public IPixelShader {
public:
	explicit D3D12PixelShaderImpl(Vector<u8>&& binary) : m_binary(sf::move(binary)) {}

	// IPixelShader
	Slice<u8 const> binary() const final { return m_binary; }
	// ~IPixelShader

private:
	Vector<u8> m_binary;
};

SF_GPU_NAMESPACE_END
