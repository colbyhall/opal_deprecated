// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"
#include "core/non_copyable.h"

EU_CORE_NAMESPACE_BEGIN

template <typename T>
class Array : private NonCopyable {
public:
	Array() = default;
	EU_ALWAYS_INLINE Array(Array<T>&& move) noexcept;
	EU_ALWAYS_INLINE Array& operator=(Array<T>&& move) noexcept;
	~Array();

	EU_ALWAYS_INLINE usize len() const { return m_len; }
	EU_ALWAYS_INLINE usize cap() const { return m_cap; }

	EU_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	EU_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	EU_ALWAYS_INLINE bool is_valid_index(usize index) const {
		return index < len();
	}

	EU_ALWAYS_INLINE operator Slice<T>() { return {m_ptr, m_len}; }
	EU_ALWAYS_INLINE operator Slice<T const>() { return {m_ptr, m_len}; }

	EU_ALWAYS_INLINE T* begin() { return m_ptr; }
	EU_ALWAYS_INLINE T* end() { return m_ptr + m_len; }

	EU_ALWAYS_INLINE const T* cbegin() const { return m_ptr; }
	EU_ALWAYS_INLINE const T* cend() const { return m_ptr + m_len; }

	EU_ALWAYS_INLINE T& operator[](usize index);
	EU_ALWAYS_INLINE const T& operator[](usize index) const;

	EU_ALWAYS_INLINE Option<T&> last();
	EU_ALWAYS_INLINE Option<T const&> last() const;

	void reserve(usize amount);

	void insert(usize index, T&& item);
	EU_ALWAYS_INLINE void insert(usize index, const T& item_to_copy);
	EU_ALWAYS_INLINE usize push(T&& item);
	EU_ALWAYS_INLINE usize push(const T& item);

	/// Throws assertion if `index` is out of bounds
	T remove(usize index);
	EU_ALWAYS_INLINE Option<T> pop();
	/// Removes all items from array
	void reset();

private:
	T* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;
};

EU_CORE_NAMESPACE_END

#include "core/containers/array.inl"

EU_NAMESPACE_BEGIN
using core::Array;
EU_NAMESPACE_END
