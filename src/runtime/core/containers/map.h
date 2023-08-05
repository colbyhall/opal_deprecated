// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/vector.h"
#include "core/hash.h"
#include "core/non_copyable.h"
#include "core/type_traits.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Hasher>
class Map;

template <typename Key, typename Value, typename Hasher>
class MapIterator {
public:
	GJ_ALWAYS_INLINE explicit MapIterator(Map<Key, Value, Hasher>& map) : m_map(map) {}

	GJ_ALWAYS_INLINE operator bool() const { return m_index < m_map.m_buckets.len(); }
	GJ_ALWAYS_INLINE MapIterator& operator++() {
		m_index += 1;
		return *this;
	}
	GJ_ALWAYS_INLINE const Key& key() const { return m_map.m_buckets[m_index].key; }
	GJ_ALWAYS_INLINE Value& value() const { return m_map.m_buckets[m_index].value; }

private:
	usize m_index = 0;
	Map<Key, Value, Hasher>& m_map;
};

template <typename Key, typename Value, typename Hasher>
class ConstMapIterator {
public:
	GJ_ALWAYS_INLINE explicit ConstMapIterator(const Map<Key, Value, Hasher>& map) : m_map(map) {}

	GJ_ALWAYS_INLINE operator bool() const { return m_index < m_map.m_buckets.len(); }
	GJ_ALWAYS_INLINE ConstMapIterator& operator++() {
		m_index += 1;
		return *this;
	}
	GJ_ALWAYS_INLINE const Key& key() const { return m_map.m_buckets[m_index].key; }
	GJ_ALWAYS_INLINE const Value& value() const { return m_map.m_buckets[m_index].value; }

private:
	usize m_index = 0;
	const Map<Key, Value, Hasher>& m_map;
};

template <typename Key, typename Value, typename Hasher = FNV1Hasher>
class Map {
	struct Bucket {
		Key key;
		Value value;
		i32 next;
	};
	static_assert(std::is_base_of_v<core::IHasher, Hasher>, "IHasher must be a valid IHasher");

public:
	constexpr Map() = default;

	using ConstIterator = ConstMapIterator<Key, Value, Hasher>;
	using Iterator = MapIterator<Key, Value, Hasher>;

	void reserve(usize amount);
	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize len() const { return m_buckets.len(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize cap() const { return m_buckets.cap(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_empty() const { return m_buckets.is_empty(); }

	void insert(const Key& key, Value&& value);
	void insert(const Key& key, const Value& value);

	Option<Value> remove(const Key& key);
	void retain(FunctionRef<bool(const Key&, const Value&)> keep);

	Option<Value const&> find(const Key& key) const;
	Option<Value&> find_mut(const Key& key);

	GJ_ALWAYS_INLINE ConstIterator iter() const;
	GJ_ALWAYS_INLINE Iterator iter_mut();

private:
	friend class MapIterator<Key, Value, Hasher>;
	friend class ConstMapIterator<Key, Value, Hasher>;

	GJ_ALWAYS_INLINE usize key_to_layout_index(const Key& key) const;
	void refresh_layout();

	Vector<Bucket> m_buckets;
	Vector<i32> m_layout;
};

GJ_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/map.inl"

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Map;
GJ_NAMESPACE_END