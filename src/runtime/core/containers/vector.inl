// Copyright Colby Hall. All Rights Reserved.

#include "core/os/memory.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename Element>
Vector<Element> Vector<Element>::from(Slice<const Element> slice) {
	Vector<Element> result;

	result.m_len = slice.len();
	result.reserve(result.len());

	for (usize i = 0; i < slice.len(); ++i) {
		new (result.begin() + i) Element(slice[i]);
	}

	return result;
}

template <typename Element>
Vector<Element>::Vector(const Vector& copy) noexcept : m_len(copy.m_len) {
	reserve(m_cap);
	for (usize i = 0; i < m_len; ++i) {
		new (m_ptr + i) Element(copy[i]);
	}
}

template <typename Element>
Vector<Element>& Vector<Element>::operator=(const Vector& copy) noexcept {
	auto to_destroy = gj::move(*this);
	GJ_UNUSED(to_destroy);

	m_len = copy.m_len;
	reserve(copy.m_cap);
	for (usize i = 0; i < m_len; ++i) {
		new (m_ptr + i) Element(copy[i]);
	}

	return *this;
}

template <typename Element>
Vector<Element>::Vector(Vector&& move) noexcept
	: m_ptr(move.m_ptr)
	, m_len(move.m_len)
	, m_cap(move.m_cap) {
	move.m_ptr = nullptr;
	move.m_len = 0;
	move.m_cap = 0;
}

template <typename Element>
Vector<Element>& Vector<Element>::operator=(Vector&& move) noexcept {
	auto to_destroy = gj::move(*this);
	GJ_UNUSED(to_destroy);

	m_ptr = move.m_ptr;
	m_len = move.m_len;
	m_cap = move.m_cap;

	move.m_ptr = 0;
	move.m_len = 0;
	move.m_cap = 0;
	return *this;
}

template <typename Element>
Vector<Element>::~Vector() {
	for (usize i = 0; i < m_len; ++i) {
		Element& item = m_ptr[i];
		item.~Element();
	}
}

template <typename Element>
GJ_ALWAYS_INLINE bool Vector<Element>::is_valid_index(usize index) const {
	return index < len();
}

template <typename Element>
GJ_ALWAYS_INLINE Vector<Element>::operator Slice<Element>() {
	return { m_ptr, m_len };
}

template <typename Element>
GJ_ALWAYS_INLINE Vector<Element>::operator Slice<Element const>() const {
	return { m_ptr, m_len };
}

template <typename Element>
GJ_ALWAYS_INLINE Element* Vector<Element>::begin() {
	return m_ptr;
}

template <typename Element>
GJ_ALWAYS_INLINE Element* Vector<Element>::end() {
	return m_ptr + m_len;
}

template <typename Element>
GJ_ALWAYS_INLINE const Element* Vector<Element>::cbegin() const {
	return m_ptr;
}

template <typename Element>
GJ_ALWAYS_INLINE const Element* Vector<Element>::cend() const {
	return m_ptr + m_len;
}

template <typename Element>
GJ_ALWAYS_INLINE Element& Vector<Element>::operator[](usize index) {
	GJ_ASSERT(is_valid_index(index), "Index out of bounds");
	return m_ptr[index];
}

template <typename Element>
GJ_ALWAYS_INLINE const Element& Vector<Element>::operator[](usize index) const {
	GJ_ASSERT(is_valid_index(index), "Index out of bounds");
	return m_ptr[index];
}

template <typename Element>
GJ_ALWAYS_INLINE Option<Element&> Vector<Element>::last() {
	if (len() > 0) return m_ptr[len() - 1];
	return nullptr;
}

template <typename Element>
GJ_ALWAYS_INLINE Option<Element const&> Vector<Element>::last() const {
	if (len() > 0) return m_ptr[len() - 1];
	return nullptr;
}

template <typename Element>
GJ_ALWAYS_INLINE void Vector<Element>::reserve(usize amount) {
	const auto desired = m_cap + amount;
	const auto old_cap = m_cap;
	while (m_cap < desired) {
		m_cap += desired >> 1;
		m_cap += 1;
	}

	if (m_ptr == nullptr) {
		void* ptr = core::malloc(core::Layout::array<Element>(m_cap));
		m_ptr = static_cast<Element*>(ptr);
	} else {
		void* ptr = core::realloc(
			m_ptr,
			core::Layout::array<Element>(old_cap),
			core::Layout::array<Element>(m_cap)
		);
		m_ptr = static_cast<Element*>(ptr);
	}
}

template <typename Element>
void Vector<Element>::insert(usize index, Element&& item) {
	GJ_ASSERT(index <= m_len);
	if (len() == cap()) reserve(1);

	auto* src = m_ptr + index;
	if (index != len()) {
		core::move(src + 1, src, (len() - index) * sizeof(Element));
		core::set(src, 0, sizeof(Element));
	}

	new (src) Element(gj::forward<Element>(item));
	m_len += 1;
}

template <typename Element>
GJ_ALWAYS_INLINE void
Vector<Element>::insert(usize index, const Element& item) {
	Element copy = item;
	insert(index, gj::move(copy));
}

template <typename Element>
GJ_ALWAYS_INLINE usize Vector<Element>::push(Element&& item) {
	const auto index = len();
	insert(index, gj::move(item));
	return index;
}

template <typename Element>
GJ_ALWAYS_INLINE usize Vector<Element>::push(const Element& item) {
	Element copy = item;
	return push(gj::move(copy));
}

template <typename Element>
Element Vector<Element>::remove(usize index) {
	GJ_ASSERT(is_valid_index(index), "Index out of bounds");

	Element result = gj::move(m_ptr[index]);
	void* clear = &m_ptr[index];
	core::set(clear, 0, sizeof(Element));
	if (index < m_len - 1) {
		auto* src = m_ptr + index;
		gj::core::move(src, src + 1, (len() - index) * sizeof(Element));
	}
	m_len -= 1;
	return result;
}

template <typename Element>
GJ_ALWAYS_INLINE Option<Element> Vector<Element>::pop() {
	if (m_len > 0) {
		m_len -= 1;
		return gj::move(m_ptr[m_len]);
	}
	return nullptr;
}

template <typename Element>
void Vector<Element>::reset() {
	const i32 count = static_cast<i32>(len());
	for (i32 i = count - 1; i >= 0; i -= 1) {
		remove(static_cast<usize>(i));
	}
}

GJ_CORE_NAMESPACE_END
