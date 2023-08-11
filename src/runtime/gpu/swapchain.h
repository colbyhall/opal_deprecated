// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/unique.h"
#include "gpu/gpu.h"

GJ_GPU_NAMESPACE_BEGIN

class Texture;

class ISwapchain {
public:
	virtual const Texture& backbuffer() const = 0;
	virtual void present() = 0;
	virtual void wait_for_previous() = 0;
	virtual void resize() = 0;
	virtual ~ISwapchain() = default;
};

class Swapchain {
public:
	static Swapchain make(void* window_handle);

	GJ_ALWAYS_INLINE const Texture& backbuffer() const {
		return m_interface->backbuffer();
	}
	GJ_ALWAYS_INLINE void present() { m_interface->present(); }
	GJ_ALWAYS_INLINE void wait_for_previous() {
		m_interface->wait_for_previous();
	}
	GJ_ALWAYS_INLINE void resize() { m_interface->resize(); }

	template <typename T = ISwapchain>
	GJ_ALWAYS_INLINE T const& cast() const {
		static_assert(
			std::is_base_of_v<ISwapchain, T>,
			"T is not derived of ISwapchain"
		);
		return static_cast<const T&>(*m_interface);
	}

private:
	GJ_ALWAYS_INLINE explicit Swapchain(Unique<ISwapchain>&& interface)
		: m_interface(gj::move(interface)) {}

	Unique<ISwapchain> m_interface;
};

GJ_GPU_NAMESPACE_END
