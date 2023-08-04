// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/unique.h"
#include "gpu/gpu.h"

EU_GPU_NAMESPACE_BEGIN

enum class Backend {
	D3D12,
};

class IContext {
public:
	virtual Backend backend() const = 0;
	virtual ~IContext() = default;
};

class Context {
public:
	static const Context& the();

	EU_NO_DISCARD EU_ALWAYS_INLINE Backend backend() const {
		return m_interface->backend();
	}

private:
	EU_ALWAYS_INLINE explicit Context(Unique<IContext>&& interface) :
		m_interface(eu::forward<Unique<IContext>>(interface)) {}

	friend const Context& init();

	Unique<IContext> m_interface;
};

const Context& init();

EU_GPU_NAMESPACE_END