// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/non_null.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <new>
#include <type_traits>
EU_SUPPRESS_WARNINGS_STD_END

EU_CORE_NAMESPACE_BEGIN
EU_MSVC_SUPPRESS_WARNING(4268)

struct Layout {
	usize size;
	usize alignment;

	template <typename T>
	static constexpr Layout single = {sizeof(T), alignof(T)};

	template <typename T>
	static inline constexpr Layout array(usize len) {
		return Layout{sizeof(T) * len, alignof(T)};
	}
};

NonNull<void> malloc(const Layout& layout);

template <typename T>
EU_ALWAYS_INLINE NonNull<T> malloc(usize len = 1) {
	static_assert(std::is_trivial_v<T>, "T must be a trivial type to malloc");
	return eu::core::malloc(Layout::array<T>(len)).as<T>();
}

NonNull<void> realloc(NonNull<void> old_ptr, const Layout& old_layout,
					  const Layout& new_layout);
void free(NonNull<void> ptr);

NonNull<void> copy(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> move(NonNull<void> dst, NonNull<void const> src, usize count);
NonNull<void> set(NonNull<void> ptr, u8 value, usize count);

EU_CORE_NAMESPACE_END
