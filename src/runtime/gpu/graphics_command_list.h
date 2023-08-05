// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "gpu/gpu.h"

EU_GPU_NAMESPACE_BEGIN

enum class Layout : u8 {
	Undefined,
	General,
	ColorAttachment,
	DepthAttachment,
	TransferSrc,
	TransferDst,
	ShaderReadOnly,
	Present,
};

class Texture;
class Buffer;
class GraphicsPipeline;

class IGraphicsCommandRecorder {
public:
	virtual void begin() = 0;

	virtual void copy_buffer_to_texture(const Texture& dst, const Buffer& src) = 0;

	virtual void texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) = 0;

	virtual void begin_render_pass(const Texture& color, Option<Texture const&> depth) = 0;

	virtual void set_pipeline(const GraphicsPipeline& pipeline) = 0;
	virtual void set_vertices(const Buffer& buffer, u32 stride) = 0;
	virtual void set_indices(const Buffer& buffer) = 0;
	virtual void push_constant(const void* ptr) = 0;

	virtual void draw(usize vertex_count, usize first_vertex) = 0;
	virtual void draw_indexed(usize index_count, usize first_index) = 0;

	virtual void end_render_pass() = 0;

	virtual void end() = 0;

	virtual ~IGraphicsCommandRecorder() = default;
};

class GraphicsCommandRecorder;
class RenderPassRecorder;

class GraphicsCommandList {
public:
	static GraphicsCommandList record(FunctionRef<void(GraphicsCommandRecorder&)> callable);

private:
	EU_ALWAYS_INLINE explicit GraphicsCommandList(Shared<IGraphicsCommandRecorder>&& interface)
		: m_interface(eu::move(interface)) {}

	Shared<IGraphicsCommandRecorder> m_interface;
};

class GraphicsCommandRecorder {
public:
	GraphicsCommandRecorder& copy_buffer_to_texture(const Texture& dst, const Buffer& src);
	GraphicsCommandRecorder& texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout);

	GraphicsCommandRecorder&
	render_pass(const Texture& color, Option<const Texture&> depth, FunctionRef<void(RenderPassRecorder&)> callable);

private:
	inline GraphicsCommandRecorder(IGraphicsCommandRecorder& interface) : m_interface(interface) {}
	friend class GraphicsCommandList;

	IGraphicsCommandRecorder& m_interface;
};

class RenderPassRecorder {
public:
	RenderPassRecorder& set_pipeline(const GraphicsPipeline& pipeline);
	RenderPassRecorder& set_vertices(const Buffer& buffer, u32 stride);
	RenderPassRecorder& set_indices(const Buffer& buffer);
	RenderPassRecorder& push_constants(const void* ptr);

	RenderPassRecorder& draw(usize vertex_count, usize first_vertex = 0);
	RenderPassRecorder& draw_index(usize index_count, usize first_index = 0);

private:
	inline RenderPassRecorder(IGraphicsCommandRecorder& interface) : m_interface(interface) {}
	friend class GraphicsCommandRecorder;

	IGraphicsCommandRecorder& m_interface;
};

EU_GPU_NAMESPACE_END