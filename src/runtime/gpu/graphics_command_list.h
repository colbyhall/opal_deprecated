// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/math/vector4.h"
#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

/**
 * Defines the memory format for an ITexture. Used to transition a texture into specific states for a specific batch of
 * operations.
 *
 * @see IGraphicsCommandRecorder::texture_barrier
 */
enum class Layout : u8 {
	/**
	 * Default layout that textures are created in except for textures created by swapchains.
	 */
	General,

	/**
	 * Used when binding a texture as a color attachment in a render pass.  Texture's usage must contain
	 * TextureUsage::Color.
	 */
	ColorAttachment,

	/**
	 * Used when binding a texture as a depth attachment in a render pass. Texture's usage must contain
	 * TextureUsage::Depth.
	 */
	DepthAttachment,

	/**
	 * Used when copying data from a texture. TODO: Finish
	 */
	TransferSrc,

	/**
	 * Used when copying data into a texture. Texture's usage must contain TextureUsage::TransferDst.
	 */
	TransferDst,

	/**
	 * Used when we want a texture to be sampled during a draw call from a shader. Texture's usage must contain
	 * TextureUsage::Sampled.
	 */
	ShaderReadOnly,

	/**
	 * Used when we want to display a texture to an OS window. Texture's usage must contain TextureUsage::Backbuffer.
	 */
	Present,
};

/**
 * Stores a list of graphics commands that can be submitted to the GPU to be processed. Produced after recording the
 * commands.
 *
 * @note not always implemented as a list.
 *
 * @see IDevice::record_graphics
 * @see IDevice::submit
 */
class IGraphicsCommandList : public SharedFromThis<IGraphicsCommandList> {
public:
	virtual ~IGraphicsCommandList() = default;
};

/**
 * Builder pattern object that builds IGraphicsCommandList.
 *
 * @see IDevice::record_graphics.
 */
class IGraphicsCommandRecorder {
public:
	/**
	 * Copies a buffer to a texture.
	 *
	 * @param dst - the texture that will be copied into. The texture must be in Layout::TransferDst.
	 *
	 * @param src - the buffer that will be copied from.
	 *
	 * @see IGraphicsCommandRecorder::texture_barrier for layout transition.
	 */
	virtual IGraphicsCommandRecorder& copy_buffer_to_texture(const ITexture& dst, const IBuffer& src) = 0;

	/**
	 * Transitions a texture's layout.
	 *
	 * @param texture - the texture that will change layouts.
	 *
	 * @param old_layout - the layout that the texture is in before the operation is executed.
	 *
	 * @param new_layout - the layout that the texture will be transitioned to.
	 */
	virtual IGraphicsCommandRecorder&
	texture_barrier(const ITexture& texture, Layout old_layout, Layout new_layout) = 0;

	/**
	 * Executes a render pass on provided color and depth attachments. Draw calls are executed during render passes.
	 *
	 * @param color - the texture that will be rendered into. TODO: This should be a slice of ITexture's.
	 *
	 * @param depth - an optional texture that will have depth information rendered into.
	 *
	 * @param callable - a FunctionRef which takes an IRenderPassCommandRecorder to record render pass specific
	 * commands.
	 */
	virtual IGraphicsCommandRecorder& render_pass(
		const ITexture& color,
		Option<ITexture const&> depth,
		FunctionRef<void(IRenderPassCommandRecorder&)> callable
	) = 0;

	virtual ~IGraphicsCommandRecorder() = default;
};

/**
 * Similar to an IGraphicsCommandRecorder but only allows render pass specific operations.
 *
 * @see IGraphicsCommandRecorder::render_pass.
 */
class IRenderPassCommandRecorder {
public:
	/**
	 * Sets every pixel in color attachment's current scissor to a certain color.
	 *
	 * TODO: color should use some LinearColor abstraction
	 */
	virtual IRenderPassCommandRecorder& clear_color(const Vector4<f32>& color) = 0;

	/**
	 * Specifies which pipeline following commands should use.
	 */
	virtual IRenderPassCommandRecorder& set_pipeline(const IGraphicsPipeline& pipeline) = 0;

	/**
	 * Specifies which buffer to be used for vertex data during a draw command.
	 *
	 * @note buffer's usage must contain BufferUsage::Vertex.
	 *
	 * @see IRenderPassCommandRecorder::draw
	 * @see IRenderPassCommandRecorder::draw_indexed
	 */
	virtual IRenderPassCommandRecorder& set_vertices(const IBuffer& buffer, u32 stride) = 0;

	/**
	 * Specifies which buffer to be used for index data during a draw command.
	 *
	 * @note buffer's usage must contain BufferUsage::Index.
	 * @note index buffers currently are expected to be u32's.
	 *
	 * @see IRenderPassCommandRecorder::draw_indexed
	 */
	virtual IRenderPassCommandRecorder& set_indices(const IBuffer& buffer) = 0;

	/**
	 * TODO: Wrap this into an appropriate api.
	 */
	virtual IRenderPassCommandRecorder& push_constants(const void* ptr) = 0;

	/**
	 * Performs a draw call using just vertex data.
	 *
	 * @param vertex_count - the amount of vertices to draw.
	 * @param first_verex - index of vertex to start drawing at. Defaults to 0.
	 *
	 * @see IRenderPassCommandRecorder::set_pipeline
	 * @see IRenderPassCommandRecorder::set_vertices
	 */
	virtual IRenderPassCommandRecorder& draw(usize vertex_count, usize first_vertex = 0) = 0;

	/**
	 * Performs a draw call using index data.
	 *
	 * @param index_count - the amount of indices to draw from.
	 * @param first_index - index of the index to start drawing from. Defaults to 0.
	 *
	 * @see IRenderPassCommandRecorder::set_pipeline
	 * @see IRenderPassCommandRecorder::set_vertices
	 * @see IRenderPassCommandRecorder::set_indices
	 */
	virtual IRenderPassCommandRecorder& draw_index(usize index_count, usize first_index = 0) = 0;

	virtual ~IRenderPassCommandRecorder() = default;
};

SF_GPU_NAMESPACE_END
