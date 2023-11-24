// Copyright Colby Hall. All Rights Reserved.

#include "game/world.h"

OP_GAME_NAMESPACE_BEGIN

EntityRefMut World::spawn() {
	auto id = m_entities.insert(Entity{});
	return EntityRefMut(id, *this);
}

Option<EntityRef> World::get(EntityId id) const {
	if (m_entities.contains(id)) {
		return EntityRef(id, *this);
	}
	return nullopt;
}

Option<EntityRefMut> World::get(EntityId id) {
	if (m_entities.contains(id)) {
		return EntityRefMut(id, *this);
	}
	return nullopt;
}

u32 World::find_or_create_archetype(Slice<ComponentType const> supported_types) {
	Option<u32> result = nullopt;
	for (u32 index = 0; index < m_archetypes.len(); ++index) {
		bool supports_all = true;
		for (auto type : supported_types) {
			if (!m_archetypes[index].supports(type)) {
				supports_all = false;
				break;
			}
		}
		if (supports_all) {
			result = index;
			break;
		}
	}

	if (!result.is_set()) {
		auto archetype = Archetype{};
		for (auto type : supported_types) {
			auto& type_info = m_component_registry->find(type);
			archetype.add_storage(type_info.create_storage());
		}

		result = (u32)m_archetypes.len();
		m_archetypes.push(op::move(archetype));
	}

	OP_ASSERT(result.is_set(), "Failed to create an archetype somehow.");
	return result.unwrap();
}

OP_GAME_NAMESPACE_END