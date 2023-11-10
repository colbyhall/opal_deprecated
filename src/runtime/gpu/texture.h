// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"
#include "gpu/gpu.h"

SF_GPU_NAMESPACE_BEGIN

/**
 * Defines which pixel type an Texture can use.
 */
enum class Format : u16 { Undefined, R_U8, RGBA_U8, RGBA_U8_SRGB, RGBA_F16, RGBA_F32, D24S8_U32 };

/**
 * Provides the size of the pixel format in bytes.
 */
usize format_size_in_bytes(Format format);

/**
 * Describes how an Texture will be used throughout its lifetime.
 *
 * Implementations may use this data for optimizations. Enforced internally by implementations.
 */
enum class TextureUsage : u8 {
	/**
	 * Allows a texture to have pixel data copied into it from a buffer or other texture.
	 *
	 * @see GraphicsCommandRecorder::copy_buffer_to_texture.
	 */
	TransferDst = (1 << 0),

	/**
	 * Allows a texture to be sampled in a shader.
	 *
	 * If specified the texture will be selected using bindless architecture.
	 */
	Sampled = (1 << 1),

	/**
	 * Allows a texture to be used as a color attachment in a render pass.
	 */
	Color = (1 << 2),

	/**
	 * Allows a texture to be used as a depth attachment in a render pass.
	 */
	Depth = (1 << 3),

	/**
	 * Allows a texture to be displayed on an OS window.
	 */
	Backbuffer = (1 << 4),
};
SF_ENUM_CLASS_BITFIELD(TextureUsage)

/**
 * A specialized form of Buffer's that can be used to store a variety of information in an efficient way. They can be
 * used to store 1D, 2D, or 3D data using a predefined Format. They're allocated using Heap::Storage memory to be
 * accessed as efficiently as possible.
 *
 * The most common use cases for textures are for storing diffuse maps, normal maps, depth, etc. They can be used for
 * much more though including voxels.
 *
 * @see Device::create_texture
 */
class Texture : public SharedFromThis<Texture> {
public:
	/**
	 * Returns the TextureUsage of this texture.
	 */
	virtual TextureUsage usage() const = 0;

	/**
	 * Returns the Format of this texture.
	 */
	virtual Format format() const = 0;

	/**
	 * Returns the size of this texture in terms of Format.
	 */
	virtual Vector3<u32> size() const = 0;

	/**
	 * Returns the index of this texture in the devices bindless buffer. Shaders can access textures directly using this
	 * index.
	 *
	 * @note texture's usage must contain TextureUsage::Sampled.
	 */
	virtual u32 bindless() const = 0;

	virtual ~Texture() = default;
};

SF_GPU_NAMESPACE_END
