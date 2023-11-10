// Copyright Colby Hall. All Rights Reserved.

#include "memory.h"

SF_SUPPRESS_WARNINGS_STD_BEGIN

#include <cstdio>
#include <cstdlib>
#include <cstring>

SF_SUPPRESS_WARNINGS_STD_END

SF_CORE_NAMESPACE_BEGIN

NonNull<void> malloc(const Layout& layout) {
	void* result = std::malloc(static_cast<std::size_t>(layout.size));
	return result; // Nullptr check happens inside NonNull
}

NonNull<void> realloc(NonNull<void> old_ptr, const Layout& old_layout, const Layout& new_layout) {
	SF_UNUSED(old_layout);

	void* result = std::realloc(old_ptr, static_cast<std::size_t>(new_layout.size));
	return result; // Nullptr check happens inside NonNull
}

void free(NonNull<void> ptr) { std::free(ptr); }

NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count) {
	return std::memcpy(dst, src, static_cast<std::size_t>(count));
}

NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count) {
	return std::memmove(dst, src, static_cast<std::size_t>(count));
}

NonNull<void> set(NonNull<void> ptr, u8 value, usize count) {
	return std::memset(ptr, value, static_cast<std::size_t>(count));
}

#define B2(n)	   n, n + 1, n + 1, n + 2
#define B4(n)	   B2(n), B2(n + 1), B2(n + 1), B2(n + 2)
#define B6(n)	   B4(n), B4(n + 1), B4(n + 1), B4(n + 2)
#define COUNT_BITS B6(0), B6(1), B6(1), B6(2)

static u8 count_set_bits_table[256] = { COUNT_BITS };

u8 count_ones(u8 byte) { return count_set_bits_table[byte]; }

#undef B2
#undef B4
#undef B6
#undef COUNT_BITS

SF_CORE_NAMESPACE_END
