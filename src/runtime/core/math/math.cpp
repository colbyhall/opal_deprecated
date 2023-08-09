// Copyright Colby Hall. All Rights Reserved.

#include "core/math/math.h"

GJ_SUPPRESS_WARNINGS_STD_BEGIN
#include <cmath>
GJ_SUPPRESS_WARNINGS_STD_END

GJ_CORE_NAMESPACE_BEGIN

f32 cos(f32 x) { return std::cos(x); }
f64 cos(f64 x) { return std::cos(x); }

f32 sin(f32 x) { return std::sin(x); }
f64 sin(f64 x) { return std::sin(x); }

f32 tan(f32 x) { return std::tan(x); }
f64 tan(f64 x) { return std::tan(x); }

f32 acos(f32 x) { return std::acos(x); }
f64 acos(f64 x) { return std::acos(x); }

f32 asin(f32 x) { return std::asin(x); }
f64 asin(f64 x) { return std::asin(x); }

f32 atan(f32 x) { return std::atan(x); }
f64 atan(f64 x) { return std::atan(x); }

f32 atan2(f32 y, f32 x) { return std::atan2(y, x); }
f64 atan2(f64 y, f64 x) { return std::atan2(y, x); }

f32 sqrt(f32 x) { return std::sqrt(x); }
f64 sqrt(f64 x) { return std::sqrt(x); }

f32 fmod(f32 numerator, f32 denominator) {
	return std::fmod(numerator, denominator);
}
f64 fmod(f64 numerator, f64 denominator) {
	return std::fmod(numerator, denominator);
}

GJ_CORE_NAMESPACE_END
