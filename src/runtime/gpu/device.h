// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/unique.h"
#include "core/math/vector3.h"
#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

/**
 * Provides an interface for creating resources and submitting commands to a GPU. Owns the queues used to submit
 * IGraphicsCommandList along with the heaps that resources allocate from.
 */
class IDevice : public SharedFromThis<IDevice> {
public:
	/**
	 * Creates a swapchain and binds it to an OS window.
	 *
	 * @param handle - the handle for an OS window.
	 */
	virtual Unique<ISwapchain> create_swapchain(void* handle) const = 0;

	/**
	 * Creates a buffer that can be used to manage gpu memory.
	 *
	 * @param usage - the bitflag that describes how this buffer will be used. Implementations can use this for
	 * optimization purposes. It will also be enforced by the api.
	 *
	 * @param heap - specifies which gpu heap to allocate memory from.
	 *
	 * @param size - the size of memory allocated
	 */
	virtual Shared<IBuffer> create_buffer(BufferUsage usage, Heap heap, usize size) const = 0;

	/**
	 * Creates a texture
	 *
	 * @param usage - the bitflag that describes how this texture will be used. Implementations can use this for
	 * optimization purposes. It will also be enforced by the api.
	 *
	 * @param format - the format of the textures pixels.
	 *
	 * @param size - the size of the texture. All size components must be greater than 0.
	 */
	virtual Shared<ITexture> create_texture(TextureUsage usage, Format format, const Vector3<u32>& size) const = 0;

	/**
	 * Creates a graphics pipeline from the given definition.
	 *
	 * @param definition - the definition of this pipeline.
	 */
	virtual Shared<IGraphicsPipeline> create_graphics_pipeline(GraphicsPipelineDefinition&& definition) const = 0;

	/**
	 * Creates a vertex shader using a compiled binary.
	 *
	 * @param binary - dxc compiled vertex shader binary.
	 *
	 * @param input_parameters - input parameters specified in the shader outputted from dxc.
	 */
	virtual Shared<IVertexShader>
	create_vertex_shader(Vector<u8>&& binary, Vector<InputParameter>&& input_parameters) const = 0;

	/**
	 * Creates a pixel shader using a compiled binary.
	 *
	 * @param binary - dxc compiled pixel shader binary.
	 */
	virtual Shared<IPixelShader> create_pixel_shader(Vector<u8>&& binary) const = 0;

	/**
	 * Records commands to an IGraphicsCommandList. When recording IGraphicsCommandList take advantage of the safety
	 * provided by functors to allow some form of compile time checking.
	 *
	 * @param callable - a FunctionRef which takes an IGraphicsCommandRecorder to record graphics commands.
	 */
	virtual Shared<IGraphicsCommandList> record_graphics(FunctionRef<void(IGraphicsCommandRecorder&)> callable
	) const = 0;

	/**
	 * Submits an IGraphicsCommandList to be executed on the gpu.
	 *
	 * @param command_list - the command list to be executed.
	 */
	virtual void submit(const IGraphicsCommandList& command_list) const = 0;

	virtual ~IDevice() = default;
};

SF_GPU_NAMESPACE_END
