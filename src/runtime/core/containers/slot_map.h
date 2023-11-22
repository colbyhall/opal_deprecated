// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"

OP_CORE_NAMESPACE_BEGIN

/**
 * A slot map is a data structure that allows for constant time insertion and removal of elements.
 *
 * @tparam T The type of element to store in the slot map.
 */
template <typename T>
class SlotMap {
public:
	class Key {
	public:
		explicit Key(u32 index, u32 version) : m_index(index), m_version(version) {}

		OP_ALWAYS_INLINE bool operator==(const Key& other) const {
			return m_index == other.m_index && m_version == other.m_version;
		}
		OP_ALWAYS_INLINE bool operator!=(const Key& other) const { return !(*this == other); }

	private:
		friend class SlotMap<T>;
		
		u32 m_index;
		u32 m_version;
	};

	SlotMap() = default;

	OP_NO_DISCARD Key insert(T&& value);

	OP_NO_DISCARD Option<T&> get(const Key& key);

	OP_NO_DISCARD Option<T const&> get(const Key& key) const;

	OP_NO_DISCARD bool is_valid(const Key& key) const;

	Option<T> remove(const Key& key);

private:
	struct Slot {
		u32 version;
		Option<T> value;
	};
	Vector<Slot> m_elements;
	Vector<u32> m_free_indices;
};

OP_CORE_NAMESPACE_END

#include "core/containers/slot_map.inl"

OP_NAMESPACE_BEGIN
using core::SlotMap;
OP_NAMESPACE_END