// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

OP_CORE_NAMESPACE_BEGIN

constexpr u64 nanos_per_sec = 1000000000;
constexpr u64 nanos_per_milli = 1000000;
constexpr u64 nanos_per_micro = 1000;
constexpr u64 millis_per_sec = 1000;
constexpr u64 micros_per_sec = 1000000;

class Duration {
public:
	constexpr explicit Duration(u64 secs, u32 nanos) : m_secs(secs), m_nanos(nanos) {}
	OP_ALWAYS_INLINE static Duration from_millis(u64 millis) {
		return Duration(millis / millis_per_sec, (u32)(millis & millis_per_sec));
	}

	OP_ALWAYS_INLINE f32 as_secs_f32() const { return (f32)m_secs + ((f32)m_nanos / (f32)nanos_per_sec); }
	OP_ALWAYS_INLINE f64 as_secs_f64() const { return (f64)m_secs + ((f64)m_nanos / (f64)nanos_per_sec); }
	OP_ALWAYS_INLINE u64 as_millis() const { return m_secs * millis_per_sec + (u64)m_nanos / nanos_per_milli; }

private:
	u64 m_secs;
	u32 m_nanos;
};

class Instant {
public:
	static Instant now();

	Duration duration_since(Instant earlier) const;
	OP_ALWAYS_INLINE Duration elapsed() const { return Instant::now().duration_since(*this); }

private:
	constexpr Instant(u64 ticks) : m_tick(ticks) {}

	u64 m_tick;
};

OP_CORE_NAMESPACE_END

OP_NAMESPACE_BEGIN
using core::Duration;
using core::Instant;
OP_NAMESPACE_END