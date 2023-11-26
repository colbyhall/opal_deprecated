// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/os/library.h"
#include "gpu/d3d12/memory.h"
#include "gpu/device.h"
#include "gpu/graphics_command_list.h"

typedef HRESULT(__stdcall* PFN_D3D12_SERIALIZE_ROOT_SIGNATURE)(
	const D3D12_ROOT_SIGNATURE_DESC* root_signature_desc,
	D3D_ROOT_SIGNATURE_VERSION version,
	ID3DBlob** blob,
	ID3DBlob** error_blob
);

OP_GPU_NAMESPACE_BEGIN

struct D3D12QueuedWork {
	ComPtr<ID3D12Fence> fence;
	Shared<GraphicsCommandList> command_list;

	OP_ALWAYS_INLINE explicit D3D12QueuedWork(
		const ComPtr<ID3D12Fence>& in_fence,
		Shared<GraphicsCommandList>&& in_command_list
	)
		: fence(in_fence)
		, command_list(op::forward<Shared<GraphicsCommandList>>(in_command_list)) {}

	OP_ALWAYS_INLINE D3D12QueuedWork(D3D12QueuedWork&& move) noexcept
		: fence(move.fence)
		, command_list(op::move(move.command_list)) {
		move.fence.Reset();
	}
	OP_ALWAYS_INLINE D3D12QueuedWork& operator=(D3D12QueuedWork&& move) noexcept {
		auto to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		fence = move.fence;
		move.fence.Reset();
		command_list = op::move(move.command_list);

		return *this;
	}
};

class D3D12Device final : public Device {
public:
	using FnSerializeRootSignature = PFN_D3D12_SERIALIZE_ROOT_SIGNATURE;
	using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;

	static Shared<Device> make();
	explicit D3D12Device();

	// Device
	Unique<Swapchain> create_swapchain(void* handle) const final;
	Shared<Buffer> create_buffer(BufferUsage usage, Heap kind, usize size) const final;
	Shared<Texture> create_texture(TextureUsage usage, Format format, const Vector3<u32>& size) const final;
	Shared<GraphicsPipeline> create_graphics_pipeline(GraphicsPipelineDefinition&& definition) const final;
	Shared<VertexShader>
	create_vertex_shader(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters) const final;
	Shared<PixelShader> create_pixel_shader(Vector<u8>&& binary) const final;

	Shared<GraphicsCommandList> record_graphics(FunctionRef<void(GraphicsCommandRecorder&)> callable) const final;
	void submit(const GraphicsCommandList& command_list) const final;
	// ~Device

	void flush_queue() const;

	OP_ALWAYS_INLINE FnSerializeRootSignature serialize_root_signature() const { return m_serialize_root_signature; }
	OP_ALWAYS_INLINE const ComPtr<IDXGIFactory4>& factory() const { return m_factory; }
	OP_ALWAYS_INLINE const ComPtr<ID3D12Device1>& device() const { return m_device; }
	OP_ALWAYS_INLINE const ComPtr<ID3D12CommandQueue>& queue() const { return m_queue; }
	OP_ALWAYS_INLINE const ComPtr<ID3D12CommandAllocator>& command_allocator() const { return m_command_allocator; }
	OP_ALWAYS_INLINE const D3D12RootSignature& root_signature() const { return m_root_signature; }

private:
	Option<core::Library> m_d3d12;
	FnCreateDevice m_create_device;
	FnSerializeRootSignature m_serialize_root_signature;

#if OP_GPU_DEBUG
	using FnGetDebugInterface = PFN_D3D12_GET_DEBUG_INTERFACE;
	FnGetDebugInterface m_get_debug_interface;
	ComPtr<ID3D12Debug> m_debug_interface;
#endif

	ComPtr<IDXGIFactory4> m_factory;
	ComPtr<ID3D12Device1> m_device;

	ComPtr<ID3D12CommandQueue> m_queue;
	ComPtr<ID3D12CommandAllocator> m_command_allocator;

	D3D12RootSignature m_root_signature;

	// Resources used to fill empty bindless slots
	Option<Shared<Texture>> null_texture;

	// TODO: Wrap this in a Mutex to ensure thread safety
	mutable Vector<D3D12QueuedWork> m_queued_work;
};

OP_GPU_NAMESPACE_END
