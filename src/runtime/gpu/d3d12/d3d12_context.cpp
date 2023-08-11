// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/d3d12_context.h"
#include "gpu/d3d12/d3d12_graphics_command_list.h"
#include "gpu/texture.h"

GJ_GPU_NAMESPACE_BEGIN

D3D12ContextImpl::D3D12ContextImpl() {
	m_d3d12 = core::Library::open("d3d12.dll");
	auto& d3d12 = m_d3d12.as_mut().unwrap();

	m_create_device = d3d12.find<FnCreateDevice>("D3D12CreateDevice");
	m_serialize_root_signature =
		d3d12.find<FnSerializeRootSignature>("D3D12SerializeRootSignature");

#if GJ_GPU_DEBUG
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer

	m_get_debug_interface =
		d3d12.find<FnGetDebugInterface>("D3D12GetDebugInterface");
	(m_get_debug_interface)(IID_PPV_ARGS(&m_debug_interface));

	m_debug_interface->EnableDebugLayer();
#endif

	// Find the best DXGI adapter. We're not using WARP
	ComPtr<IDXGIAdapter4> adapter;
	{
		UINT create_factory_flags = 0;
#if GJ_GPU_DEBUG
		create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		throw_if_failed(
			CreateDXGIFactory2(create_factory_flags, IID_PPV_ARGS(&m_factory))
		);

		ComPtr<IDXGIAdapter1> adapter1;
		ComPtr<IDXGIAdapter4> adapter4;

		usize max_dedicated_video_memory = 0;
		for (UINT i = 0;
			 m_factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND;
			 ++i) {
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			adapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without
			// actually creating it. The adapter with the largest dedicated
			// video memory is favored.
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED((m_create_device
				)(adapter1.Get(),
				  D3D_FEATURE_LEVEL_11_0,
				  __uuidof(ID3D12Device),
				  nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory >
					max_dedicated_video_memory) {
				max_dedicated_video_memory =
					dxgiAdapterDesc1.DedicatedVideoMemory;
				throw_if_failed(adapter1.As(&adapter4));
			}
		}

		adapter = adapter4;
	}

	// Create the device
	{
		ComPtr<ID3D12Device1> device1;
		auto result = (m_create_device
		)(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device1));
		throw_if_failed(result);

		// Enable debug messages in debug mode.
#if GJ_GPU_DEBUG
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		if (SUCCEEDED(device1.As(&pInfoQueue))) {
			pInfoQueue->SetBreakOnSeverity(
				D3D12_MESSAGE_SEVERITY_CORRUPTION,
				TRUE
			);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			pInfoQueue->SetBreakOnSeverity(
				D3D12_MESSAGE_SEVERITY_WARNING,
				TRUE
			);

			// Suppress whole categories of messages
			// D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY Severities[] = {
				D3D12_MESSAGE_SEVERITY_INFO
			};

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID DenyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE, // I'm really not sure how to avoid this
																			  // message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE, // This warning occurs
														// when using capture
														// frame while graphics
														// debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE, // This warning occurs
														  // when using capture
														  // frame while
														  // graphics debugging.
			};

			D3D12_INFO_QUEUE_FILTER NewFilter = {};
			// NewFilter.DenyList.NumCategories = _countof(Categories);
			// NewFilter.DenyList.pCategoryList = Categories;
			NewFilter.DenyList.NumSeverities = _countof(Severities);
			NewFilter.DenyList.pSeverityList = Severities;
			NewFilter.DenyList.NumIDs = _countof(DenyIds);
			NewFilter.DenyList.pIDList = DenyIds;

			throw_if_failed(pInfoQueue->PushStorageFilter(&NewFilter));
		}
#endif

		m_device = device1;
	}

	// Create the direct command queue
	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;

		throw_if_failed(
			m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue))
		);
	}

	// Create the command allocator
	throw_if_failed(m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_command_allocator)
	));

	m_root_signature.init(*this);
}

void D3D12ContextImpl::post_init() {
	null_texture =
		Texture::make(Texture::Usage::Sampled, Format::RGBA_U8, { 8, 8, 1 });
}

void D3D12ContextImpl::submit(const GraphicsCommandList& command_list) const {
	auto& command_recorder =
		command_list.cast<D3D12GraphicsCommandRecorderImpl>();
	ID3D12CommandList* ppCommandLists[] = { command_recorder.command_list().Get(
	) };
	m_queue->ExecuteCommandLists(1, ppCommandLists);

	ComPtr<ID3D12Fence> fence;
	throw_if_failed(
		m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence))
	);
	throw_if_failed(m_queue->Signal(fence.Get(), 1));

	flush_queue();

	m_queued_work.push(QueuedWork{
		fence,
		command_list,
	});
}

void D3D12ContextImpl::flush_queue() const {
	for (i32 i = static_cast<i32>(m_queued_work.len()) - 1; i >= 0; --i) {
		const auto value = m_queued_work[i].fence->GetCompletedValue();

		if (value > 0) {
			m_queued_work.remove(i);
		}
	}
}

GJ_GPU_NAMESPACE_END

// TODO:
// Rename the interface method to cast to fix name collision issue
// Fix the issue with D3D12ContextImpl::submit not using interface
