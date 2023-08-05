// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
class NonNull;

/// `Value*` but can not be set to null.
template <typename T>
class NonNull {
public:
	// Only way to initialize NonNull is by a valid ptr
	GJ_ALWAYS_INLINE constexpr NonNull(T* ptr) : m_ptr(ptr) {
		GJ_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessors
	GJ_ALWAYS_INLINE operator T*() const { return m_ptr; }
	// GJ_ALWAYS_INLINE operator void*() const { return m_ptr; }
	GJ_ALWAYS_INLINE operator NonNull<void>() const { return m_ptr; }
	GJ_ALWAYS_INLINE operator NonNull<void const>() const { return m_ptr; }
	GJ_ALWAYS_INLINE T* operator->() const { return m_ptr; }
	GJ_ALWAYS_INLINE T& operator*() const { return *m_ptr; }
	GJ_ALWAYS_INLINE T& operator[](usize index) const { return m_ptr[index]; }

	// Compare ops
	GJ_ALWAYS_INLINE bool operator==(NonNull<T> ptr) const { return ptr.m_ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator==(T* ptr) const { return ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(NonNull<T> ptr) const { return ptr.m_ptr != m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(T* ptr) const { return ptr != m_ptr; }

private:
	T* m_ptr;
};

// void ptr specialization
template <>
class NonNull<void> {
public:
	// Only way to initialize NonNull is by a valid ptr
	GJ_ALWAYS_INLINE constexpr NonNull(void* ptr) : m_ptr(ptr) {
		GJ_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	GJ_ALWAYS_INLINE operator void*() const { return m_ptr; }
	GJ_ALWAYS_INLINE void* operator*() const { return m_ptr; }

	// Compare ops
	GJ_ALWAYS_INLINE bool operator==(NonNull<void> ptr) const { return ptr.m_ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator==(void* ptr) const { return ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(NonNull<void> ptr) const { return ptr.m_ptr != m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(void* ptr) const { return ptr != m_ptr; }

	// Casting
	template <typename T>
	GJ_ALWAYS_INLINE NonNull<T> as() const {
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
	GJ_ALWAYS_INLINE constexpr NonNull(void const* ptr) : m_ptr(ptr) {
		GJ_ASSERT(m_ptr != nullptr, "NonNull only accepts pointers that are not nullptr");
	}

	// Prevent default and nullptr initialization
	NonNull() = delete;
	NonNull(NullPtr) = delete;

	// Accessor
	GJ_ALWAYS_INLINE operator void const*() const { return m_ptr; }
	GJ_ALWAYS_INLINE void const* operator*() const { return m_ptr; }

	// Compare ops
	GJ_ALWAYS_INLINE bool operator==(NonNull<void const> ptr) const { return ptr.m_ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator==(void const* ptr) const { return ptr == m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(NonNull<void const> ptr) const { return ptr.m_ptr != m_ptr; }
	GJ_ALWAYS_INLINE bool operator!=(void const* ptr) const { return ptr != m_ptr; }

	// Casting
	template <typename T>
	GJ_ALWAYS_INLINE NonNull<T> as() const {
		return static_cast<T*>(m_ptr);
	}

private:
	void const* m_ptr;
};

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::NonNull;
GJ_NAMESPACE_END
