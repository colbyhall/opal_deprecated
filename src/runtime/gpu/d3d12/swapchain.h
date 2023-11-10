// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/texture.h"
#include "gpu/swapchain.h"

SF_GPU_NAMESPACE_BEGIN

class D3D12Swapchain final : public Swapchain {
public:
	static inline constexpr usize frame_count = 2;

	explicit D3D12Swapchain(const D3D12Device& device, void* window_handle);

	void wait_for_previous();

	// Swapchain
	const Texture& backbuffer() const final;
	void present() final;
	void resize() final;
	// ~Swapchain

private:
	Shared<Device> m_device;

	HWND m_hwnd;

	ComPtr<IDXGISwapChain3> m_swapchain;
	Array<Shared<Texture>, frame_count> m_backbuffers;
	u8 m_current;

	ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fence_event;
	u64 m_fence_value;
};

SF_GPU_NAMESPACE_END
