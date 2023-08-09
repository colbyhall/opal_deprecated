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
	void begin() override;
	void copy_buffer_to_texture(const Texture& dst, const Buffer& src) override;
	void texture_barrier(
		const Texture& texture,
		Layout old_layout,
		Layout new_layout
	) override;
	void begin_render_pass(const Texture& color, Option<const Texture&> depth)
		override;
	void set_pipeline(const GraphicsPipeline& pipeline) override;
	void set_vertices(const Buffer& buffer, u32 stride) override;
	void set_indices(const Buffer& buffer) override;
	void push_constant(const void* ptr) override;
	void draw(usize vertex_count, usize first_vertex) override;
	void draw_indexed(usize index_count, usize first_index) override;
	void end_render_pass() override;
	void end() override;
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
