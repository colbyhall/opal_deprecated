// Copyright Colby Hall. All Rights Reserved.

#include "core/os/memory.h"

SF_CORE_NAMESPACE_BEGIN

template <typename Element, usize Count>
Array<Element, Count>::Array(const Array& copy) noexcept : m_len(copy.m_len) {
	for (usize i = 0; i < m_len; ++i) {
		new (begin() + i) Element(copy[i]);
	}
}

template <typename Element, usize Count>
Array<Element, Count>& Array<Element, Count>::operator=(const Array& copy
) noexcept {
	auto to_destroy = sf::move(*this);
	SF_UNUSED(to_destroy);

	m_len = copy.m_len;
	for (usize i = 0; i < m_len; ++i) {
		new (begin() + i) Element(copy[i]);
	}

	return *this;
}

template <typename Element, usize Count>
Array<Element, Count>::Array(Array&& move) noexcept : m_len(move.m_len) {
	for (usize i = 0; i < m_len; ++i) {
		new (begin() + i) Element(sf::move(move[i]));
	}

	move.m_len = 0;
}

template <typename Element, usize Count>
Array<Element, Count>& Array<Element, Count>::operator=(Array&& move) noexcept {
	auto to_destroy = sf::move(*this);
	SF_UNUSED(to_destroy);

	m_len = move.m_len;
	for (usize i = 0; i < m_len; ++i) {
		new (begin() + i) Element(sf::move(move[i]));
	}

	move.m_len = 0;

	return *this;
}

template <typename Element, usize Count>
Array<Element, Count>::~Array() {
	for (usize i = 0; i < m_len; ++i) {
		Element& item = begin()[i];
		item.~Element();
	}
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE bool Array<Element, Count>::is_valid_index(usize index) const {
	return index < len();
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Array<Element, Count>::operator Slice<Element>() {
	return { begin(), m_len };
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Array<Element, Count>::operator Slice<Element const>() const {
	return { begin(), m_len };
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE const Element* Array<Element, Count>::cbegin() const {
	return reinterpret_cast<const Element*>(m_bytes);
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE const Element* Array<Element, Count>::cend() const {
	return cbegin() + m_len;
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Element& Array<Element, Count>::operator[](usize index) {
	SF_ASSERT(is_valid_index(index), "Index out of bounds");
	return begin()[index];
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE const Element& Array<Element, Count>::operator[](usize index
) const {
	SF_ASSERT(is_valid_index(index), "Index out of bounds");
	return cbegin()[index];
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Option<Element&> Array<Element, Count>::last() {
	if (len() > 0) return begin()[len() - 1];
	return nullptr;
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Option<Element const&> Array<Element, Count>::last() const {
	if (len() > 0) return cbegin()[len() - 1];
	return nullptr;
}

template <typename Element, usize Count>
void Array<Element, Count>::insert(usize index, Element&& item) {
	SF_ASSERT(index <= m_len);
	SF_ASSERT(len() != Count, "Array is at max capacity.");

	auto* src = begin() + index;
	if (index != len()) {
		core::move(src + 1, src, (len() - index) * sizeof(Element));
		core::set(src, 0, sizeof(Element));
	}

	new (src) Element(sf::forward<Element>(item));
	m_len += 1;
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE void
Array<Element, Count>::insert(usize index, const Element& item) {
	Element copy = item;
	insert(index, sf::move(copy));
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE usize Array<Element, Count>::push(Element&& item) {
	const auto index = len();
	insert(index, sf::move(item));
	return index;
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE usize Array<Element, Count>::push(const Element& item) {
	Element copy = item;
	return push(sf::move(copy));
}

template <typename Element, usize Count>
Element Array<Element, Count>::remove(usize index) {
	SF_ASSERT(is_valid_index(index), "Index out of bounds");

	Element result = sf::move(begin()[index]);
	void* clear = &begin()[index];
	core::set(clear, 0, sizeof(Element));
	if (index < m_len - 1) {
		auto* src = begin() + index;
		sf::core::move(src, src + 1, (len() - index) * sizeof(Element));
	}
	m_len -= 1;
	return result;
}

template <typename Element, usize Count>
SF_ALWAYS_INLINE Option<Element> Array<Element, Count>::pop() {
	if (m_len > 0) {
		m_len -= 1;
		return sf::move(begin()[m_len]);
	}
	return nullptr;
}

template <typename Element, usize Count>
void Array<Element, Count>::reset() {
	const i32 count = static_cast<i32>(len());
	for (i32 i = count - 1; i >= 0; i -= 1) {
		remove(static_cast<usize>(i));
	}
}

SF_CORE_NAMESPACE_END
