// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/graphics_command_list.h"
#include "gpu/d3d12/buffer.h"
#include "gpu/d3d12/device.h"
#include "gpu/d3d12/graphics_pipeline.h"
#include "gpu/d3d12/shader.h"
#include "gpu/d3d12/texture.h"

OP_GPU_NAMESPACE_BEGIN

static D3D12_RESOURCE_STATES layout_to_resource_states(Layout layout) {
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	OP_SUPPRESS_WARNING_PUSH
	OP_MSVC_SUPPRESS_WARNING(4061) // Not all switch cases are explicitly handled
	switch (layout) {
	case Layout::General:
		state = D3D12_RESOURCE_STATE_GENERIC_READ;
		break;
	case Layout::ColorAttachment:
		state = D3D12_RESOURCE_STATE_RENDER_TARGET;
		break;
	case Layout::TransferDst:
		state = D3D12_RESOURCE_STATE_COPY_DEST;
		break;
	case Layout::Present:
		state = D3D12_RESOURCE_STATE_PRESENT;
		break;
	default:
		OP_UNIMPLEMENTED;
		break;
	}
	OP_SUPPRESS_WARNING_POP

	return state;
}

GraphicsCommandRecorder&
D3D12GraphicsCommandRecorderImpl::copy_buffer_to_texture(const Texture& dst, const Buffer& src) {
	const auto& dst_interface = static_cast<const D3D12Texture&>(dst);
	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	dst_location.pResource = dst_interface.resource().Get();

	const auto& src_interface = static_cast<const D3D12Buffer&>(src);
	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	src_location.pResource = src_interface.resource().Get();

	src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src_location.PlacedFootprint.Footprint.Format = format_to_dxgi(dst_interface.format());
	src_location.PlacedFootprint.Footprint.Width = dst_interface.size().x;
	src_location.PlacedFootprint.Footprint.Height = dst_interface.size().y;
	src_location.PlacedFootprint.Footprint.Depth = dst_interface.size().z;
	src_location.PlacedFootprint.Footprint.RowPitch =
		(UINT)(format_size_in_bytes(dst_interface.format()) * dst_interface.size().x);

	m_command_list.command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

	m_command_list.textures_in_use.push(dst.to_shared());
	m_command_list.buffers_in_use.push(src.to_shared());

	return *this;
}

GraphicsCommandRecorder&
D3D12GraphicsCommandRecorderImpl::texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) {
	const auto& impl = static_cast<const D3D12Texture&>(texture);

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.pResource = impl.resource().Get();
	barrier.Transition.StateBefore = layout_to_resource_states(old_layout);
	barrier.Transition.StateAfter = layout_to_resource_states(new_layout);
	m_command_list.command_list->ResourceBarrier(1, &barrier);

	m_command_list.textures_in_use.push(texture.to_shared());

	return *this;
}

GraphicsCommandRecorder& D3D12GraphicsCommandRecorderImpl::render_pass(
	const Texture& color,
	Option<Texture const&> depth,
	FunctionRef<void(RenderPassCommandRecorder&)> callable
) {
	m_command_list.textures_in_use.push(color.to_shared());

	const D3D12_CPU_DESCRIPTOR_HANDLE* depth_handle = nullptr;
	if (depth) {
		auto& depth_texture = depth.unwrap();
		auto& impl = static_cast<const D3D12Texture&>(depth_texture);
		depth_handle = &impl.dsv_handle().handle;
		m_command_list.textures_in_use.push(depth_texture.to_shared());
	}

	const auto& impl = static_cast<const D3D12Texture&>(color);
	m_command_list.command_list->OMSetRenderTargets(1, &impl.rtv_handle().handle, 0, depth_handle);

	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (f32)impl.size().x;
	viewport.Height = (f32)impl.size().y;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_command_list.command_list->RSSetViewports(1, &viewport);

	D3D12_RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.bottom = (LONG)impl.size().y;
	rect.right = (LONG)impl.size().x;
	m_command_list.command_list->RSSetScissorRects(1, &rect);

	m_command_list.bound_color_buffer = impl.rtv_handle().handle;
	if (depth_handle) m_command_list.bound_depth_buffer = *depth_handle;

	auto render_pass_recorder = D3D12RenderPassRecorderImpl(m_command_list);
	callable(render_pass_recorder);

	if (m_command_list.bound_color_buffer) {
		m_command_list.bound_color_buffer.unwrap();
	}

	if (m_command_list.bound_depth_buffer) {
		m_command_list.bound_depth_buffer.unwrap();
	}

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::clear_color(const LinearColor& color) {
	const auto handle = m_command_list.bound_color_buffer.as_ref().unwrap();
	const f32 clear_color0[] = { color.r, color.g, color.g, color.a };
	m_command_list.command_list->ClearRenderTargetView(handle, clear_color0, 0, nullptr);

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::set_pipeline(const GraphicsPipeline& pipeline) {
	auto& impl = static_cast<const D3D12GraphicsPipeline&>(pipeline);

	m_command_list.command_list->SetPipelineState(impl.the().Get());
	m_command_list.command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::set_vertices(const Buffer& buffer, u32 stride) {
	auto& impl = static_cast<const D3D12Buffer&>(buffer);

	OP_ASSERT(impl.size() % stride == 0);
	OP_ASSERT((impl.usage() & BufferUsage::Vertex) == BufferUsage::Vertex);

	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)impl.resource()->GetGPUVirtualAddress();
	view.StrideInBytes = (UINT)stride;
	view.SizeInBytes = (UINT)(impl.size());
	m_command_list.command_list->IASetVertexBuffers(0, 1, &view);
	m_command_list.buffers_in_use.push(buffer.to_shared());

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::set_indices(const Buffer& buffer, u32 stride) {
	auto& impl = static_cast<const D3D12Buffer&>(buffer);

	OP_ASSERT(impl.size() % sizeof(u32) == 0);
	OP_ASSERT((impl.usage() & BufferUsage::Index) == BufferUsage::Index);

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)impl.resource()->GetGPUVirtualAddress();
	view.SizeInBytes = (UINT)(impl.size());

	// Currently the only supported index format is 16 or 32 bit
	auto format = DXGI_FORMAT_UNKNOWN;
	switch (stride) {
	case 2:
		format = DXGI_FORMAT_R16_UINT;
		break;
	case 4:
		format = DXGI_FORMAT_R32_UINT;
		break;
	}
	OP_ASSERT(format != DXGI_FORMAT_UNKNOWN);

	view.Format = format;
	m_command_list.command_list->IASetIndexBuffer(&view);
	m_command_list.buffers_in_use.push(buffer.to_shared());

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::push_constants(const void* ptr) {
	m_command_list.command_list->SetGraphicsRoot32BitConstants(0, 16, ptr, 0);

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::draw(usize vertex_count, usize first_vertex) {
	m_command_list.command_list->DrawInstanced((UINT)vertex_count, 1, (UINT)first_vertex, 0);

	return *this;
}

RenderPassCommandRecorder& D3D12RenderPassRecorderImpl::draw_index(usize index_count, usize first_index) {
	m_command_list.command_list->DrawIndexedInstanced((UINT)index_count, 1, (UINT)first_index, 0, 0);

	return *this;
}

OP_GPU_NAMESPACE_END
