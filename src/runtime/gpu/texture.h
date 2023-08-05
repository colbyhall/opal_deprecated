// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vec3.h"
#include "gpu/gpu.h"

EU_GPU_NAMESPACE_BEGIN

class ITexture;

enum class Format : u16 { Undefined, R_U8, RGBA_U8, RGBA_U8_SRGB, RGBA_F16, RGBA_F32, D24S8_U32 };

usize format_size_in_bytes(Format format);

class Texture {
public:
	enum class Usage : u8 {
		Sampled = (1 << 0),
		Color = (1 << 1),
		Depth = (1 << 2),
		Backbuffer = (1 << 3),
	};

	static Texture make(Usage usage, Format format, const Vec3u32& size);

	template <typename T = ITexture>
	T const& interface() const {
		static_assert(std::is_base_of_v<ITexture, T>, "T is not derived of ITexture");
		return static_cast<const T&>(*m_interface);
	}

private:
	EU_ALWAYS_INLINE explicit Texture(Shared<ITexture>&& interface) : m_interface(eu::move(interface)) {}

	Shared<ITexture> m_interface;
};

EU_ENUM_CLASS_BITFIELD(Texture::Usage)

class ITexture {
public:
	virtual Texture::Usage usage() const = 0;
	virtual Format format() const = 0;
	virtual Vec3u32 size() const = 0;
	virtual u32 bindless() const = 0;
	virtual ~ITexture() = default;
};

EU_GPU_NAMESPACE_END