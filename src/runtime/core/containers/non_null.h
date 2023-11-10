// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

SF_CORE_NAMESPACE_BEGIN

template <typename T>
class NonNull;

/// `Value*` but can not be set to null.
template <typename T>
class NonNull {
public:
	// Only way to initialize NonNull is by a valid ptr
	SF_ALWAYS_INLINE constexpr NonNull(T* ptr) : m_ptr(ptr) {
		SF_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessors
	SF_ALWAYS_INLINE operator T*() const { return m_ptr; }
	// SF_ALWAYS_INLINE operator void*() const { return m_ptr; }
	SF_ALWAYS_INLINE operator NonNull<void>() const { return m_ptr; }
	SF_ALWAYS_INLINE operator NonNull<void const>() const { return m_ptr; }
	SF_ALWAYS_INLINE T* operator->() const { return m_ptr; }
	SF_ALWAYS_INLINE T& operator*() const { return *m_ptr; }
	SF_ALWAYS_INLINE T& operator[](usize index) const { return m_ptr[index]; }

	// Compare ops
	SF_ALWAYS_INLINE bool operator==(NonNull<T> ptr) const { return ptr.m_ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator==(T* ptr) const { return ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(NonNull<T> ptr) const { return ptr.m_ptr != m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(T* ptr) const { return ptr != m_ptr; }

private:
	T* m_ptr;
};

// void ptr specialization
template <>
class NonNull<void> {
public:
	// Only way to initialize NonNull is by a valid ptr
	SF_ALWAYS_INLINE constexpr NonNull(void* ptr) : m_ptr(ptr) {
		SF_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	SF_ALWAYS_INLINE operator void*() const { return m_ptr; }
	SF_ALWAYS_INLINE void* operator*() const { return m_ptr; }

	// Compare ops
	SF_ALWAYS_INLINE bool operator==(NonNull<void> ptr) const { return ptr.m_ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator==(void* ptr) const { return ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(NonNull<void> ptr) const { return ptr.m_ptr != m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(void* ptr) const { return ptr != m_ptr; }

	// Casting
	template <typename T>
	SF_ALWAYS_INLINE NonNull<T> as() const {
		return static_cast<T*>(m_ptr);
	}

private:
	void* m_ptr;
};

// const void ptr specialization
template <>
class NonNull<void const> {
public:
	// Only way to initialize NonNull is by a valid ptr
	SF_ALWAYS_INLINE constexpr NonNull(void const* ptr) : m_ptr(ptr) {
		SF_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	SF_ALWAYS_INLINE operator void const*() const { return m_ptr; }
	SF_ALWAYS_INLINE void const* operator*() const { return m_ptr; }

	// Compare ops
	SF_ALWAYS_INLINE bool operator==(NonNull<void const> ptr) const { return ptr.m_ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator==(void const* ptr) const { return ptr == m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(NonNull<void const> ptr) const { return ptr.m_ptr != m_ptr; }
	SF_ALWAYS_INLINE bool operator!=(void const* ptr) const { return ptr != m_ptr; }

	// Casting
	template <typename T>
	SF_ALWAYS_INLINE NonNull<T> as() const {
		return static_cast<T*>(m_ptr);
	}

private:
	void const* m_ptr;
};

SF_CORE_NAMESPACE_END

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::NonNull;
SF_NAMESPACE_END
