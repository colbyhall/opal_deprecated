// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/non_copyable.h"
#include "gpu/gpu.h"

OP_GPU_NAMESPACE_BEGIN

class Texture;

class Swapchain : NonCopyable {
public:
	virtual const Texture& backbuffer() const = 0;
	virtual void present() = 0;
	virtual void resize() = 0;

	virtual ~Swapchain() {}
};

OP_GPU_NAMESPACE_END
