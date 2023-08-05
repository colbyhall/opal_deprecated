// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slice.h"
#include "core/containers/string.h"
#include "gpu/gpu.h"

GJ_GPU_NAMESPACE_BEGIN

class IShader {
public:
	virtual Slice<u8 const> binary() const = 0;
	virtual ~IShader() = default;
};

enum class Primitive : u8 {
	Uint32,
	Int32,
	Float32,
	Vec2f32,
	Vec3f32,
	Vec4f32,
	Mat4f32,
};

struct InputParameter {
	u32 semantic_index;
	String semantic_name;
	Primitive primitive;
};

class IVertexShader : public IShader {
public:
	virtual Slice<InputParameter const> input_parameters() const = 0;
};

class VertexShader {
public:
	static VertexShader make(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters);

	GJ_ALWAYS_INLINE Slice<u8 const> binary() const { return m_interface->binary(); }
	GJ_ALWAYS_INLINE Slice<InputParameter const> input_parameters() const { return m_interface->input_parameters(); }

private:
	GJ_ALWAYS_INLINE explicit VertexShader(Shared<IVertexShader>&& interface) : m_interface(gj::move(interface)) {}

	Shared<IVertexShader> m_interface;
};

class IPixelShader : public IShader {
public:
};

class PixelShader {
public:
	static PixelShader make(Vector<u8>&& binary);

	GJ_ALWAYS_INLINE Slice<u8 const> binary() const { return m_interface->binary(); }

private:
	GJ_ALWAYS_INLINE explicit PixelShader(Shared<IPixelShader>&& interface) : m_interface(gj::move(interface)) {}

	Shared<IPixelShader> m_interface;
};

GJ_GPU_NAMESPACE_END