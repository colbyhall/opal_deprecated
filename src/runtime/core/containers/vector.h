// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename Element>
class Vector {
public:
	static_assert(std::is_nothrow_move_constructible_v<Element> && std::is_nothrow_move_assignable_v<Element>);

	Vector() = default;
	static Vector from(Slice<const Element> slice);

	Vector(const Vector& copy) noexcept;
	Vector& operator=(const Vector& copy) noexcept;
	Vector(Vector&& move) noexcept;
	Vector& operator=(Vector&& move) noexcept;

	~Vector();

	GJ_ALWAYS_INLINE usize len() const { return m_len; }
	GJ_ALWAYS_INLINE usize cap() const { return m_cap; }

	GJ_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	GJ_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	GJ_ALWAYS_INLINE bool is_valid_index(usize index) const;

	GJ_ALWAYS_INLINE operator Slice<Element>();
	GJ_ALWAYS_INLINE operator Slice<Element const>() const;

	GJ_ALWAYS_INLINE Element* begin();
	GJ_ALWAYS_INLINE Element* end();

	GJ_ALWAYS_INLINE const Element* cbegin() const;
	GJ_ALWAYS_INLINE const Element* cend() const;

	GJ_ALWAYS_INLINE Element& operator[](usize index);
	GJ_ALWAYS_INLINE const Element& operator[](usize index) const;

	GJ_NO_DISCARD GJ_ALWAYS_INLINE Option<Element&> last();
	GJ_NO_DISCARD GJ_ALWAYS_INLINE Option<Element const&> last() const;

	GJ_ALWAYS_INLINE void reserve(usize amount);

	void insert(usize index, Element&& item);
	GJ_ALWAYS_INLINE void insert(usize index, const Element& item_to_copy);
	GJ_ALWAYS_INLINE usize push(Element&& item);
	GJ_ALWAYS_INLINE usize push(const Element& item);

	Element remove(usize index);
	GJ_ALWAYS_INLINE Option<Element> pop();
	void reset();

private:
	Element* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/vector.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Vector;
GJ_NAMESPACE_END
