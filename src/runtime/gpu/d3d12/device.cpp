// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/device.h"
#include "gpu/d3d12/buffer.h"
#include "gpu/d3d12/graphics_command_list.h"
#include "gpu/d3d12/graphics_pipeline.h"
#include "gpu/d3d12/shader.h"
#include "gpu/d3d12/swapchain.h"
#include "gpu/d3d12/texture.h"

OP_GPU_NAMESPACE_BEGIN

static D3D12_BLEND convert_blend_factor(BlendFactor factor) {
	switch (factor) {
	case BlendFactor::Zero:
		return D3D12_BLEND_ZERO;
	case BlendFactor::One:
		return D3D12_BLEND_ONE;
	case BlendFactor::SrcColor:
		return D3D12_BLEND_SRC_COLOR;
	case BlendFactor::OneMinusSrcColor:
		return D3D12_BLEND_INV_SRC_COLOR;
	case BlendFactor::DstColor:
		return D3D12_BLEND_DEST_COLOR;
	case BlendFactor::OneMinusDstColor:
		return D3D12_BLEND_INV_DEST_COLOR;
	case BlendFactor::SrcAlpha:
		return D3D12_BLEND_SRC_ALPHA;
	case BlendFactor::OneMinusSrcAlpha:
		return D3D12_BLEND_INV_SRC_ALPHA;
	}
	return D3D12_BLEND_ZERO;
}

static D3D12_BLEND_OP convert_blend_op(BlendOp op) {
	switch (op) {
	case BlendOp::Add:
		return D3D12_BLEND_OP_ADD;
	case BlendOp::Subtract:
		return D3D12_BLEND_OP_SUBTRACT;
	case BlendOp::ReverseSubtract:
		return D3D12_BLEND_OP_REV_SUBTRACT;
	case BlendOp::Min:
		return D3D12_BLEND_OP_MIN;
	case BlendOp::Max:
		return D3D12_BLEND_OP_MAX;
	}
	return D3D12_BLEND_OP_ADD;
}

Shared<Device> D3D12Device::create() {
	auto result = Shared<D3D12Device>::make();

	// Initialize null resources used to fill empty bindless slots
	result->null_texture = result->create_texture(TextureUsage::Sampled, Format::RGBA_U8, { 8, 8, 1 });

	return result;
}

D3D12Device::D3D12Device() {
	m_d3d12 = core::Library::open("d3d12.dll");
	auto& d3d12 = m_d3d12.as_mut().unwrap();

	m_create_device = d3d12.find<FnCreateDevice>("D3D12CreateDevice");
	m_serialize_root_signature = d3d12.find<FnSerializeRootSignature>("D3D12SerializeRootSignature");

#if OP_GPU_DEBUG
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
#if OP_GPU_DEBUG
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
#if OP_GPU_DEBUG
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
				// I'm really not sure how to avoid this message.
				D3D12_MESSAGE_ID_CLEARRENDERTARGETVIEW_MISMATCHINGCLEARVALUE,

				// This warning occurs when using capture frame while graphics debugging.
				D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,

				// This warning occurs when using capture frame graphics debugging.
				D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
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

		throw_if_failed(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue)));
	}

	// Create the command allocator
	throw_if_failed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator))
	);

	m_root_signature.init(*this);
}

Unique<Swapchain> D3D12Device::create_swapchain(void* handle) const {
	return Unique<D3D12Swapchain>::make(*this, handle);
}

Shared<Buffer> D3D12Device::create_buffer(BufferUsage usage, Heap kind, usize size) const {
	D3D12_HEAP_PROPERTIES heap = {};
	switch (kind) {
	case Heap::Storage:
		heap.Type = D3D12_HEAP_TYPE_DEFAULT;
		break;
	case Heap::Upload:
		heap.Type = D3D12_HEAP_TYPE_UPLOAD;
		break;
	default:
		// heap.Type = D3D12_HEAP_TYPE_READBACK;
		OP_UNIMPLEMENTED;
		break;
	}

	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = size;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	const D3D12_RESOURCE_STATES initial_state = D3D12_RESOURCE_STATE_GENERIC_READ;

	ComPtr<ID3D12Resource> resource;
	throw_if_failed(m_device->CreateCommittedResource(
		&heap,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		initial_state,
		nullptr,
		IID_PPV_ARGS(&resource)
	));

	return Shared<D3D12Buffer>::make(usage, kind, size, op::move(resource));
}

Shared<Texture> D3D12Device::create_texture(TextureUsage usage, Format format, const Vector3<u32>& size) const {
	return Shared<D3D12Texture>::make(*this, usage, format, size);
}

Shared<GraphicsCommandList> D3D12Device::record_graphics(FunctionRef<void(GraphicsCommandRecorder&)> callable) const {
	ComPtr<ID3D12GraphicsCommandList> command_list;
	throw_if_failed(m_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_command_allocator.Get(),
		nullptr,
		IID_PPV_ARGS(&command_list)
	));
	throw_if_failed(command_list->Close());

	throw_if_failed(command_list->Reset(m_command_allocator.Get(), nullptr));
	command_list->SetGraphicsRootSignature(m_root_signature.the().Get());

	auto* heap_ptr = m_root_signature.bt2dv_heap().the().Get();
	command_list->SetDescriptorHeaps(1, &heap_ptr);

	command_list->SetGraphicsRootDescriptorTable(
		static_cast<UINT>(D3D12RootSignature::bt2dv_index),
		heap_ptr->GetGPUDescriptorHandleForHeapStart()
	);

	auto result = Shared<D3D12GraphicsCommandList>::make(command_list);

	auto recorder = D3D12GraphicsCommandRecorderImpl(*result);
	callable(recorder);
	throw_if_failed(command_list->Close());

	return result;
}

Shared<GraphicsPipeline> D3D12Device::create_graphics_pipeline(GraphicsPipelineDefinition&& definition) const {
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	desc.pRootSignature = m_root_signature.the().Get();

	// InputLayout
	auto input_parameters = definition.vertex_shader->input_parameters();

	Array<D3D12_INPUT_ELEMENT_DESC, 32> input_layout;

	usize offset = 0;
	for (usize i = 0; i < input_parameters.len(); ++i) {
		D3D12_INPUT_ELEMENT_DESC input = {};
		// input.SemanticIndex = i;
		input.SemanticName = *input_parameters[i].semantic_name;

		usize size_in_bytes = 0;
		switch (input_parameters[i].primitive) {
		case ShaderPrimitive::Uint32:
			input.Format = DXGI_FORMAT_R32_UINT;
			size_in_bytes = 4;
			break;
		case ShaderPrimitive::Int32:
			input.Format = DXGI_FORMAT_R32_SINT;
			size_in_bytes = 4;
			break;
		case ShaderPrimitive::Float32:
			input.Format = DXGI_FORMAT_R32_FLOAT;
			size_in_bytes = 4;
			break;
		case ShaderPrimitive::Vector2f32:
			input.Format = DXGI_FORMAT_R32G32_FLOAT;
			size_in_bytes = 8;
			break;
		case ShaderPrimitive::Vector3f32:
			input.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			size_in_bytes = 16;
			break;
		case ShaderPrimitive::Vector4f32:
			input.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			size_in_bytes = 16;
			break;
		case ShaderPrimitive::Matrix4f32:
			OP_INVALID_CODE_PATH;
			break;
		}
		input.AlignedByteOffset = (UINT)offset;
		input.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;

		input_layout.push(input);
		offset += size_in_bytes;
	}
	desc.InputLayout.pInputElementDescs = &input_layout[0];
	desc.InputLayout.NumElements = (UINT)input_layout.len();

	// Vertex shader
	{
		auto& shader = definition.vertex_shader;
		auto binary = shader->binary();
		desc.VS.pShaderBytecode = &binary[0];
		desc.VS.BytecodeLength = static_cast<SIZE_T>(binary.len());
	}

	// Pixel shader
	{
		auto& shader = definition.pixel_shader;
		auto binary = shader->binary();
		desc.PS.pShaderBytecode = &binary[0];
		desc.PS.BytecodeLength = static_cast<SIZE_T>(binary.len());
	}

	// BlendState
	{
		D3D12_BLEND_DESC blend = {};
		blend.AlphaToCoverageEnable = FALSE;
		blend.IndependentBlendEnable = FALSE;

		D3D12_RENDER_TARGET_BLEND_DESC def = {};
		def.BlendEnable = definition.blend_enabled;
		// def.LogicOpEnable = m_config.blend_enabled;
		def.SrcBlend = convert_blend_factor(definition.src_color_blend_factor);
		def.DestBlend = convert_blend_factor(definition.dst_color_blend_factor);
		def.BlendOp = convert_blend_op(definition.color_blend_op);
		def.SrcBlendAlpha = convert_blend_factor(definition.src_alpha_blend_factor);
		def.DestBlendAlpha = convert_blend_factor(definition.dst_alpha_blend_factor);
		def.BlendOpAlpha = convert_blend_op(definition.alpha_blend_op);
		def.LogicOp = D3D12_LOGIC_OP_NOOP;
		def.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		for (int i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			blend.RenderTarget[i] = def;
		}

		desc.BlendState = blend;
	}

	// RasterizerState
	{
		// TODO: Finish this
		D3D12_RASTERIZER_DESC rasterizer_state = {};
		rasterizer_state.FillMode = D3D12_FILL_MODE_SOLID;
		switch (definition.cull_mode) {
		case CullMode::None:
			rasterizer_state.CullMode = D3D12_CULL_MODE_NONE;
			break;
		case CullMode::Front:
			rasterizer_state.CullMode = D3D12_CULL_MODE_FRONT;
			break;
		case CullMode::Back:
			rasterizer_state.CullMode = D3D12_CULL_MODE_BACK;
			break;
		}
		rasterizer_state.FrontCounterClockwise = FALSE;
		rasterizer_state.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		rasterizer_state.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		rasterizer_state.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		rasterizer_state.DepthClipEnable = TRUE;
		rasterizer_state.MultisampleEnable = FALSE;
		rasterizer_state.AntialiasedLineEnable = FALSE;
		rasterizer_state.ForcedSampleCount = 0;
		rasterizer_state.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		desc.RasterizerState = rasterizer_state;
	}

	// DepthStencilState
	if (definition.depth_attachment.is_set()) {
		desc.DepthStencilState.DepthEnable = TRUE;
		desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		desc.DepthStencilState.StencilEnable = FALSE;
	}

	desc.NumRenderTargets = (UINT)definition.color_attachments.len();
	for (usize i = 0; i < definition.color_attachments.len(); ++i) {
		desc.RTVFormats[i] = format_to_dxgi(definition.color_attachments[i]);
	}

	if (definition.depth_attachment.is_set()) {
		desc.DSVFormat = format_to_dxgi(definition.depth_attachment.unwrap());
	}

	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.SampleDesc.Count = 1;

	ComPtr<ID3D12PipelineState> pipeline;
	throw_if_failed(m_device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline)));

	return Shared<D3D12GraphicsPipeline>::make(op::forward<GraphicsPipelineDefinition>(definition), op::move(pipeline));
}

Shared<VertexShader>
D3D12Device::create_vertex_shader(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters) const {
	return Shared<D3D12VertexShader>::make(
		op::forward<Vector<u8>>(binary),
		op::forward<Vector<InputParameter>>(input_parameters)
	);
}

Shared<PixelShader> D3D12Device::create_pixel_shader(Vector<u8>&& binary) const {
	return Shared<D3D12PixelShader>::make(op::forward<Vector<u8>>(binary));
}

void D3D12Device::submit(const GraphicsCommandList& command_list) const {
	auto& casted_command_list = static_cast<const D3D12GraphicsCommandList&>(command_list);
	ID3D12CommandList* ppCommandLists[] = { casted_command_list.command_list.Get() };
	m_queue->ExecuteCommandLists(1, ppCommandLists);

	ComPtr<ID3D12Fence> fence;
	throw_if_failed(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
	throw_if_failed(m_queue->Signal(fence.Get(), 1));

	flush_queue();

	m_queued_work.push(D3D12QueuedWork(fence, command_list.to_shared()));
}

void D3D12Device::flush_queue() const {
	for (i32 i = static_cast<i32>(m_queued_work.len()) - 1; i >= 0; --i) {
		const auto value = m_queued_work[i].fence->GetCompletedValue();

		if (value > 0) {
			m_queued_work.remove(i);
		}
	}
}

OP_GPU_NAMESPACE_END
