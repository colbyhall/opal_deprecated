// Copyright Colby Hall. All Rights Reserved.

#include "core/os/memory.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Array<Element, Allocator>::Array(Array&& move) noexcept
	: m_allocation(eu::move(move.m_allocation))
	, m_len(move.m_len)
	, m_cap(move.m_cap) {
	move.m_len = 0;
	move.m_cap = 0;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Array<Element, Allocator>& Array<Element, Allocator>::operator=(Array&& move) noexcept {
	auto to_destroy = eu::move(*this);
	EU_UNUSED(to_destroy);

	m_allocation = eu::move(move.m_allocation);
	m_len = move.m_len;
	m_cap = move.m_cap;

	move.m_len = 0;
	move.m_cap = 0;
	return *this;
}

template <typename Element, typename Allocator>
Array<Element, Allocator>::~Array() {
	for (usize i = 0; i < m_len; ++i) {
		Element& item = m_allocation.ptr()[i];
		item.~Element();
	}
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE bool Array<Element, Allocator>::is_valid_index(usize index) const {
	return index < len();
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Array<Element, Allocator>::operator Slice<Element>() {
	return { m_allocation.ptr(), m_len };
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Array<Element, Allocator>::operator Slice<Element const>() const {
	return { m_allocation.ptr(), m_len };
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE const Element* Array<Element, Allocator>::cbegin() const {
	return m_allocation.ptr();
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE const Element* Array<Element, Allocator>::cend() const {
	return m_allocation.ptr() + m_len;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Element& Array<Element, Allocator>::operator[](usize index) {
	EU_ASSERT(is_valid_index(index), "Index out of bounds");
	return m_allocation.ptr()[index];
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE const Element& Array<Element, Allocator>::operator[](usize index) const {
	EU_ASSERT(is_valid_index(index), "Index out of bounds");
	return m_allocation.ptr()[index];
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Option<Element&> Array<Element, Allocator>::last() {
	if (len() > 0) return m_allocation.ptr()[len() - 1];
	return nullptr;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Option<Element const&> Array<Element, Allocator>::last() const {
	if (len() > 0) return m_allocation.ptr()[len() - 1];
	return nullptr;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE void Array<Element, Allocator>::reserve(usize amount) {
	m_cap = m_allocation.resize(m_cap, m_cap + amount);
}

template <typename Element, typename Allocator>
void Array<Element, Allocator>::insert(usize index, Element&& item) {
	EU_ASSERT(index <= m_len);
	if (len() == cap()) reserve(1);

	auto* src = m_allocation.ptr() + index;
	if (index != len()) {
		core::move(src + 1, src, (len() - index) * sizeof(Element));
		core::set(src, 0, sizeof(Element));
	}

	new (src) Element(eu::forward<Element>(item));
	m_len += 1;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE void Array<Element, Allocator>::insert(usize index, const Element& item) {
	Element copy = item;
	insert(index, eu::move(copy));
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE usize Array<Element, Allocator>::push(Element&& item) {
	const auto index = len();
	insert(index, eu::move(item));
	return index;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE usize Array<Element, Allocator>::push(const Element& item) {
	Element copy = item;
	return push(eu::move(copy));
}

template <typename Element, typename Allocator>
Element Array<Element, Allocator>::remove(usize index) {
	EU_ASSERT(is_valid_index(index), "Index out of bounds");

	Element result = eu::move(m_allocation.ptr()[index]);
	void* clear = &m_allocation.ptr()[index];
	core::set(clear, 0, sizeof(Element));
	if (index < m_len - 1) {
		auto* src = m_allocation.ptr() + index;
		eu::core::move(src, src + 1, (len() - index) * sizeof(Element));
	}
	m_len -= 1;
	return result;
}

template <typename Element, typename Allocator>
EU_ALWAYS_INLINE Option<Element> Array<Element, Allocator>::pop() {
	if (m_len > 0) {
		m_len -= 1;
		return eu::move(m_allocation.ptr()[m_len]);
	}
	return nullptr;
}

template <typename Element, typename Allocator>
void Array<Element, Allocator>::reset() {
	const i32 count = static_cast<i32>(len());
	for (i32 i = count - 1; i >= 0; i -= 1) {
		remove(static_cast<usize>(i));
	}
}

EU_CORE_NAMESPACE_END
