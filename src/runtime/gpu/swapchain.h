// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/gpu.h"

OP_GPU_NAMESPACE_BEGIN

class Texture;

class Swapchain {
public:
	virtual const Texture& backbuffer() const = 0;
	virtual void present() = 0;
	virtual void resize() = 0;
	virtual ~Swapchain() = default;
};

OP_GPU_NAMESPACE_END
