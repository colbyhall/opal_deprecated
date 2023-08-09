// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/shader.h"
#include "gpu/texture.h"

GJ_GPU_NAMESPACE_BEGIN

enum class DrawMode : u8 { Fill, Line, Point };

enum class CullMode : u8 { None, Front, Back };

enum class CompareOp : u8 {
	// `false`
	Never,
	// `A < B`
	Less,
	// `A == B`
	Equal,
	// `A < B || A == B`
	LessOrEqual,
	// `A > B`
	Greater,
	// `A != B`
	NotEqual,
	// `A > B || A == B`
	GreaterOrEqual,
	// `true`
	Always,
};

enum class BlendOp : u8 {
	Add,
	Subtract,
	ReverseSubtract,
	Min,
	Max,
};

enum class BlendFactor : u8 {
	Zero,
	One,

	SrcColor,
	OneMinusSrcColor,
	DstColor,
	OneMinusDstColor,

	SrcAlpha,
	OneMinusSrcAlpha,
};

class IGraphicsPipeline;

class GraphicsPipeline {
public:
	struct Definition {
		VertexShader vertex_shader;
		PixelShader pixel_shader;

		Array<Format, 8> color_attachments;
		Option<Format> depth_attachment = gj::none;

		DrawMode draw_mode = DrawMode::Fill;
		f32 line_width = 1.f;
		CullMode cull_mode = CullMode::None;

		bool blend_enabled = false;

		BlendFactor src_color_blend_factor = BlendFactor::One;
		BlendFactor dst_color_blend_factor = BlendFactor::One;
		BlendOp color_blend_op = BlendOp::Add;

		BlendFactor src_alpha_blend_factor = BlendFactor::One;
		BlendFactor dst_alpha_blend_factor = BlendFactor::One;
		BlendOp alpha_blend_op = BlendOp::Add;

		bool depth_test = false;
		bool depth_write = false;
		CompareOp depth_compare = CompareOp::Always;
	};

	static GraphicsPipeline make(Definition&& definition);

	template <typename T = IGraphicsPipeline>
	GJ_ALWAYS_INLINE T const& cast() const {
		static_assert(
			std::is_base_of_v<IGraphicsPipeline, T>,
			"T is not derived of IGraphicsPipeline"
		);
		return static_cast<const T&>(*m_interface);
	}

private:
	GJ_ALWAYS_INLINE explicit GraphicsPipeline(Shared<IGraphicsPipeline>&&
												   interface)
		: m_interface(gj::move(interface)) {}

	Shared<IGraphicsPipeline> m_interface;
};

class IGraphicsPipeline {
public:
	virtual const GraphicsPipeline::Definition& definition() const = 0;
	virtual ~IGraphicsPipeline() = default;
};

GJ_GPU_NAMESPACE_END
