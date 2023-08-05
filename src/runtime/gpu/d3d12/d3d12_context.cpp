// Copyright Colby Hall. All Rights Reserved.

#include "d3d12_context.h"

EU_GPU_NAMESPACE_BEGIN

D3D12ContextImpl::D3D12ContextImpl() {
	m_d3d12 = core::Library::open("d3d12.dll");
	auto& d3d12 = m_d3d12.as_mut().unwrap();

	m_create_device = d3d12.find<FnCreateDevice>("D3D12CreateDevice");
	m_serialize_root_signature = d3d12.find<FnSerializeRootSignature>("D3D12SerializeRootSignature");

#if EU_GPU_DEBUG
	// Always enable the debug layer before doing anything DX12 related
	// so all possible errors generated while creating DX12 objects
	// are caught by the debug layer

	m_get_debug_interface = d3d12.find<FnGetDebugInterface>("D3D12GetDebugInterface");
	(m_get_debug_interface)(IID_PPV_ARGS(&m_debug_interface));

	m_debug_interface->EnableDebugLayer();
#endif

	// Find the best DXGI adapter. We're not using WARP
	ComPtr<IDXGIAdapter4> adapter;
	{
		UINT create_factory_flags = 0;
#if EU_GPU_DEBUG
		create_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#endif

		throw_if_failed(CreateDXGIFactory2(create_factory_flags, IID_PPV_ARGS(&m_factory)));

		ComPtr<IDXGIAdapter1> adapter1;
		ComPtr<IDXGIAdapter4> adapter4;

		usize max_dedicated_video_memory = 0;
		for (UINT i = 0; m_factory->EnumAdapters1(i, &adapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			adapter1->GetDesc1(&dxgiAdapterDesc1);

			// Check to see if the adapter can create a D3D12 device without
			// actually creating it. The adapter with the largest dedicated
			// video memory is favored.
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED((m_create_device)(adapter1.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > max_dedicated_video_memory) {
				max_dedicated_video_memory = dxgiAdapterDesc1.DedicatedVideoMemory;
				throw_if_failed(adapter1.As(&adapter4));
			}
		}

		adapter = adapter4;
	}

	// Create the device
	{
		ComPtr<ID3D12Device1> device1;
		auto result = (m_create_device)(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device1));
		throw_if_failed(result);

		// Enable debug messages in debug mode.
#ifdef EU_GPU_DEBUG
		ComPtr<ID3D12InfoQueue> pInfoQueue;
		if (SUCCEEDED(device1.As(&pInfoQueue))) {
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
			pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);

			// Suppress whole categories of messages
			// D3D12_MESSAGE_CATEGORY Categories[] = {};

			// Suppress messages based on their severity level
			D3D12_MESSAGE_SEVERITY Severities[] = { D3D12_MESSAGE_SEVERITY_INFO };

			// Suppress individual messages by their ID
			D3D12_MESSAGE_ID DenyIds[] = {
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE, // I'm really not sure how to avoid this
																			  // message.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,						  // This warning occurs
																			  // when using capture
																			  // frame while graphics
																			  // debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,					  // This warning occurs
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

	// Resource Descriptors
	{
		constexpr u32 rtv_heap_size = 2048;
		constexpr u32 dsv_heap_size = 2048;
		constexpr u32 btv_heap_size = 2048;

		D3D12_ROOT_PARAMETER push_constants = {};
		push_constants.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		push_constants.Constants.Num32BitValues = 16;

		D3D12_DESCRIPTOR_RANGE texture2d_range = {};
		texture2d_range.BaseShaderRegister = 0;
		texture2d_range.NumDescriptors = btv_heap_size;
		texture2d_range.OffsetInDescriptorsFromTableStart = 0;
		texture2d_range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		texture2d_range.RegisterSpace = 0;

		D3D12_ROOT_PARAMETER texture2d_table = {};
		texture2d_table.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		texture2d_table.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		texture2d_table.DescriptorTable.NumDescriptorRanges = 1;
		texture2d_table.DescriptorTable.pDescriptorRanges = &texture2d_range;

		D3D12_ROOT_PARAMETER root_params[] = { push_constants, texture2d_table };

		D3D12_ROOT_SIGNATURE_DESC desc = {};
		desc.pParameters = root_params;
		desc.NumParameters = 2;
		desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_ANISOTROPIC;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		desc.pStaticSamplers = &sampler;
		desc.NumStaticSamplers = 1;

		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		throw_if_failed((m_serialize_root_signature)(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
		throw_if_failed(m_device->CreateRootSignature(
			0,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			IID_PPV_ARGS(&m_root_signature)
		));

		m_rtv_heap.init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rtv_heap_size, false);
		m_dsv_heap.init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, dsv_heap_size, false);
		m_btv_heap.init(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, btv_heap_size, true);
	}
}

EU_GPU_NAMESPACE_END
