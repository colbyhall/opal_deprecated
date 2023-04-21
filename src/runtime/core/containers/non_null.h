// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

EU_CORE_NAMESPACE_BEGIN

template <typename T>
class NonNull {
public:
	// Only way to initialize NonNull is by a valid ptr
	EU_ALWAYS_INLINE constexpr NonNull(T* ptr) : m_ptr(ptr) {
		EU_ASSERT(m_ptr != nullptr,
				  "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessors
	EU_ALWAYS_INLINE operator T*() const { return m_ptr; }
	EU_ALWAYS_INLINE operator void*() const { return m_ptr; }
	EU_ALWAYS_INLINE T* operator->() const { return m_ptr; }
	EU_ALWAYS_INLINE T& operator*() const { return *m_ptr; }

	// Compare ops
	EU_ALWAYS_INLINE bool operator==(NonNull<T> ptr) const {
		return ptr.m_ptr == m_ptr;
	}
	EU_ALWAYS_INLINE bool operator==(T* ptr) const { return ptr == m_ptr; }
	EU_ALWAYS_INLINE bool operator!=(NonNull<T> ptr) const {
		return ptr.m_ptr != m_ptr;
	}
	EU_ALWAYS_INLINE bool operator!=(T* ptr) const { return ptr != m_ptr; }

private:
	T* m_ptr;
};

// void ptr specialization
template <>
class NonNull<void> {
public:
	// Only way to initialize NonNull is by a valid ptr
	EU_ALWAYS_INLINE constexpr NonNull(void* ptr) : m_ptr(ptr) {
		EU_ASSERT(m_ptr != nullptr,
				  "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	inline operator void*() const { return m_ptr; }

	// Compare ops
	EU_ALWAYS_INLINE bool operator==(NonNull<void> ptr) const {
		return ptr.m_ptr == m_ptr;
	}
	EU_ALWAYS_INLINE bool operator==(void* ptr) const { return ptr == m_ptr; }
	EU_ALWAYS_INLINE bool operator!=(NonNull<void> ptr) const {
		return ptr.m_ptr != m_ptr;
	}
	EU_ALWAYS_INLINE bool operator!=(void* ptr) const { return ptr != m_ptr; }

private:
	void* m_ptr;
};

// const void ptr specialization
template <>
class NonNull<void const> {
public:
	// Only way to initialize NonNull is by a valid ptr
	EU_ALWAYS_INLINE constexpr NonNull(void const* ptr) : m_ptr(ptr) {
		EU_ASSERT(m_ptr != nullptr,
				  "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	EU_ALWAYS_INLINE operator void const*() const { return m_ptr; }

	// Compare ops
	EU_ALWAYS_INLINE bool operator==(NonNull<void const> ptr) const {
		return ptr.m_ptr == m_ptr;
	}
	EU_ALWAYS_INLINE bool operator==(void const* ptr) const {
		return ptr == m_ptr;
	}
	EU_ALWAYS_INLINE bool operator!=(NonNull<void const> ptr) const {
		return ptr.m_ptr != m_ptr;
	}
	EU_ALWAYS_INLINE bool operator!=(void const* ptr) const {
		return ptr != m_ptr;
	}

private:
	void const* m_ptr;
};

EU_CORE_NAMESPACE_END

// Export NonNull out to eu namespace
EU_NAMESPACE_BEGIN
using core::NonNull;
EU_NAMESPACE_END
