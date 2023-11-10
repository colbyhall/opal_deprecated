// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/d3d12.h"
#include "gpu/shader.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12VertexShader final : public VertexShader {
public:
	explicit D3D12VertexShader(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters)
		: m_binary(sf::move(binary))
		, m_input_parameters(sf::move(input_parameters)) {}

	// VertexShader
	Slice<u8 const> binary() const final { return m_binary; }
	Slice<gpu::InputParameter const> input_parameters() const final { return m_input_parameters; }
	// ~VertexShader

private:
	Vector<u8> m_binary;
	Vector<InputParameter> m_input_parameters;
};

class D3D12PixelShader : public PixelShader {
public:
	explicit D3D12PixelShader(Vector<u8>&& binary) : m_binary(sf::move(binary)) {}

	// PixelShader
	Slice<u8 const> binary() const final { return m_binary; }
	// ~PixelShader

private:
	Vector<u8> m_binary;
};

SF_GPU_NAMESPACE_END
