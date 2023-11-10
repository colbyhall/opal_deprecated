// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/non_null.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN
#include <new>
SF_SUPPRESS_WARNINGS_STD_END

SF_CORE_NAMESPACE_BEGIN
SF_MSVC_SUPPRESS_WARNING(4268)

struct Layout {
	usize size;
	usize alignment;

	template <typename T>
	static constexpr Layout single = { sizeof(T), alignof(T) };

	template <typename T>
	static inline constexpr Layout array(usize len) {
		return Layout{ sizeof(T) * len, alignof(T) };
	}
};

NonNull<void> malloc(const Layout& layout);

template <typename T>
SF_ALWAYS_INLINE NonNull<T> malloc(usize len = 1) {
	static_assert(std::is_trivial_v<T>, "Value must be a trivial type to malloc");
	return sf::core::malloc(Layout::array<T>(len)).as<T>();
}

NonNull<void> realloc(NonNull<void> old_ptr, const Layout& old_layout, const Layout& new_layout);
void free(NonNull<void> ptr);

NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> set(NonNull<void> ptr, u8 value, usize count);

u8 count_ones(u8 byte);

template <typename T>
SF_ALWAYS_INLINE u32 count_ones(T t) {
	const usize size = sizeof(T);
	void* ptr = &t;
	u8 const* u8_casted = (u8 const*)ptr;

	u32 result = 0;
	for (usize i = 0; i < size; ++i) {
		result += count_ones(u8_casted[i]);
	}

	return result;
}

SF_CORE_NAMESPACE_END