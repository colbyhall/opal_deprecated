// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"

#include "gpu/d3d12/d3d12.h"
#include "gpu/graphics_command_list.h"

GJ_GPU_NAMESPACE_BEGIN

class D3D12GraphicsCommandRecorderImpl final : public IGraphicsCommandRecorder {
public:
	explicit D3D12GraphicsCommandRecorderImpl();

	// IGraphicsCommandRecorder
	void begin() final;
	void copy_buffer_to_texture(const Texture& dst, const Buffer& src) final;
	void texture_barrier(
		const Texture& texture,
		Layout old_layout,
		Layout new_layout
	) final;
	void
	begin_render_pass(const Texture& color, Option<const Texture&> depth) final;
	void clear_color(const Vector4<f32>& color) final;
	void set_pipeline(const GraphicsPipeline& pipeline) final;
	void set_vertices(const Buffer& buffer, u32 stride) final;
	void set_indices(const Buffer& buffer) final;
	void push_constant(const void* ptr) final;
	void draw(usize vertex_count, usize first_vertex) final;
	void draw_indexed(usize index_count, usize first_index) final;
	void end_render_pass() final;
	void end() final;
	// ~IGraphicsCommandRecorder

	GJ_ALWAYS_INLINE ComPtr<ID3D12GraphicsCommandList> command_list() const {
		return m_command_list;
	}

private:
	ComPtr<ID3D12GraphicsCommandList> m_command_list;

	Option<D3D12_CPU_DESCRIPTOR_HANDLE> m_bound_color_buffer;
	Option<D3D12_CPU_DESCRIPTOR_HANDLE> m_bound_depth_buffer;

	Vector<Texture> m_textures_in_use;
	Vector<Buffer> m_buffers_in_use;
};

GJ_GPU_NAMESPACE_END
