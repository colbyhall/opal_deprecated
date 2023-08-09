// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"
#include "core/containers/result.h"
#include "core/containers/string_view.h"

#include "gpu/shader.h"

// Begin the dxc namespace
#define GJ_DXC_NAMESPACE_BEGIN																    \
	GJ_NAMESPACE_BEGIN																			\
	namespace dxc {

// End the dxc namespace
#define GJ_DXC_NAMESPACE_END																	\
	}																						\
	GJ_NAMESPACE_END

GJ_DXC_NAMESPACE_BEGIN

using gpu::InputParameter;

enum class ShaderType {
	Vertex,
	Pixel
};

struct Input {
	StringView source;
	StringView main;
	ShaderType type;
};

struct Output {
	Vector<u8> binary;
	Vector<InputParameter> input_parameters;
};

Result<Output, String> compile(const Input& input);

GJ_DXC_NAMESPACE_END
