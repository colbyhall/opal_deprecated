// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

class ITexture;

class ISwapchain {
public:
	virtual const ITexture& backbuffer() const = 0;
	virtual void present() = 0;
	virtual void resize() = 0;
	virtual ~ISwapchain() = default;
};

SF_GPU_NAMESPACE_END
