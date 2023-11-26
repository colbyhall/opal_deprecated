// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/math/vector4.h"

OP_CORE_NAMESPACE_BEGIN

struct LinearColor;

f32 srgb_to_linear(f32 value);
f32 linear_to_srgb(f32 value);

class SRGBColor {
public:
	constexpr SRGBColor() : m_color(0xFFFFFFFF) {}
	constexpr explicit SRGBColor(u8 r, u8 g, u8 b, u8 a = 255) : m_a(a), m_b(b), m_g(g), m_r(r) {}
	constexpr SRGBColor(u32 color) : m_color(color) {}
	static SRGBColor from_linear(const LinearColor& color);
	LinearColor to_linear() const;

	OP_ALWAYS_INLINE constexpr u8 r() const { return m_r; }
	OP_ALWAYS_INLINE constexpr u8 g() const { return m_g; }
	OP_ALWAYS_INLINE constexpr u8 b() const { return m_b; }
	OP_ALWAYS_INLINE constexpr u8 a() const { return m_a; }

	OP_ALWAYS_INLINE explicit operator u32() const { return m_color; }

	OP_ALWAYS_INLINE bool operator==(const SRGBColor& other) const { return m_color == other.m_color; }
	OP_ALWAYS_INLINE bool operator!=(const SRGBColor& other) const { return m_color != other.m_color; }

	static constexpr SRGBColor white() { return SRGBColor(255, 255, 255); }
	static constexpr SRGBColor black() { return SRGBColor(0, 0, 0); }
	static constexpr SRGBColor red() { return SRGBColor(255, 0, 0); }
	static constexpr SRGBColor green() { return SRGBColor(0, 255, 0); }
	static constexpr SRGBColor blue() { return SRGBColor(0, 0, 255); }
	static constexpr SRGBColor yellow() { return SRGBColor(255, 255, 0); }
	static constexpr SRGBColor cyan() { return SRGBColor(0, 255, 255); }
	static constexpr SRGBColor magenta() { return SRGBColor(255, 0, 255); }

private:
	union {
		u32 m_color;
		struct {
			u8 m_a;
			u8 m_b;
			u8 m_g;
			u8 m_r;
		};
	};
};

struct LinearColor {
public:
	f32 r = 1.0f;
	f32 g = 1.0f;
	f32 b = 1.0f;
	f32 a = 1.0f;

	constexpr LinearColor() = default;
	constexpr LinearColor(f32 in_r, f32 in_g, f32 in_b, f32 in_a = 1.0f) : r(in_r), g(in_g), b(in_b), a(in_a) {}
	constexpr explicit LinearColor(const Vector4<f32>& colors) : r(colors.x), g(colors.y), b(colors.z), a(colors.w) {}
	static LinearColor from_srgb(const SRGBColor& color);
	SRGBColor to_srgb() const;

	OP_ALWAYS_INLINE bool operator==(const LinearColor& other) const {
		return r == other.r && g == other.g && b == other.b && a == other.a;
	}
	OP_ALWAYS_INLINE bool operator!=(const LinearColor& other) const {
		return r != other.r || g != other.g || b != other.b || a != other.a;
	}

	static constexpr LinearColor white() { return LinearColor(1.0f, 1.0f, 1.0f); }
	static constexpr LinearColor black() { return LinearColor(0.0f, 0.0f, 0.0f); }
	static constexpr LinearColor red() { return LinearColor(1.0f, 0.0f, 0.0f); }
	static constexpr LinearColor green() { return LinearColor(0.0f, 1.0f, 0.0f); }
	static constexpr LinearColor blue() { return LinearColor(0.0f, 0.0f, 1.0f); }
	static constexpr LinearColor yellow() { return LinearColor(1.0f, 1.0f, 0.0f); }
	static constexpr LinearColor cyan() { return LinearColor(0.0f, 1.0f, 1.0f); }
	static constexpr LinearColor magenta() { return LinearColor(1.0f, 0.0f, 1.0f); }
};

OP_CORE_NAMESPACE_END

OP_NAMESPACE_BEGIN
using core::LinearColor;
using core::SRGBColor;
OP_NAMESPACE_END