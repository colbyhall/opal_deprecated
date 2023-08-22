// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/d3d12_texture.h"
#include "gpu/swapchain.h"

GJ_GPU_NAMESPACE_BEGIN

class D3D12SwapchainImpl final : public ISwapchain {
public:
	static inline constexpr usize frame_count = 2;

	explicit D3D12SwapchainImpl(const D3D12DeviceImpl& device, void* window_handle);

	void wait_for_previous();

	// ISwapchain
	const ITexture& backbuffer() const final;
	void present() final;
	void resize() final;
	// ~ISwapchain

private:
	Shared<IDevice> m_device;

	HWND m_hwnd;

	ComPtr<IDXGISwapChain3> m_swapchain;
	Array<Shared<ITexture>, frame_count> m_backbuffers;
	u8 m_current;

	ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fence_event;
	u64 m_fence_value;
};

GJ_GPU_NAMESPACE_END
