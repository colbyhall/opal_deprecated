// Copyright Colby Hall. All Rights Reserved.

#include "gpu/context.h"
#include "gpu/d3d12/d3d12_graphics_pipeline.h"

EU_GPU_NAMESPACE_BEGIN

GraphicsPipeline GraphicsPipeline::make(Definition&& definition) {
	auto& context = Context::the();

	Option<Shared<IGraphicsPipeline>> interface;
	switch (context.backend()) {
	case Backend::D3D12:
		interface = Shared<D3D12GraphicsPipelineImpl>::make(eu::move(definition));
		break;
	}

	return GraphicsPipeline{ interface.unwrap() };
}

EU_GPU_NAMESPACE_END