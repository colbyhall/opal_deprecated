// Copyright Colby Hall. All Rights Reserved.

#include "core/math/color.h"

OP_CORE_NAMESPACE_BEGIN

f32 srgb_to_linear(f32 value) {
	if (value <= 0.04045f) {
		return value / 12.92f;
	} else {
		return powf((value + 0.055f) / 1.055f, 2.4f);
	}
}

f32 linear_to_srgb(f32 value) {
	if (value <= 0.0031308f) {
		return value * 12.92f;
	} else {
		return 1.055f * powf(value, 1.0f / 2.4f) - 0.055f;
	}
}

SRGBColor SRGBColor::from_linear(const LinearColor& color) {
	return SRGBColor(
		(u8)(linear_to_srgb(color.r) * 255.0f),
		(u8)(linear_to_srgb(color.g) * 255.0f),
		(u8)(linear_to_srgb(color.b) * 255.0f),
		(u8)(color.a * 255.0f)
	);
}

LinearColor SRGBColor::to_linear() const {
	return LinearColor(
		srgb_to_linear(m_r / 255.0f),
		srgb_to_linear(m_g / 255.0f),
		srgb_to_linear(m_b / 255.0f),
		m_a / 255.0f
	);
}

SRGBColor LinearColor::to_srgb() const {
	return SRGBColor(
		(u8)(linear_to_srgb(r) * 255.0f),
		(u8)(linear_to_srgb(g) * 255.0f),
		(u8)(linear_to_srgb(b) * 255.0f),
		(u8)(a * 255.0f)
	);
}

OP_CORE_NAMESPACE_END