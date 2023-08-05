// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/d3d12_graphics_command_list.h"
#include "gpu/d3d12/d3d12_buffer.h"
#include "gpu/d3d12/d3d12_context.h"
#include "gpu/d3d12/d3d12_graphics_pipeline.h"
#include "gpu/d3d12/d3d12_texture.h"

GJ_GPU_NAMESPACE_BEGIN

static D3D12_RESOURCE_STATES layout_to_resource_states(Layout layout) {
	D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;

	GJ_SUPPRESS_WARNING_PUSH
	GJ_MSVC_SUPPRESS_WARNING(4061) // Not all switch cases are explicitly handled
	switch (layout) {
	case Layout::Undefined:
		break;
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
		GJ_UNIMPLEMENTED;
		break;
	}
	GJ_SUPPRESS_WARNING_POP

	return state;
}

D3D12GraphicsCommandRecorderImpl::D3D12GraphicsCommandRecorderImpl() {
	auto& context = Context::the().interface<D3D12ContextImpl>();

	throw_if_failed(context.device()->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		context.command_allocator().Get(),
		nullptr,
		IID_PPV_ARGS(&m_command_list)
	));
	throw_if_failed(m_command_list->Close());
}

void D3D12GraphicsCommandRecorderImpl::begin() {
	auto& context = Context::the().interface<D3D12ContextImpl>();

	// throw_if_failed(context.command_allocator->Reset());
	throw_if_failed(m_command_list->Reset(context.command_allocator().Get(), nullptr));
	m_command_list->SetGraphicsRootSignature(context.root_signature().the().Get());

	auto* heap_ptr = context.root_signature().bt2dv_heap().the().Get();
	m_command_list->SetDescriptorHeaps(1, &heap_ptr);

	m_command_list->SetGraphicsRootDescriptorTable(
		(UINT)D3D12RootSignatureImpl::bt2dv_index,
		context.root_signature().bt2dv_heap().the()->GetGPUDescriptorHandleForHeapStart()
	);
}

void D3D12GraphicsCommandRecorderImpl::copy_buffer_to_texture(const Texture& dst, const Buffer& src) {
	const auto& dst_interface = dst.interface<D3D12TextureImpl>();
	D3D12_TEXTURE_COPY_LOCATION dst_location = {};
	dst_location.pResource = dst_interface.resource().Get();

	const auto& src_interface = src.interface<D3D12BufferImpl>();
	D3D12_TEXTURE_COPY_LOCATION src_location = {};
	src_location.pResource = src_interface.resource().Get();

	src_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	src_location.PlacedFootprint.Footprint.Format = format_to_dxgi(dst_interface.format());
	src_location.PlacedFootprint.Footprint.Width = dst_interface.size().x;
	src_location.PlacedFootprint.Footprint.Height = dst_interface.size().y;
	src_location.PlacedFootprint.Footprint.Depth = dst_interface.size().z;
	src_location.PlacedFootprint.Footprint.RowPitch =
		(UINT)(format_size_in_bytes(dst_interface.format()) * dst_interface.size().x);

	m_command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

	m_textures_in_use.push(dst);
	m_buffers_in_use.push(src);
}

void D3D12GraphicsCommandRecorderImpl::texture_barrier(const Texture& texture, Layout old_layout, Layout new_layout) {
	const auto& interface = texture.interface<D3D12TextureImpl>();

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.pResource = interface.resource().Get();
	barrier.Transition.StateBefore = layout_to_resource_states(old_layout);
	barrier.Transition.StateAfter = layout_to_resource_states(new_layout);
	m_command_list->ResourceBarrier(1, &barrier);

	m_textures_in_use.push(texture);
}

void D3D12GraphicsCommandRecorderImpl::begin_render_pass(const Texture& color, Option<const Texture&> depth) {
	m_textures_in_use.push(color);

	const D3D12_CPU_DESCRIPTOR_HANDLE* depth_handle = nullptr;
	if (depth) {
		auto& depth_texture = depth.unwrap();
		auto& interface = depth_texture.interface<D3D12TextureImpl>();
		depth_handle = &interface.dsv_handle().handle;
		m_textures_in_use.push(depth_texture);
	}

	const auto& interface = color.interface<D3D12TextureImpl>();
	m_command_list->OMSetRenderTargets(1, &interface.rtv_handle().handle, 0, depth_handle);

	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (f32)interface.size().x;
	viewport.Height = (f32)interface.size().y;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_command_list->RSSetViewports(1, &viewport);

	D3D12_RECT rect = {};
	rect.left = 0;
	rect.top = 0;
	rect.bottom = (LONG)interface.size().y;
	rect.right = (LONG)interface.size().x;
	m_command_list->RSSetScissorRects(1, &rect);

	m_bound_color_buffer = interface.rtv_handle().handle;
	if (depth_handle) m_bound_depth_buffer = *depth_handle;
}

void D3D12GraphicsCommandRecorderImpl::set_pipeline(const GraphicsPipeline& pipeline) {
	auto& interface = pipeline.interface<D3D12GraphicsPipelineImpl>();
	m_command_list->SetPipelineState(interface.the().Get());
	m_command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D12GraphicsCommandRecorderImpl::set_vertices(const Buffer& buffer, u32 stride) {
	auto& interface = buffer.interface<D3D12BufferImpl>();

	GJ_ASSERT(interface.len() % stride == 0);
	GJ_ASSERT((interface.usage() & Buffer::Usage::Vertex) == Buffer::Usage::Vertex);

	D3D12_VERTEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)interface.resource()->GetGPUVirtualAddress();
	view.StrideInBytes = (UINT)stride;
	view.SizeInBytes = (UINT)(interface.len());
	m_command_list->IASetVertexBuffers(0, 1, &view);
	m_buffers_in_use.push(buffer);
}

void D3D12GraphicsCommandRecorderImpl::set_indices(const Buffer& buffer) {
	auto& interface = buffer.interface<D3D12BufferImpl>();

	GJ_ASSERT(interface.len() % sizeof(u32) == 0);
	GJ_ASSERT((interface.usage() & Buffer::Usage::Index) == Buffer::Usage::Index);

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = (UINT)interface.resource()->GetGPUVirtualAddress();
	view.SizeInBytes = (UINT)(interface.len());
	view.Format = DXGI_FORMAT_R32_UINT; // All index buffers must be of u32
	m_command_list->IASetIndexBuffer(&view);
	m_buffers_in_use.push(buffer);
}

void D3D12GraphicsCommandRecorderImpl::push_constant(const void* ptr) {
	m_command_list->SetGraphicsRoot32BitConstants(0, 16, ptr, 0);
}

void D3D12GraphicsCommandRecorderImpl::draw(usize vertex_count, usize first_vertex) {
	m_command_list->DrawInstanced((UINT)vertex_count, 1, (UINT)first_vertex, 0);
}

void D3D12GraphicsCommandRecorderImpl::draw_indexed(usize index_count, usize first_index) {
	m_command_list->DrawIndexedInstanced((UINT)index_count, 1, (UINT)first_index, 0, 0);
}

void D3D12GraphicsCommandRecorderImpl::end_render_pass() {
	if (m_bound_color_buffer) {
		m_bound_color_buffer.unwrap();
	}

	if (m_bound_depth_buffer) {
		m_bound_depth_buffer.unwrap();
	}
}

void D3D12GraphicsCommandRecorderImpl::end() { throw_if_failed(m_command_list->Close()); }

GJ_GPU_NAMESPACE_END