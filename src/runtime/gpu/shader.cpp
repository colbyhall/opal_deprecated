// Copyright Colby Hall. All Rights Reserved.

#include "gpu/shader.h"
#include "gpu/context.h"
#include "gpu/d3d12/d3d12_shader.h"

GJ_GPU_NAMESPACE_BEGIN

VertexShader VertexShader::make(
	Vector<u8>&& binary,
	Vector<InputParameter>&& input_parameters
) {
	auto& context = Context::the();

	Option<Shared<IVertexShader>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Shared<D3D12VertexShaderImpl>::make(
			gj::move(binary),
			gj::move(input_parameters)
		);
		break;
	}

	return VertexShader{ interface.unwrap() };
}

PixelShader PixelShader::make(Vector<u8>&& binary) {
	auto& context = Context::the();

	Option<Shared<IPixelShader>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Shared<D3D12PixelShaderImpl>::make(gj::move(binary));
		break;
	}

	return PixelShader{ interface.unwrap() };
}

GJ_GPU_NAMESPACE_END