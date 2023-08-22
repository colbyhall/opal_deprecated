// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"

#include "gpu/d3d12/d3d12.h"
#include "gpu/graphics_command_list.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12GraphicsCommandListImpl final : public IGraphicsCommandList {
public:
	explicit D3D12GraphicsCommandListImpl(const ComPtr<ID3D12GraphicsCommandList>& in_command_list)
		: command_list(in_command_list) {}

	ComPtr<ID3D12GraphicsCommandList> command_list;

	Option<D3D12_CPU_DESCRIPTOR_HANDLE> bound_color_buffer;
	Option<D3D12_CPU_DESCRIPTOR_HANDLE> bound_depth_buffer;

	Vector<Shared<ITexture>> textures_in_use;
	Vector<Shared<IBuffer>> buffers_in_use;
};

class D3D12GraphicsCommandRecorderImpl final : public IGraphicsCommandRecorder {
public:
	explicit D3D12GraphicsCommandRecorderImpl(D3D12GraphicsCommandListImpl& command_list)
		: m_command_list(command_list) {}

	// IGraphicsCommandRecorder
	IGraphicsCommandRecorder& copy_buffer_to_texture(const ITexture& dst, const IBuffer& src) final;

	IGraphicsCommandRecorder& texture_barrier(const ITexture& texture, Layout old_layout, Layout new_layout) final;

	IGraphicsCommandRecorder& render_pass(
		const ITexture& color,
		Option<ITexture const&> depth,
		FunctionRef<void(IRenderPassCommandRecorder&)> callable
	) final;
	// ~IGraphicsCommandRecorder

private:
	D3D12GraphicsCommandListImpl& m_command_list;
};

class D3D12RenderPassRecorderImpl final : public IRenderPassCommandRecorder {
public:
	explicit D3D12RenderPassRecorderImpl(D3D12GraphicsCommandListImpl& command_list) : m_command_list(command_list) {}

	// IGraphicsCommandRecorder
	IRenderPassCommandRecorder& clear_color(const Vector4<f32>& color) final;
	IRenderPassCommandRecorder& set_pipeline(const IGraphicsPipeline& pipeline) final;
	IRenderPassCommandRecorder& set_vertices(const IBuffer& buffer, u32 stride) final;
	IRenderPassCommandRecorder& set_indices(const IBuffer& buffer) final;
	IRenderPassCommandRecorder& push_constants(const void* ptr) final;
	IRenderPassCommandRecorder& draw(usize vertex_count, usize first_vertex = 0) final;
	IRenderPassCommandRecorder& draw_index(usize index_count, usize first_index = 0) final;
	// ~IGraphicsCommandRecorder

private:
	D3D12GraphicsCommandListImpl& m_command_list;
};

SF_GPU_NAMESPACE_END
