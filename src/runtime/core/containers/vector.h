// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/concepts.h"
#include "core/containers/option.h"
#include "core/containers/slice.h"

OP_CORE_NAMESPACE_BEGIN

/**
 * A dynamic array that can be resized at runtime.
 *
 * @tparam Element The type of element to store in the vector.
 */
template <Movable Element>
class Vector {
public:
	Vector() = default;

	/**
	 * Constructs a vector with the specified capacity.
	 *
	 * @param capacity The capacity of the vector.
	 */
	static Vector from(Slice<const Element> slice);

	Vector(const Vector& copy) noexcept;
	Vector& operator=(const Vector& copy) noexcept;
	Vector(Vector&& move) noexcept;
	Vector& operator=(Vector&& move) noexcept;

	~Vector();

	OP_ALWAYS_INLINE usize len() const { return m_len; }
	OP_ALWAYS_INLINE usize cap() const { return m_cap; }

	OP_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	OP_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	OP_ALWAYS_INLINE bool is_valid_index(usize index) const;

	OP_ALWAYS_INLINE operator Slice<Element>();
	OP_ALWAYS_INLINE operator Slice<Element const>() const;

	OP_ALWAYS_INLINE Element* begin();
	OP_ALWAYS_INLINE Element* end();

	OP_ALWAYS_INLINE const Element* begin() const;
	OP_ALWAYS_INLINE const Element* end() const;

	OP_ALWAYS_INLINE Element& operator[](usize index);
	OP_ALWAYS_INLINE const Element& operator[](usize index) const;

	OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element&> last() {
		if (len() > 0) return m_ptr[len() - 1];
		return nullopt;
	}
	OP_NO_DISCARD OP_ALWAYS_INLINE Option<Element const&> last() const {
		if (len() > 0) return m_ptr[len() - 1];
		return nullopt;
	}

	OP_ALWAYS_INLINE void reserve(usize amount);

	void insert(usize index, Element&& item);
	OP_ALWAYS_INLINE void insert(usize index, const Element& item_to_copy);
	OP_ALWAYS_INLINE usize push(Element&& item);
	OP_ALWAYS_INLINE usize push(const Element& item);

	Element remove(usize index);
	OP_ALWAYS_INLINE Option<Element> pop();
	void reset();

private:
	Element* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;
};

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/vector.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Vector;
OP_NAMESPACE_END
