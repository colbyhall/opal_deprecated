// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "editor/editor.h"
#include "gpu/gpu.h"

SF_EDITOR_NAMESPACE_BEGIN

class Application {
public:
	explicit Application(const gpu::IDevice& device);

	void run(FunctionRef<void()> f);

private:
	gpu::Shared<gpu::IDevice> m_device;
};

SF_EDITOR_NAMESPACE_END
