// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/os/library.h"
#include "gpu/d3d12/d3d12_memory.h"
#include "gpu/device.h"
#include "gpu/graphics_command_list.h"

typedef HRESULT(__stdcall* PFN_D3D12_SERIALIZE_ROOT_SIGNATURE
)(const D3D12_ROOT_SIGNATURE_DESC* root_signature_desc,
  D3D_ROOT_SIGNATURE_VERSION version,
  ID3DBlob** blob,
  ID3DBlob** error_blob);

SF_GPU_NAMESPACE_BEGIN

struct D3D12QueuedWork {
	ComPtr<ID3D12Fence> fence;
	Shared<IGraphicsCommandList> command_list;

	SF_ALWAYS_INLINE explicit D3D12QueuedWork(
		const ComPtr<ID3D12Fence>& in_fence,
		Shared<IGraphicsCommandList>&& in_command_list
	)
		: fence(in_fence)
		, command_list(sf::forward<Shared<IGraphicsCommandList>>(in_command_list)) {}

	SF_ALWAYS_INLINE D3D12QueuedWork(D3D12QueuedWork&& move) noexcept
		: fence(move.fence)
		, command_list(sf::move(move.command_list)) {
		move.fence.Reset();
	}
	SF_ALWAYS_INLINE D3D12QueuedWork& operator=(D3D12QueuedWork&& move) noexcept {
		auto to_destroy = sf::move(*this);
		SF_UNUSED(to_destroy);

		fence = move.fence;
		move.fence.Reset();
		command_list = sf::move(move.command_list);

		return *this;
	}
};

class D3D12DeviceImpl final : public IDevice {
public:
	using FnSerializeRootSignature = PFN_D3D12_SERIALIZE_ROOT_SIGNATURE;
	using FnCreateDevice = PFN_D3D12_CREATE_DEVICE;

	static Shared<IDevice> create();
	explicit D3D12DeviceImpl();

	// IDevice
	Unique<ISwapchain> create_swapchain(void* handle) const final;
	Shared<IBuffer> create_buffer(BufferUsage usage, Heap kind, usize size) const final;
	Shared<ITexture> create_texture(TextureUsage usage, Format format, const Vector3<u32>& size) const final;
	Shared<IGraphicsPipeline> create_graphics_pipeline(GraphicsPipelineDefinition&& definition) const final;
	Shared<IVertexShader>
	create_vertex_shader(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters) const final;
	Shared<IPixelShader> create_pixel_shader(Vector<u8>&& binary) const final;

	Shared<IGraphicsCommandList> record_graphics(FunctionRef<void(IGraphicsCommandRecorder&)> callable) const final;
	void submit(const IGraphicsCommandList& command_list) const final;
	// ~IDevice

	void flush_queue() const;

	SF_ALWAYS_INLINE FnSerializeRootSignature serialize_root_signature() const { return m_serialize_root_signature; }
	SF_ALWAYS_INLINE const ComPtr<IDXGIFactory4>& factory() const { return m_factory; }
	SF_ALWAYS_INLINE const ComPtr<ID3D12Device1>& device() const { return m_device; }
	SF_ALWAYS_INLINE const ComPtr<ID3D12CommandQueue>& queue() const { return m_queue; }
	SF_ALWAYS_INLINE const ComPtr<ID3D12CommandAllocator>& command_allocator() const { return m_command_allocator; }
	SF_ALWAYS_INLINE const D3D12RootSignature& root_signature() const { return m_root_signature; }

private:
	Option<core::Library> m_d3d12;
	FnCreateDevice m_create_device;
	FnSerializeRootSignature m_serialize_root_signature;

#if SF_GPU_DEBUG
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
	Option<Shared<ITexture>> null_texture;

	// TODO: Wrap this in a Mutex to ensure thread safety
	mutable Vector<D3D12QueuedWork> m_queued_work;
};

SF_GPU_NAMESPACE_END
