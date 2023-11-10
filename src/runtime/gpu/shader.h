// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string.h"
#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

/**
 * A shader is a program that is ran on the GPU to transform some data. All of our shaders are written in HLSL and are
 * compiled by DXC.
 *
 * @see dxc/dxc.h
 */
class IShader {
public:
	/**
	 * Returns a slice to the compiled binary.
	 */
	virtual Slice<u8 const> binary() const = 0;

	virtual ~IShader() = default;
};

/**
 * Defines which types are allowed to be a shader primitive.
 */
enum class ShaderPrimitive : u8 {
	Uint32,
	Int32,
	Float32,
	Vector2f32,
	Vector3f32,
	Vector4f32,
	Matrix4f32,
};

/**
 * Defines input parameters to HLSL vertex shaders.
 */
struct InputParameter {
	u32 semantic_index;
	String semantic_name;
	ShaderPrimitive primitive;
};

/**
 * Vertex shaders process inputted vertex data into triangles for the GPU to rasterize.
 *
 * @see GraphicsPipelineDescription
 */
class VertexShader : public IShader, public SharedFromThis<VertexShader> {
public:
	/**
	 * Returns the HLSL vertex input parameters.
	 */
	virtual Slice<InputParameter const> input_parameters() const = 0;
};

/**
 * Pixel shaders determine the color at for every pixel of a rasterized triangle.
 */
class PixelShader : public IShader, public SharedFromThis<PixelShader> {
public:
};

SF_GPU_NAMESPACE_END
