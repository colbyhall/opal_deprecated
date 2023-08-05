// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/option.h"
#include "gpu/d3d12/d3d12_context.h"

GJ_GPU_NAMESPACE_BEGIN

static Option<Context> g_context = nullptr;

const Context& Context::the() { return g_context.as_ref().unwrap(); }

const Context& init() {
	// In the future we'll have a way to swap out what graphics API we're using
	// on the backend. It'll probably be related to some engine config system or
	// an environment variable
	auto interface = Unique<D3D12ContextImpl>::make();
	g_context = Context(gj::move(interface));
	return g_context.as_ref().unwrap();
}

GJ_GPU_NAMESPACE_END