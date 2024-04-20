// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "gpu/d3d12/texture.h"
#include "gpu/swapchain.h"

OP_GPU_NAMESPACE_BEGIN

class D3D12Swapchain final : public Swapchain {
public:
	static inline constexpr usize frame_count = 2;

	explicit D3D12Swapchain(const D3D12Device& device, void* window_handle);
	OP_ALWAYS_INLINE D3D12Swapchain(D3D12Swapchain&& move) noexcept
		: m_device(op::move(move.m_device))
		, m_hwnd(move.m_hwnd)
		, m_swapchain(op::move(move.m_swapchain))
		, m_backbuffers(op::move(move.m_backbuffers))
		, m_current(move.m_current)
		, m_fence(op::move(move.m_fence))
		, m_fence_event(move.m_fence_event)
		, m_fence_value(move.m_fence_value) {
		move.m_hwnd = nullptr;
		m_fence_event = 0;
	}
	OP_ALWAYS_INLINE D3D12Swapchain& operator=(D3D12Swapchain&& move) noexcept {
		m_device = op::move(move.m_device);
		m_hwnd = move.m_hwnd;
		m_swapchain = op::move(move.m_swapchain);
		m_backbuffers = op::move(move.m_backbuffers);
		m_current = move.m_current;
		m_fence = op::move(move.m_fence);
		m_fence_event = move.m_fence_event;
		m_fence_value = move.m_fence_value;
		move.m_hwnd = nullptr;
		move.m_fence_event = 0;
		return *this;
	}

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

OP_GPU_NAMESPACE_END
