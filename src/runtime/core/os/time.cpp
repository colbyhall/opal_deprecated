// Copyright Colby Hall. All Rights Reserved.

#include "core/os/time.h"
#include "core/containers/option.h"

#if OP_PLATFORM_WINDOWS
	#include "core/os/windows.h"
#endif

OP_CORE_NAMESPACE_BEGIN

#if OP_PLATFORM_WINDOWS

// Thread local so we don't have to lock
static thread_local Option<u64> g_timer_frequency;

Instant Instant::now() {
	LARGE_INTEGER ticks;
	const auto result = QueryPerformanceCounter(&ticks);
	OP_ASSERT(result);
	return { static_cast<u64>(ticks.QuadPart) };
}

Duration Instant::duration_since(Instant earlier) const {
	if (!g_timer_frequency.is_set()) {
		LARGE_INTEGER freq;
		const auto result = QueryPerformanceFrequency(&freq);
		OP_ASSERT(result);
		g_timer_frequency = freq.QuadPart;
	}
	const auto& freq = g_timer_frequency.as_ref().unwrap();

	auto duration = m_tick - earlier.m_tick;
	const auto secs = duration / freq;
	duration -= secs * freq;
	const auto nanos = (u32)(duration * (nanos_per_sec / freq));

	return Duration(secs, nanos);
}

#endif

OP_CORE_NAMESPACE_END