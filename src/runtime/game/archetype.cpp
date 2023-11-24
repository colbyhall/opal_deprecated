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

void Archetype::transfer_to(Archetype& other, u32 from, u32 to) {
	m_free_indices.push(from);

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