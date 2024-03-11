// Copyright Colby Hall. All Rights Reserved.

#include "gpu/d3d12/swapchain.h"
#include "core/math/vector2.h"
#include "gpu/d3d12/device.h"

OP_GPU_NAMESPACE_BEGIN

D3D12Swapchain::D3D12Swapchain(const D3D12Device& device, void* window_handle) : m_device(device.to_shared())
{
	OP_ASSERT(window_handle != nullptr);

	m_hwnd = (HWND)window_handle;

	RECT rect;
	::GetClientRect(m_hwnd, &rect);
	const Vector2<u32> size = { (u32)(rect.right - rect.left), (u32)(rect.bottom - rect.top) };

	DXGI_SWAP_CHAIN_DESC1 desc = {};
	desc.BufferCount = D3D12Swapchain::frame_count;
	desc.Width = size.width;
	desc.Height = size.height;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	desc.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> swapchain1;
	throw_if_failed(
		device.factory()
			->CreateSwapChainForHwnd(device.queue().Get(), (HWND)window_handle, &desc, nullptr, nullptr, &swapchain1)
	);

	throw_if_failed(device.factory()->MakeWindowAssociation((HWND)window_handle, DXGI_MWA_NO_ALT_ENTER));

	throw_if_failed(swapchain1.As(&m_swapchain));
	m_current = (u8)m_swapchain->GetCurrentBackBufferIndex();

	throw_if_failed(device.device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fence_event = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	OP_ASSERT(m_fence_event);
	m_fence_value = 1;

	for (int i = 0; i < D3D12Swapchain::frame_count; ++i) {
		ComPtr<ID3D12Resource> resource;
		throw_if_failed(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&resource)));

		const Vector3<u32> buffer_size = { size.width, size.height, 1 };

		auto backbuffer = Shared<D3D12Texture>::make(
			device,
			TextureUsage::Color | TextureUsage::Backbuffer,
			Format::RGBA_U8,
			buffer_size,
			resource
		);
		m_backbuffers.push(op::move(backbuffer));
	}
}

const Texture& D3D12Swapchain::backbuffer() const { return *m_backbuffers[m_current]; }

void D3D12Swapchain::present()
{
	throw_if_failed(m_swapchain->Present(1, 0));
	wait_for_previous();

	auto& device = static_cast<D3D12Device&>(*m_device);
	device.command_allocator()->Reset();
}

void D3D12Swapchain::wait_for_previous()
{
	auto& device = static_cast<D3D12Device&>(*m_device);

	const auto fence_value = m_fence_value;
	throw_if_failed(device.queue()->Signal(m_fence.Get(), fence_value));
	m_fence_value += 1;

	// Wait until the previous frame is finished and then flush the work from
	// the graphics queue
	if (m_fence->GetCompletedValue() < fence_value) {
		throw_if_failed(m_fence->SetEventOnCompletion(fence_value, m_fence_event));
		WaitForSingleObject(m_fence_event, INFINITE);
	}

	device.flush_queue();
	m_current = (u8)m_swapchain->GetCurrentBackBufferIndex();
}

void D3D12Swapchain::resize()
{
	m_backbuffers.reset();

	RECT rect;
	GetClientRect(m_hwnd, &rect);
	const Vector2<u32> size = { (u32)(rect.right - rect.left), (u32)(rect.bottom - rect.top) };

	throw_if_failed(m_swapchain->ResizeBuffers(0, size.width, size.height, DXGI_FORMAT_UNKNOWN, 0));

	auto& device = static_cast<const D3D12Device&>(*m_device);
	for (int i = 0; i < D3D12Swapchain::frame_count; ++i) {
		ComPtr<ID3D12Resource> resource;
		throw_if_failed(m_swapchain->GetBuffer(i, IID_PPV_ARGS(&resource)));

		const Vector3<u32> buffer_size = { size.width, size.height, 1 };

		auto backbuffer = Shared<D3D12Texture>::make(
			device,
			TextureUsage::Color | TextureUsage::Backbuffer,
			Format::RGBA_U8,
			buffer_size,
			resource
		);
		m_backbuffers.push(op::move(backbuffer));
	}

	wait_for_previous();
}

OP_GPU_NAMESPACE_END
