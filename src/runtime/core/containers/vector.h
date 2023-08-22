// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

SF_CORE_NAMESPACE_BEGIN

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

	SF_ALWAYS_INLINE usize len() const { return m_len; }
	SF_ALWAYS_INLINE usize cap() const { return m_cap; }

	SF_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	SF_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	SF_ALWAYS_INLINE bool is_valid_index(usize index) const;

	SF_ALWAYS_INLINE operator Slice<Element>();
	SF_ALWAYS_INLINE operator Slice<Element const>() const;

	SF_ALWAYS_INLINE Element* begin();
	SF_ALWAYS_INLINE Element* end();

	SF_ALWAYS_INLINE const Element* cbegin() const;
	SF_ALWAYS_INLINE const Element* cend() const;

	SF_ALWAYS_INLINE Element& operator[](usize index);
	SF_ALWAYS_INLINE const Element& operator[](usize index) const;

	SF_NO_DISCARD SF_ALWAYS_INLINE Option<Element&> last();
	SF_NO_DISCARD SF_ALWAYS_INLINE Option<Element const&> last() const;

	SF_ALWAYS_INLINE void reserve(usize amount);

	void insert(usize index, Element&& item);
	SF_ALWAYS_INLINE void insert(usize index, const Element& item_to_copy);
	SF_ALWAYS_INLINE usize push(Element&& item);
	SF_ALWAYS_INLINE usize push(const Element& item);

	Element remove(usize index);
	SF_ALWAYS_INLINE Option<Element> pop();
	void reset();

private:
	Element* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;
};

SF_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/vector.inl"

// Export to gj namespace
SF_NAMESPACE_BEGIN
using core::Vector;
SF_NAMESPACE_END
