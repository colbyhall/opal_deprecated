// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Element, usize Count>
class Array {
public:
	static_assert(Count > 0, "Must have a Count greater than zero for this to be useful");

	Array() = default;
	Array(const Array& copy) noexcept;
	Array& operator=(const Array& copy) noexcept;
	Array(Array&& move) noexcept;
	Array& operator=(Array&& move) noexcept;
	~Array();

	EU_NO_DISCARD EU_ALWAYS_INLINE usize len() const { return m_len; }

	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	EU_NO_DISCARD EU_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_valid_index(usize index) const;

	EU_ALWAYS_INLINE operator Slice<Element>();
	EU_ALWAYS_INLINE operator Slice<Element const>() const;

	EU_ALWAYS_INLINE Element* begin() { return reinterpret_cast<Element*>(m_bytes); }
	EU_ALWAYS_INLINE Element* end() { return begin() + m_len; }

	EU_ALWAYS_INLINE const Element* cbegin() const;
	EU_ALWAYS_INLINE const Element* cend() const;

	EU_ALWAYS_INLINE Element& operator[](usize index);
	EU_ALWAYS_INLINE const Element& operator[](usize index) const;

	EU_NO_DISCARD EU_ALWAYS_INLINE Option<Element&> last();
	EU_NO_DISCARD EU_ALWAYS_INLINE Option<Element const&> last() const;

	/**
	 * Inserts item into array by moving item into the index given
	 *
	 * @param index - index to move the element into. can be the length of the array to insert at end
	 * @param item - element moved into array
	 */
	void insert(usize index, Element&& item);

	/**
	 * Inserts item into array by copying item into the index given
	 *
	 * @param index - index to copy the element into. can be the length of the array to insert at end
	 * @param item - element copied into array
	 */
	EU_ALWAYS_INLINE void insert(usize index, const Element& item_to_copy);

	/**
	 * Adds item to the end of array by moving it
	 *
	 * @param item - element moved into array
	 * @return index that the element was inserted at
	 */
	EU_ALWAYS_INLINE usize push(Element&& item);

	/**
	 * Adds item to the end of array by copying it
	 *
	 * @param item - element copied into array
	 * @return index that the element was inserted at
	 */
	EU_ALWAYS_INLINE usize push(const Element& item);

	/**
	 * Removes an item from the array by moving it out and then shifting moving all elements after over
	 *
	 * @param index - points to which item to remove. will crash if invalid
	 * @return removed element
	 */
	Element remove(usize index);

	/**
	 * If the array is not empty remove the last item by moving it out and decrementing len
	 *
	 * @return removed element if there was one to remove
	 */

	EU_ALWAYS_INLINE Option<Element> pop();

	/**
	 * Frees all elements in the array
	 */
	void reset();

private:
	alignas(Element[Count]) u8 m_bytes[sizeof(Element[Count])];
	usize m_len = 0;
};

EU_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/array.inl"

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Array;
EU_NAMESPACE_END
