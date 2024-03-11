// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/memory.h"
#include "gpu/d3d12/device.h"

OP_GPU_NAMESPACE_BEGIN

void D3D12DescriptorHeap::init(
	ComPtr<ID3D12Device1> device,
	D3D12_DESCRIPTOR_HEAP_TYPE type,
	usize cap,
	bool shader_visible
) {
	m_cap = cap;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = (UINT)cap;
	desc.Type = type;

	// Use bool because
	// D3D12_DESCRIPTOR_HEAP_FLAG_NONE = 0
	// D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE = 1
	desc.Flags = (D3D12_DESCRIPTOR_HEAP_FLAGS)shader_visible;

	throw_if_failed(device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap)));

	m_size = device->GetDescriptorHandleIncrementSize(type);
	m_free_slots.reserve(cap);
	// TODO: memcpy this
	for (usize i = 0; i < cap; ++i) {
		m_free_slots.push(false);
	}
}

D3D12DescriptorHandle D3D12DescriptorHeap::alloc() const {
	OP_ASSERT(m_cap > 0, "Heap needs to be initialized with D3D12DescriptorHeap::init");

	while (true) {
		for (usize i = 0; i < m_cap; ++i) {
			const auto slot = m_free_slots[i].load();
			if (!slot) {
				if (m_free_slots[i].compare_exchange_weak(false, true)) {
					D3D12_CPU_DESCRIPTOR_HANDLE handle = m_heap->GetCPUDescriptorHandleForHeapStart();
					handle.ptr += static_cast<SIZE_T>(i * m_size);
					return { handle, static_cast<u32>(i) };
				}
			}
		}
	}
}

void D3D12DescriptorHeap::free(const D3D12DescriptorHandle& handle) const {
	OP_ASSERT(m_cap > 0, "Heap needs to be initialized with D3D12DescriptorHeap::init");
	OP_ASSERT(m_free_slots[handle.index].load(), "Attempting to free a freed slot");

	m_free_slots[handle.index].store(false);
}

void D3D12RootSignature::init(const D3D12Device& context) {
	D3D12_ROOT_PARAMETER push_constants = {};
	push_constants.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	push_constants.Constants.Num32BitValues = 16;

	D3D12_DESCRIPTOR_RANGE texture2d_range = {};
	texture2d_range.BaseShaderRegister = 0;
	texture2d_range.NumDescriptors = bt2dv_heap_size;
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
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
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
	throw_if_failed((context.serialize_root_signature())(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
	throw_if_failed(context.device()->CreateRootSignature(
		0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&m_root_signature)
	));

	m_rtv_heap.init(context.device(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, rtv_heap_size, false);
	m_dsv_heap.init(context.device(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, dsv_heap_size, false);
	m_bt2dv_heap.init(context.device(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, bt2dv_heap_size, true);
}

OP_GPU_NAMESPACE_END
