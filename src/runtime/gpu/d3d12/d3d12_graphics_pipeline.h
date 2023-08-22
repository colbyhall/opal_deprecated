// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12.h"
#include "gpu/graphics_pipeline.h"

GJ_GPU_NAMESPACE_BEGIN

class D3D12GraphicsPipelineImpl final : public IGraphicsPipeline {
public:
	explicit D3D12GraphicsPipelineImpl(GraphicsPipelineDefinition&& definition, ComPtr<ID3D12PipelineState>&& pipeline)
		: m_definition(gj::forward<GraphicsPipelineDefinition>(definition))
		, m_pipeline(gj::forward<ComPtr<ID3D12PipelineState>>(pipeline)) {}

	// IGraphicsPipeline
	const GraphicsPipelineDefinition& definition() const final { return m_definition; }
	// ~IGraphicsPipeline

	GJ_ALWAYS_INLINE ComPtr<ID3D12PipelineState> the() const { return m_pipeline; }

private:
	GraphicsPipelineDefinition m_definition;
	ComPtr<ID3D12PipelineState> m_pipeline;
};

GJ_GPU_NAMESPACE_END
