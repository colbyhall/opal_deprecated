// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/memory.h"

EU_CORE_NAMESPACE_BEGIN

template <typename T>
inline constexpr bool is_allocator = false;

class MallocAllocator {
public:
	template <typename Element>
	class Allocation {
	public:
		// ~Begin Allocator Interface
		Allocation() = default;
		EU_ALWAYS_INLINE Element* ptr() { return m_ptr; }
		EU_ALWAYS_INLINE Element const* ptr() const { return m_ptr; }
		i32 resize(i32 current, i32 desired) {
			if (current == desired) {
				return current;
			}

			const auto old_current = current;
			while (current < desired) {
				current += desired >> 1;
				current += 1;
			}

			if (m_ptr == nullptr) {
				void* ptr = core::malloc(core::Layout::array<Element>(current));
				m_ptr = static_cast<Element*>(ptr);
			} else {
				void* ptr = core::realloc(
					m_ptr, core::Layout::array<Element>(old_current),
					core::Layout::array<Element>(current));
				m_ptr = static_cast<Element*>(ptr);
			}

			return current;
		}
		~Allocation() {
			if (m_ptr) {
				core::free(m_ptr);
				m_ptr = nullptr;
			}
		}
		// ~End Allocator Interface

	private:
		Element* m_ptr;
	};
};

template <>
inline constexpr bool is_allocator<MallocAllocator> = true;

template <i32 Count>
class InlineAllocator {
public:
	template <typename Element>
	class Allocation {
	public:
		// ~Begin Allocator Interface
		Allocation() = default;
		EU_ALWAYS_INLINE Element* ptr() {
			return reinterpret_cast<Element*>(&m_data[0]);
		}
		EU_ALWAYS_INLINE Element const* ptr() const {
			return reinterpret_cast<Element const*>(&m_data[0]);
		}
		EU_ALWAYS_INLINE i32 resize(i32 current, i32 desired) { return Count; }
		~Allocation() {}
		// ~End Allocator Interface

	private:
		alignas(Element)
			u8 m_data[sizeof(Element) * static_cast<usize>(Count)] = {};
	};
};

template <i32 Count>
inline constexpr bool is_allocator<InlineAllocator<Count>> = true;

EU_CORE_NAMESPACE_END

EU_NAMESPACE_BEGIN

using MallocAllocator = core::MallocAllocator;

template <i32 Count>
using InlineAllocator = core::InlineAllocator<Count>;

EU_NAMESPACE_END
