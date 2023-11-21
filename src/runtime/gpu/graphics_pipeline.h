// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/gpu.h"

OP_GPU_NAMESPACE_BEGIN

/**
 * Defines how a triangle should be rasterized during a draw call.
 */
enum class DrawMode : u8 {
	/**
	 * Fills the entire triangle.
	 */
	Fill,

	/**
	 * Only fills in the lines connecting the vertices.
	 *
	 * @see GraphicsPipelineDefinition::line_width
	 */
	Line,

	/**
	 * Only fills in a rect at each vertex.
	 *
	 * @see GraphicsPipelineDefinition::line_width
	 */
	Point
};

/**
 * Defines if a side of a triangle should be culled or not. The side of the triangle is defined by the order of the
 * vertices. Currently we use clockwise for all vertex data.
 */
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

/**
 * Defines an GraphicsPipeline.
 */
struct GraphicsPipelineDefinition {
	/**
	 * Vertex shader that will be used when the pipeline is active.
	 */
	Shared<VertexShader> vertex_shader;

	/**
	 * Pixel shader that will be used when the pipeline is active.
	 */
	Shared<PixelShader> pixel_shader;

	/**
	 * Pixel format shaders are expecting as color attachments.
	 */
	Array<Format, 8> color_attachments;

	/**
	 * Pixel format shaders are expecting for a depth attachment. None is allowed as well.
	 */
	Option<Format> depth_attachment = op::nullopt;

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

/**
 * Describes how a render pass should execute a draw call. This includes what shaders should be used, how the rasterizer
 * should draws the triangles, and much more.
 *
 * @see Device::create_graphics_pipeline.
 * @see GraphicsPipelineDefinition.
 * @see GraphicsCommandRecorder.
 */
class GraphicsPipeline : public SharedFromThis<GraphicsPipeline> {
public:
	virtual const GraphicsPipelineDefinition& definition() const = 0;
	virtual ~GraphicsPipeline() = default;
};

OP_GPU_NAMESPACE_END
