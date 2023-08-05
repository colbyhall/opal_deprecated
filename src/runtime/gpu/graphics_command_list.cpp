// Copyright Colby Hall. All Rights Reserved.

#include "gpu/buffer.h"
#include "gpu/context.h"
#include "gpu/d3d12/d3d12_graphics_command_list.h"
#include "gpu/texture.h"

GJ_GPU_NAMESPACE_BEGIN

GraphicsCommandList GraphicsCommandList::record(FunctionRef<void(GraphicsCommandRecorder&)> callable) {
	auto& context = Context::the();

	Option<Shared<IGraphicsCommandRecorder>> interface = nullptr;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Shared<D3D12GraphicsCommandRecorderImpl>::make();
		break;
	}

	auto result = GraphicsCommandList{ interface.unwrap() };
	result.m_interface->begin();
	GraphicsCommandRecorder recorder(*result.m_interface);
	callable(recorder);
	result.m_interface->end();
	return result;
}

GraphicsCommandRecorder& GraphicsCommandRecorder::copy_buffer_to_texture(const Texture& dst, const Buffer& src) {
	m_interface.copy_buffer_to_texture(dst, src);
	return *this;
}

GraphicsCommandRecorder&
GraphicsCommandRecorder::texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) {
	m_interface.texture_barrier(texture, old_layout, new_layout);
	return *this;
}

GraphicsCommandRecorder& GraphicsCommandRecorder::render_pass(
	const Texture& color,
	Option<const Texture&> depth,
	FunctionRef<void(RenderPassRecorder&)> callable
) {
	m_interface.begin_render_pass(color, depth);
	RenderPassRecorder recorder(m_interface);
	callable(recorder);
	m_interface.end_render_pass();
	return *this;
}

RenderPassRecorder& RenderPassRecorder::set_vertices(const Buffer& buffer, u32 stride) {
	m_interface.set_vertices(buffer, stride);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::set_indices(const Buffer& buffer) {
	m_interface.set_indices(buffer);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::push_constants(const void* ptr) {
	m_interface.push_constant(ptr);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::draw(usize vertex_count, usize first_vertex) {
	m_interface.draw(vertex_count, first_vertex);
	return *this;
}

RenderPassRecorder& RenderPassRecorder::draw_index(usize index_count, usize first_index) {
	m_interface.draw_indexed(index_count, first_index);
	return *this;
}

GJ_GPU_NAMESPACE_END