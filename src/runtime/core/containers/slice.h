// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/initializer_list.h"

OP_CORE_NAMESPACE_BEGIN

/// Non owning reference to an array
template <typename T>
class Slice;

template <typename T>
class Slice {
public:
	// Constructors
	Slice() = default;
	OP_ALWAYS_INLINE constexpr explicit Slice(T* ptr, usize len) : m_ptr(ptr), m_len(len) {}

	// Copy and Move operators
	OP_ALWAYS_INLINE Slice(const Slice<T>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	OP_ALWAYS_INLINE Slice<T>& operator=(const Slice<T>& c);
	OP_ALWAYS_INLINE Slice(Slice<T>&& m) noexcept;
	OP_ALWAYS_INLINE Slice<T>& operator=(Slice<T>&& m) noexcept;

	// Array functionality
	OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_len; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_empty() const { return m_len == 0; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < m_len; }
	OP_ALWAYS_INLINE operator bool() const { return !is_empty(); }

	// Range accessors
	OP_ALWAYS_INLINE T* begin() const { return m_ptr; }
	OP_ALWAYS_INLINE T* end() const { return m_ptr + m_len; }
	OP_ALWAYS_INLINE T* cbegin() const { return m_ptr; }
	OP_ALWAYS_INLINE T* cend() const { return m_ptr + m_len; }

	// Accessor
	OP_ALWAYS_INLINE T& operator[](usize index) const {
		OP_ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	OP_ALWAYS_INLINE operator Slice<T const>() const { return { m_ptr, m_len }; }

	// Shrinks the slice by amount. Returns new len
	OP_ALWAYS_INLINE usize shrink(usize amount) {
		OP_ASSERT(amount <= m_len, "Can not shrink more than len");
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
	OP_ALWAYS_INLINE constexpr Slice(T const* ptr, usize len) : m_ptr(ptr), m_len(len) {}
	Slice(InitializerList<T const> list) : m_ptr(list.begin()), m_len(list.end() - list.begin()) {}

	// Copy and Move operators
	OP_ALWAYS_INLINE Slice(const Slice<T const>& c) : m_ptr(c.m_ptr), m_len(c.m_len) {}
	OP_ALWAYS_INLINE Slice<T const>& operator=(const Slice<T const>& c);
	OP_ALWAYS_INLINE Slice(Slice<T const>&& m) noexcept;
	OP_ALWAYS_INLINE Slice<T const>& operator=(Slice<T const>&& m) noexcept;

	// Array functionality
	OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_len; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_empty() const { return m_len == 0; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_valid_index(usize index) const { return index < m_len; }
	OP_ALWAYS_INLINE operator bool() const { return !is_empty(); }

	// Range accessors
	OP_ALWAYS_INLINE T const* begin() const { return m_ptr; }
	OP_ALWAYS_INLINE T const* end() const { return m_ptr + m_len; }
	OP_ALWAYS_INLINE T const* cbegin() const { return m_ptr; }
	OP_ALWAYS_INLINE T const* cend() const { return m_ptr + m_len; }

	// Accessor
	OP_ALWAYS_INLINE T const& operator[](usize index) const {
		OP_ASSERT(is_valid_index(index), "Index out of bounds.");
		return m_ptr[index];
	}

	// Shrinks the slice by amount. Returns new len
	OP_ALWAYS_INLINE usize shrink(usize amount) {
		OP_ASSERT(amount <= m_len, "Can not shrink more than len");
		m_len -= amount;
		return len();
	}

private:
	T const* m_ptr = nullptr;
	usize m_len = 0;
};

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/slice.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Slice;
OP_NAMESPACE_END
