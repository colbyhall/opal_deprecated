// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"

#include "gpu/d3d12/d3d12.h"
#include "gpu/graphics_command_list.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12GraphicsCommandList final : public GraphicsCommandList {
public:
	explicit D3D12GraphicsCommandList(const ComPtr<ID3D12GraphicsCommandList>& in_command_list)
		: command_list(in_command_list) {}

	ComPtr<ID3D12GraphicsCommandList> command_list;

	Option<D3D12_CPU_DESCRIPTOR_HANDLE> bound_color_buffer;
	Option<D3D12_CPU_DESCRIPTOR_HANDLE> bound_depth_buffer;

	Vector<Shared<Texture>> textures_in_use;
	Vector<Shared<Buffer>> buffers_in_use;
};

class D3D12GraphicsCommandRecorderImpl final : public GraphicsCommandRecorder {
public:
	explicit D3D12GraphicsCommandRecorderImpl(D3D12GraphicsCommandList& command_list) : m_command_list(command_list) {}

	// GraphicsCommandRecorder
	GraphicsCommandRecorder& copy_buffer_to_texture(const Texture& dst, const Buffer& src) final;

	GraphicsCommandRecorder& texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) final;

	GraphicsCommandRecorder& render_pass(
		const Texture& color,
		Option<Texture const&> depth,
		FunctionRef<void(RenderPassCommandRecorder&)> callable
	) final;
	// ~GraphicsCommandRecorder

private:
	D3D12GraphicsCommandList& m_command_list;
};

class D3D12RenderPassRecorderImpl final : public RenderPassCommandRecorder {
public:
	explicit D3D12RenderPassRecorderImpl(D3D12GraphicsCommandList& command_list) : m_command_list(command_list) {}

	// GraphicsCommandRecorder
	RenderPassCommandRecorder& clear_color(const Vector4<f32>& color) final;
	RenderPassCommandRecorder& set_pipeline(const GraphicsPipeline& pipeline) final;
	RenderPassCommandRecorder& set_vertices(const Buffer& buffer, u32 stride) final;
	RenderPassCommandRecorder& set_indices(const Buffer& buffer) final;
	RenderPassCommandRecorder& push_constants(const void* ptr) final;
	RenderPassCommandRecorder& draw(usize vertex_count, usize first_vertex = 0) final;
	RenderPassCommandRecorder& draw_index(usize index_count, usize first_index = 0) final;
	// ~GraphicsCommandRecorder

private:
	D3D12GraphicsCommandList& m_command_list;
};

SF_GPU_NAMESPACE_END
