// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/unique.h"
#include "gpu/graphics_command_list.h"

GJ_GPU_NAMESPACE_BEGIN

enum class Backend {
	D3D12,
};

class IContext {
public:
	virtual Backend backend() const = 0;
	virtual void post_init() = 0;
	virtual void submit(const GraphicsCommandList& command_list) const = 0;
	virtual ~IContext() = default;
};

class Context {
public:
	static const Context& the();

	GJ_ALWAYS_INLINE Backend backend() const { return m_interface->backend(); }
	GJ_ALWAYS_INLINE void submit(const GraphicsCommandList& command_list
	) const {
		return m_interface->submit(command_list);
	}

	template <typename T = IContext>
	GJ_ALWAYS_INLINE T const& cast() const {
		static_assert(
			std::is_base_of_v<IContext, T>,
			"T is not derived of IContext"
		);
		return static_cast<const T&>(*m_interface);
	}

private:
	GJ_ALWAYS_INLINE explicit Context(Unique<IContext>&& interface)
		: m_interface(gj::forward<Unique<IContext>>(interface)) {}

	friend const Context& init();

	Unique<IContext> m_interface;
};

const Context& init();

GJ_GPU_NAMESPACE_END
