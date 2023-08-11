// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector3.h"
#include "gpu/gpu.h"

GJ_GPU_NAMESPACE_BEGIN

class ITexture;

enum class Format : u16 {
	Undefined,
	R_U8,
	RGBA_U8,
	RGBA_U8_SRGB,
	RGBA_F16,
	RGBA_F32,
	D24S8_U32
};

usize format_size_in_bytes(Format format);

class Texture {
public:
	enum class Usage : u8 {
		TransferDst = (1 << 0),
		Sampled = (1 << 1),
		Color = (1 << 2),
		Depth = (1 << 3),
		Backbuffer = (1 << 4),
	};

	static Texture make(Usage usage, Format format, const Vector3<u32>& size);

	GJ_ALWAYS_INLINE Texture::Usage usage() const;
	GJ_ALWAYS_INLINE Format format() const;
	GJ_ALWAYS_INLINE Vector3<u32> size() const;
	GJ_ALWAYS_INLINE u32 bindless() const;

	template <typename T = ITexture>
	GJ_ALWAYS_INLINE T const& cast() const {
		static_assert(
			std::is_base_of_v<ITexture, T>,
			"T is not derived of ITexture"
		);
		return static_cast<const T&>(*m_interface);
	}

private:
	GJ_ALWAYS_INLINE explicit Texture(Shared<ITexture>&& interface)
		: m_interface(gj::move(interface)) {}

	friend class D3D12SwapchainImpl;

	Shared<ITexture> m_interface;
};

GJ_ENUM_CLASS_BITFIELD(Texture::Usage)

class ITexture {
public:
	virtual Texture::Usage usage() const = 0;
	virtual Format format() const = 0;
	virtual Vector3<u32> size() const = 0;
	virtual u32 bindless() const = 0;
	virtual ~ITexture() = default;
};

GJ_ALWAYS_INLINE Texture::Usage Texture::usage() const {
	return m_interface->usage();
}
GJ_ALWAYS_INLINE Format Texture::format() const {
	return m_interface->format();
}
GJ_ALWAYS_INLINE Vector3<u32> Texture::size() const {
	return m_interface->size();
}
GJ_ALWAYS_INLINE u32 Texture::bindless() const {
	return m_interface->bindless();
}

GJ_GPU_NAMESPACE_END
