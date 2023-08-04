// Copyright Colby Hall. All Rights Reserved.

#include "memory.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN

#include <cstdio>
#include <cstdlib>
#include <cstring>

EU_SUPPRESS_WARNINGS_STD_END

EU_CORE_NAMESPACE_BEGIN

NonNull<void> malloc(const Layout& layout) {
	void* result = std::malloc(static_cast<std::size_t>(layout.size));
	return result; // Nullptr check happens inside NonNull
}

NonNull<void> realloc(NonNull<void> old_ptr, const Layout& old_layout,
					  const Layout& new_layout) {
	EU_UNUSED(old_layout);

	void* result =
		std::realloc(old_ptr, static_cast<std::size_t>(new_layout.size));
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

EU_CORE_NAMESPACE_END
