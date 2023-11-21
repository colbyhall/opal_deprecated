// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "editor/editor.h"
#include "gpu/gpu.h"

OP_EDITOR_NAMESPACE_BEGIN

class Application {
public:
	explicit Application(const gpu::Device& device);

	void run(FunctionRef<void()> f);

private:
	gpu::Shared<gpu::Device> m_device;
};

OP_EDITOR_NAMESPACE_END
