// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/slice.h"
#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

/**
 * Specifies which GPU heap an IBuffer can be allocated from.
 */
enum class Heap : u8 {
	/**
	 * Memory that can only be written to and read from on the GPU. This memory exist on the GPU and therefore is
	 * fastest to be read from during GPU operations.
	 *
	 * Typically used by any memory that will persist through many frames and won't mutate.
	 *
	 * @see IGraphicsCommandRecorder.
	 */
	Storage,

	/**
	 * Memory that can be written to on the CPU and read from on the GPU. This memory lives on the CPU therefore, its
	 * slow to read from on the GPU.
	 *
	 * Typically used for memory that will be needed for a single frame or to transfer data to `Heap::Storage` buffers
	 * or ITextures.
	 *
	 * @see IBuffer::map
	 */
	Upload,
};

/**
 * Describes how an IBuffer will be used throughout its lifetime.
 *
 * Implementations may use this data for optimizations. Enforced by implementations.
 */
enum class BufferUsage : u8 {
	/**
	 * Allows a buffer to copy its data in a command list.
	 *
	 * Typically used for upload buffers that carry data that will be transfered to gpu storage.
	 */
	TransferSrc = (1 << 0),

	/**
	 * Allows a buffer to be copied into in a command list.
	 *
	 * Typically used for vertex buffers that persist across multiple frames.
	 */
	TransferDst = (1 << 1),

	/**
	 * Allows a buffer to be used as vertex data in an IGraphicsCommandList.
	 */
	Vertex = (1 << 2),

	/**
	 * Allows a buffer to be used as index data in an IGraphicsCommandList.
	 *
	 * Buffer must be filled with u32 data.
	 */
	Index = (1 << 3),
};
SF_ENUM_CLASS_BITFIELD(BufferUsage)

/**
 * A buffer that owns an allocation to some GPU managed memory that belongs to a heap that exist either on the GPU or
 * the CPU. This is the most primitive resource that a GPU can interact with. Its usage includes being used to store
 * vertex buffers, index buffer, constant buffers, etc.
 *
 * @see IDevice::create_buffer
 * @see Heap
 * @see BufferUsage
 */
class IBuffer : public SharedFromThis<IBuffer> {
public:
	/**
	 * Returns the BufferUsage of this buffer.
	 */
	virtual BufferUsage usage() const = 0;

	/**
	 * Returns the Heap this buffer allocated from.
	 */
	virtual Heap heap() const = 0;

	/**
	 * Returns the size in bytes of this buffer.
	 */
	virtual usize size() const = 0;

	/**
	 * Maps GPU memory to the CPU to allow the transfer of data between the processing units. May only be called on a
	 * buffer that uses Heap::Upload.
	 *
	 * @param func - Functor that takes a slice to GPU mapped memory. Using a functor ensures that we never forget to
	 * unmap this memory.
	 */
	virtual void map(FunctionRef<void(Slice<u8>)> func) = 0;

	virtual ~IBuffer() = default;
};

SF_GPU_NAMESPACE_END
