// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "game/storage.h"

OP_GAME_NAMESPACE_BEGIN

class Archetype {
public:
	explicit Archetype() = default;

	bool supports(ComponentType type) const;
	OP_ALWAYS_INLINE u32 available_slots() const { return static_cast<u32>(m_free_slot_indices.len()); }
	OP_ALWAYS_INLINE u32 total_slots() const { return static_cast<u32>(m_storages[0]->total_slots()); }
	OP_ALWAYS_INLINE u32 count() const { return total_slots() - available_slots(); }
	OP_NO_DISCARD Storage& find_storage(ComponentType component);
	OP_NO_DISCARD Storage const& find_storage(ComponentType component) const;

	OP_NO_DISCARD bool is_slot_used(u32 index) const;

	template <typename T>
	OP_NO_DISCARD Option<T&> write(u32 index) {
		auto& storage = find_storage(T::type());
		auto& typed_storage = static_cast<TypedStorage<T>&>(storage);
		return typed_storage.write(index);
	}

	template <typename T>
	OP_NO_DISCARD Option<T const&> read(u32 index) {
		auto& storage = find_storage(T::type());
		auto& typed_storage = static_cast<TypedStorage<T> const&>(storage);
		return typed_storage.read(index);
	}

	template <typename T>
	void store(T&& component, u32 index) {
		auto& storage = find_storage(T::type());
		auto& typed_storage = static_cast<TypedStorage<T>&>(storage);
		typed_storage.store(op::forward<T>(component), index);
	}
	void transfer_to(Archetype& other, u32 from, u32 to);

	OP_ALWAYS_INLINE void push_storage(Unique<Storage>&& storage) { m_storages.push(op::move(storage)); }
	OP_NO_DISCARD u32 next_slot_index();

private:
	Vector<Unique<Storage>> m_storages;
	Vector<u32> m_free_slot_indices;
};

OP_GAME_NAMESPACE_END