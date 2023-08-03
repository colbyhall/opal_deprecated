// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "core/containers/function.h"
#include "core/hash.h"
#include "core/non_copyable.h"
#include "core/type_traits.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Key, typename Value, typename Allocator, typename Hasher>
class Map;

template <typename Key, typename Value, typename Allocator, typename Hasher>
class MapIterator {
public:
	EU_ALWAYS_INLINE explicit MapIterator(
		Map<Key, Value, Allocator, Hasher>& map) :
		m_map(map) {}

	EU_ALWAYS_INLINE operator bool() const {
		return m_index < m_map.m_buckets.len();
	}
	EU_ALWAYS_INLINE MapIterator& operator++() {
		m_index += 1;
		return *this;
	}
	EU_ALWAYS_INLINE const Key& key() const {
		return m_map.m_buckets[m_index].key;
	}
	EU_ALWAYS_INLINE Value& value() const {
		return m_map.m_buckets[m_index].value;
	}

private:
	usize m_index = 0;
	Map<Key, Value, Allocator, Hasher>& m_map;
};

template <typename Key, typename Value, typename Allocator, typename Hasher>
class ConstMapIterator {
public:
	EU_ALWAYS_INLINE explicit ConstMapIterator(
		const Map<Key, Value, Allocator, Hasher>& map) :
		m_map(map) {}

	EU_ALWAYS_INLINE operator bool() const {
		return m_index < m_map.m_buckets.len();
	}
	EU_ALWAYS_INLINE ConstMapIterator& operator++() {
		m_index += 1;
		return *this;
	}
	EU_ALWAYS_INLINE const Key& key() const {
		return m_map.m_buckets[m_index].key;
	}
	EU_ALWAYS_INLINE const Value& value() const {
		return m_map.m_buckets[m_index].value;
	}

private:
	usize m_index = 0;
	const Map<Key, Value, Allocator, Hasher>& m_map;
};

template <typename Key, typename Value, typename Allocator = MallocAllocator,
		  typename Hasher = FNV1Hasher>
class Map {
	struct Bucket {
		Key key;
		Value value;
		i32 next;
	};
	static_assert(std::is_base_of_v<core::IHasher, Hasher>,
				  "IHasher must be a valid IHasher");

public:
	constexpr Map() = default;

	using ConstIterator = ConstMapIterator<Key, Value, Allocator, Hasher>;
	using Iterator = MapIterator<Key, Value, Allocator, Hasher>;

	void reserve(usize amount);
	EU_NO_DISCARD EU_ALWAYS_INLINE usize len() const { return m_buckets.len(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE usize cap() const { return m_buckets.cap(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_empty() const {
		return m_buckets.is_empty();
	}

	void insert(const Key& key, Value&& value);
	void insert(const Key& key, const Value& value);

	Option<Value> remove(const Key& key);
	void retain(FunctionRef<bool(const Key&, const Value&)> keep);

	Option<Value const&> find(const Key& key) const;
	Option<Value&> find_mut(const Key& key);

	EU_ALWAYS_INLINE ConstIterator iter() const;
	EU_ALWAYS_INLINE Iterator iter_mut();

private:
	friend class MapIterator<Key, Value, Allocator, Hasher>;
	friend class ConstMapIterator<Key, Value, Allocator, Hasher>;

	EU_ALWAYS_INLINE usize key_to_layout_index(const Key& key) const;
	void refresh_layout();

	Array<Bucket, Allocator> m_buckets;
	Array<i32, Allocator> m_layout;
};

EU_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/map.inl"

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Map;
EU_NAMESPACE_END