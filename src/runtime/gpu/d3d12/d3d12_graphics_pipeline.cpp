// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/d3d12_graphics_pipeline.h"
#include "gpu/d3d12/d3d12_context.h"
#include "gpu/d3d12/d3d12_texture.h"

EU_GPU_NAMESPACE_BEGIN

static D3D12_BLEND convert_blend_factor(BlendFactor factor) {
	switch (factor) {
	case BlendFactor::Zero:
		return D3D12_BLEND_ZERO;
	case BlendFactor::One:
		return D3D12_BLEND_ONE;
	case BlendFactor::SrcColor:
		return D3D12_BLEND_SRC_COLOR;
	case BlendFactor::OneMinusSrcColor:
		return D3D12_BLEND_INV_SRC_COLOR;
	case BlendFactor::DstColor:
		return D3D12_BLEND_DEST_COLOR;
	case BlendFactor::OneMinusDstColor:
		return D3D12_BLEND_INV_DEST_COLOR;
	case BlendFactor::SrcAlpha:
		return D3D12_BLEND_SRC_ALPHA;
	case BlendFactor::OneMinusSrcAlpha:
		return D3D12_BLEND_INV_SRC_ALPHA;
	}
	return D3D12_BLEND_ZERO;
}

static D3D12_BLEND_OP convert_blend_op(BlendOp op) {
	switch (op) {
	case BlendOp::Add:
		return D3D12_BLEND_OP_ADD;
	case BlendOp::Subtract:
		return D3D12_BLEND_OP_SUBTRACT;
	case BlendOp::ReverseSubtract:
		return D3D12_BLEND_OP_REV_SUBTRACT;
	case BlendOp::Min:
		return D3D12_BLEND_OP_MIN;
	case BlendOp::Max:
		return D3D12_BLEND_OP_MAX;
	}
	return D3D12_BLEND_OP_ADD;
}

D3D12GraphicsPipelineImpl::D3D12GraphicsPipelineImpl(Definition&& definition) : m_definition(eu::move(definition)) {
	auto& context = Context::the().interface<D3D12ContextImpl>();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = context.root_signature().the().Get();

	// InputLayout
	auto input_parameters = m_definition.vertex_shader.input_parameters();

	Array<D3D12_INPUT_ELEMENT_DESC, 32> input_layout;

	usize offset = 0;
	for (usize i = 0; i < input_parameters.len(); ++i) {
		D3D12_INPUT_ELEMENT_DESC input = {};
		// input.SemanticIndex = i;
		input.SemanticName = *input_parameters[i].semantic_name;

		usize size_in_bytes = 0;
		switch (input_parameters[i].primitive) {
		case Primitive::Uint32:
			input.Format = DXGI_FORMAT_R32_UINT;
			size_in_bytes = 4;
			break;
		case Primitive::Int32:
			input.Format = DXGI_FORMAT_R32_SINT;
			size_in_bytes = 4;
			break;
		case Primitive::Float32:
			input.Format = DXGI_FORMAT_R32_FLOAT;
			size_in_bytes = 4;
			break;
		case Primitive::Vec2f32:
			input.Format = DXGI_FORMAT_R32G32_FLOAT;
			size_in_bytes = 8;
			break;
		case Primitive::Vec3f32:
			input.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			size_in_bytes = 16;
			break;
		case Primitive::Vec4f32:
			input.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			size_in_bytes = 16;
			break;
		case Primitive::Mat4f32:
			EU_INVALID_CODE_PATH;
			break;
		}
		input.AlignedByteOffset = (UINT)offset;
		input.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

		input_layout.push(input);
		offset += size_in_bytes;
	}
	desc.InputLayout.pInputElementDescs = &input_layout[0];
	desc.InputLayout.NumElements = (UINT)input_layout.len();

	// Vertex shader
	{
		auto& shader = m_definition.vertex_shader;
		auto binary = shader.binary();
		desc.VS.pShaderBytecode = &binary[0];
		desc.VS.BytecodeLength = static_cast<SIZE_T>(binary.len());
	}

	// Pixel shader
	{
		auto& shader = m_definition.pixel_shader;
		auto binary = shader.binary();
		desc.PS.pShaderBytecode = &binary[0];
		desc.PS.BytecodeLength = static_cast<SIZE_T>(binary.len());
	}

	// BlendState
	{
		D3D12_BLEND_DESC blend = {};
		blend.AlphaToCoverageEnable = FALSE;
		blend.IndependentBlendEnable = FALSE;

		D3D12_RENDER_TARGET_BLEND_DESC def = {};
		def.BlendEnable = m_definition.blend_enabled;
		// def.LogicOpEnable = m_config.blend_enabled;
		def.SrcBlend = convert_blend_factor(m_definition.src_color_blend_factor);
		def.DestBlend = convert_blend_factor(m_definition.dst_color_blend_factor);
		def.BlendOp = convert_blend_op(m_definition.color_blend_op);
		def.SrcBlendAlpha = convert_blend_factor(m_definition.src_alpha_blend_factor);
		def.DestBlendAlpha = convert_blend_factor(m_definition.dst_alpha_blend_factor);
		def.BlendOpAlpha = convert_blend_op(m_definition.alpha_blend_op);
		def.LogicOp = D3D12_LOGIC_OP_NOOP;
		def.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			blend.RenderTarget[i] = def;
		}

		desc.BlendState = blend;
	}

	// RasterizerState
	{
		// TODO: Finish this
		D3D12_RASTERIZER_DESC rasterizer_state = {};
		rasterizer_state.FillMode = D3D12_FILL_MODE_SOLID;
		switch (m_definition.cull_mode) {
		case CullMode::None:
			rasterizer_state.CullMode = D3D12_CULL_MODE_NONE;
			break;
		case CullMode::Front:
			rasterizer_state.CullMode = D3D12_CULL_MODE_FRONT;
			break;
		case CullMode::Back:
			rasterizer_state.CullMode = D3D12_CULL_MODE_BACK;
			break;
		}
		rasterizer_state.FrontCounterClockwise = FALSE;
		rasterizer_state.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizer_state.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizer_state.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizer_state.DepthClipEnable = TRUE;
		rasterizer_state.MultisampleEnable = FALSE;
		rasterizer_state.AntialiasedLineEnable = FALSE;
		rasterizer_state.ForcedSampleCount = 0;
		rasterizer_state.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		desc.RasterizerState = rasterizer_state;
	}

	// DepthStencilState
	if (m_definition.depth_attachment.is_set()) {
		desc.DepthStencilState.DepthEnable = TRUE;
		desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		desc.DepthStencilState.StencilEnable = FALSE;
	}

	desc.NumRenderTargets = (UINT)m_definition.color_attachments.len();
	for (usize i = 0; i < m_definition.color_attachments.len(); ++i) {
		desc.RTVFormats[i] = format_to_dxgi(m_definition.color_attachments[i]);
	}

	if (m_definition.depth_attachment.is_set()) {
		desc.DSVFormat = format_to_dxgi(m_definition.depth_attachment.unwrap());
	}

	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.SampleDesc.Count = 1;

	throw_if_failed(context.device()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pipeline)));
}

EU_GPU_NAMESPACE_END