// Copyright Colby Hall. All Rights Reserved.

#include "gpu/context.h"
#include "gpu/d3d12/d3d12_swapchain.h"

GJ_GPU_NAMESPACE_BEGIN

Swapchain Swapchain::make(void* window_handle) {
	auto& context = Context::the();

	Option<Unique<ISwapchain>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Unique<D3D12SwapchainImpl>::make(window_handle);
		break;
	}

	return Swapchain{ interface.unwrap() };
}

GJ_GPU_NAMESPACE_END