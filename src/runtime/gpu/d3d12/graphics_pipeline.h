// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "gpu/d3d12/d3d12.h"
#include "gpu/graphics_pipeline.h"

OP_GPU_NAMESPACE_BEGIN

class D3D12GraphicsPipeline final : public GraphicsPipeline {
public:
	explicit D3D12GraphicsPipeline(GraphicsPipelineDefinition&& definition, ComPtr<ID3D12PipelineState>&& pipeline)
		: m_definition(op::forward<GraphicsPipelineDefinition>(definition))
		, m_pipeline(op::forward<ComPtr<ID3D12PipelineState>>(pipeline))
	{
	}

	// GraphicsPipeline
	const GraphicsPipelineDefinition& definition() const final { return m_definition; }
	// ~GraphicsPipeline

	OP_ALWAYS_INLINE ComPtr<ID3D12PipelineState> the() const { return m_pipeline; }

private:
	GraphicsPipelineDefinition m_definition;
	ComPtr<ID3D12PipelineState> m_pipeline;
};

OP_GPU_NAMESPACE_END
