// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/allocator.h"
#include "core/containers/option.h"
#include "core/containers/slice.h"
#include "core/non_copyable.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Element, typename Allocator = MallocAllocator>
class Array : private NonCopyable {
public:
	static_assert(core::is_allocator<Allocator>, "Allocator is not valid. See \"core/containers/allocator.h\"");

	using Allocation = typename Allocator::template Allocation<Element>;

	Array() = default;
	EU_ALWAYS_INLINE Array(Array&& move) noexcept;
	EU_ALWAYS_INLINE Array& operator=(Array&& move) noexcept;
	~Array();

	EU_NO_DISCARD EU_ALWAYS_INLINE usize len() const { return m_len; }
	EU_NO_DISCARD EU_ALWAYS_INLINE usize cap() const { return m_cap; }

	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_empty() const { return len() == 0; }
	EU_NO_DISCARD EU_ALWAYS_INLINE operator bool() const { return !is_empty(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_valid_index(usize index) const;

	EU_ALWAYS_INLINE operator Slice<Element>();
	EU_ALWAYS_INLINE operator Slice<Element const>() const;

	EU_ALWAYS_INLINE Element* begin() { return m_allocation.ptr(); }
	EU_ALWAYS_INLINE Element* end() { return m_allocation.ptr() + m_len; }

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

	/// throws assertion if `index` is out of bounds
	Element remove(usize index);
	EU_ALWAYS_INLINE Option<Element> pop();
	/// Removes all items from array
	void reset();

private:
	Allocation m_allocation{};
	usize m_len = 0;
	usize m_cap = 0;
};

EU_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/array.inl"

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Array;
EU_NAMESPACE_END
