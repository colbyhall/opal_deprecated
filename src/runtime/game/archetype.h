// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "game/storage.h"

OP_GAME_NAMESPACE_BEGIN

class Archetype {
public:
	explicit Archetype() = default;

	bool supports(ComponentType type) const;

	void add_storage(Unique<Storage>&& storage) { m_storages.push(op::move(storage)); }

	OP_NO_DISCARD OP_ALWAYS_INLINE u32 pop_free_index() {
		if (m_free_indices.is_empty()) {
			return m_storages[0]->len();
		}
		return m_free_indices.pop().unwrap();
	}

	template <typename T>
	void store(T&& component, u32 index) {
		for (auto& storage : m_storages) {
			if (storage->type() == T::type()) {
				auto& typed_storage = static_cast<TypedStorage<T>&>(*storage);
				typed_storage.store(op::forward<T>(component), index);
				return;
			}
		}
	}

	void transfer_to(Archetype& other, u32 from, u32 to);

private:
	Vector<Unique<Storage>> m_storages;
	Vector<u32> m_free_indices;
};

OP_GAME_NAMESPACE_END