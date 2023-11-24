// Copyright Colby Hall. All Rights Reserved.

#include "game/archetype.h"

OP_GAME_NAMESPACE_BEGIN

bool Archetype::supports(ComponentType type) const {
	for (auto& storage : m_storages) {
		if (storage->type() == type) {
			return true;
		}
	}

	return false;
}

bool Archetype::is_slot_used(op::core::u32 index) const { return m_storages[0]->is_slot_used(index); }

Storage& Archetype::find_storage(ComponentType component) {
	Option<Storage&> result = nullopt;
	for (auto& storage : m_storages) {
		if (storage->type() == component) {
			result = *storage;
			break;
		}
	}
	return result.unwrap();
}

Storage const& Archetype::find_storage(ComponentType component) const {
	return const_cast<Archetype*>(this)->find_storage(component);
}

u32 Archetype::next_slot_index() {
	// If there are no free slots, return the number of slots in the archetype
	if (m_free_slot_indices.is_empty()) {
		return m_storages[0]->total_slots();
	}

	return m_free_slot_indices.pop().unwrap();
}

void Archetype::transfer_to(Archetype& other, u32 from, u32 to) {
	m_free_slot_indices.push(from);

	for (auto& my_storage : m_storages) {
		bool found = false;
		for (auto& other_storage : other.m_storages) {
			if (my_storage->type() == other_storage->type()) {
				my_storage->transfer_to(*other_storage, from, to);
				found = true;
			}
		}
		if (!found) {
			my_storage->discard(from);
		}
	}
}

OP_GAME_NAMESPACE_END