// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Element>
class Vector {
public:
	Vector() = default;
	Vector(const Vector& copy) noexcept;
	Vector& operator=(const Vector& copy) noexcept;
	Vector(Vector&& move) noexcept;
	Vector& operator=(Vector&& move) noexcept;
	~Vector();

	EU_ALWAYS_INLINE usize len() const { return m_len; }
	EU_ALWAYS_INLINE usize cap() const { return m_cap; }

	EU_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	EU_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	EU_ALWAYS_INLINE bool is_valid_index(usize index) const;

	EU_ALWAYS_INLINE operator Slice<Element>();
	EU_ALWAYS_INLINE operator Slice<Element const>() const;

	EU_ALWAYS_INLINE Element* begin();
	EU_ALWAYS_INLINE Element* end();

	EU_ALWAYS_INLINE const Element* cbegin() const;
	EU_ALWAYS_INLINE const Element* cend() const;

	EU_ALWAYS_INLINE Element& operator[](usize index);
	EU_ALWAYS_INLINE const Element& operator[](usize index) const;

	EU_NO_DISCARD EU_ALWAYS_INLINE Option<Element&> last();
	EU_NO_DISCARD EU_ALWAYS_INLINE Option<Element const&> last() const;

	EU_ALWAYS_INLINE void reserve(usize amount);

	void insert(usize index, Element&& item);
	EU_ALWAYS_INLINE void insert(usize index, const Element& item_to_copy);
	EU_ALWAYS_INLINE usize push(Element&& item);
	EU_ALWAYS_INLINE usize push(const Element& item);

	Element remove(usize index);
	EU_ALWAYS_INLINE Option<Element> pop();
	void reset();

private:
	Element* m_ptr = nullptr;
	usize m_len = 0;
	usize m_cap = 0;
};

EU_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/vector.inl"

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Vector;
EU_NAMESPACE_END
