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

void Archetype::transfer_to(Archetype& other, u32 index) {
	for (auto& my_storage : m_storages) {
		for (auto& other_storage : other.m_storages) {
			if (my_storage->type() == other_storage->type()) {
				my_storage->transfer_to(*other_storage, index);
			}
		}
	}
	m_free_indices.push(index);
}

OP_GAME_NAMESPACE_END