// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/initializer_list.h"

GJ_CORE_NAMESPACE_BEGIN

/// Non owning reference to an array
template <typename T>
class Slice;

template <typename T>
class Slice {
public:
	// Constructors
	Slice() = default;
	GJ_ALWAYS_INLINE constexpr Slice(T* ptr, usize len) : m_ptr(ptr), m_len(len) {}
	// Copy and Move operators
	GJ_ALWAYS_INLINE Slice(const Slice<T>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	GJ_ALWAYS_INLINE Slice<T>& operator=(const Slice<T>& c);
	GJ_ALWAYS_INLINE Slice(Slice<T>&& m) noexcept;
	GJ_ALWAYS_INLINE Slice<T>& operator=(Slice<T>&& m) noexcept;

	// Array functionality
	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize len() const { return m_len; }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_empty() const { return m_len == 0; }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < m_len; }
	GJ_ALWAYS_INLINE operator bool() const { return !is_empty(); }

	// Range accessors
	GJ_ALWAYS_INLINE T* begin() const { return m_ptr; }
	GJ_ALWAYS_INLINE T* end() const { return m_ptr + m_len; }
	GJ_ALWAYS_INLINE T* cbegin() const { return m_ptr; }
	GJ_ALWAYS_INLINE T* cend() const { return m_ptr + m_len; }

	// Accessor
	GJ_ALWAYS_INLINE T& operator[](usize index) const {
		GJ_ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	GJ_ALWAYS_INLINE operator Slice<T const>() const { return { m_ptr, m_len }; }

	// Shrinks the slice by amount. Returns new len
	GJ_ALWAYS_INLINE usize shrink(usize amount) {
		GJ_ASSERT(amount <= m_len, "Can not shrink more than len");
		m_len -= amount;
		return len();
	}

private:
	T* m_ptr = nullptr;
	usize m_len = 0;
};

template <typename T>
class Slice<T const> {
public:
	// Constructors
	Slice() = default;
	GJ_ALWAYS_INLINE constexpr Slice(T const* ptr, usize len) : m_ptr(ptr), m_len(len) {}
	Slice(InitializerList<T const> list) : m_ptr(list.begin()), m_len(list.end() - list.begin()) {}

	// Copy and Move operators
	GJ_ALWAYS_INLINE Slice(const Slice<T const>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	GJ_ALWAYS_INLINE Slice<T const>& operator=(const Slice<T const>& c);
	GJ_ALWAYS_INLINE Slice(Slice<T const>&& m) noexcept;
	GJ_ALWAYS_INLINE Slice<T const>& operator=(Slice<T const>&& m) noexcept;

	// Array functionality
	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize len() const { return m_len; }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_empty() const { return m_len == 0; }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < m_len; }
	GJ_ALWAYS_INLINE operator bool() const { return !is_empty(); }

	// Range accessors
	GJ_ALWAYS_INLINE T const* begin() const { return m_ptr; }
	GJ_ALWAYS_INLINE T const* end() const { return m_ptr + m_len; }
	GJ_ALWAYS_INLINE T const* cbegin() const { return m_ptr; }
	GJ_ALWAYS_INLINE T const* cend() const { return m_ptr + m_len; }

	// Accessor
	GJ_ALWAYS_INLINE T const& operator[](usize index) const {
		GJ_ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	// Shrinks the slice by amount. Returns new len
	GJ_ALWAYS_INLINE usize shrink(usize amount) {
		GJ_ASSERT(amount <= m_len, "Can not shrink more than len");
		m_len -= amount;
		return len();
	}

private:
	T const* m_ptr = nullptr;
	usize m_len = 0;
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/slice.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Slice;
GJ_NAMESPACE_END
