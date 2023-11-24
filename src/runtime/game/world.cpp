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

bool World::remove_component(EntityId id, ComponentType component) {
	// Check if the entity exists.
	auto entity_opt = m_entities.get(id);
	if (!entity_opt) {
		return false;
	}

	// Check if the entity has the component.
	auto& entity = entity_opt.unwrap();
	if (!entity.contains(component)) {
		return false;
	}

	// Remove the component from the entity.
	entity.remove_component(component);

	// Find the archetype that supports the remaining components.
	auto new_archetype_index = find_or_create_archetype(entity.components());
	auto& new_archetype = m_archetypes[new_archetype_index];
	auto new_slot_index = new_archetype.next_slot_index();

	// Transfer all the components to the new archetype. The component we're trying to remove will be discarded in the
	// process.
	//
	// If this entity had a component it had to be stored somewhere, so this should be safe.
	auto old_component_storage = entity.component_storage().unwrap();
	auto& old_archetype = m_archetypes[old_component_storage.archetype_index];
	old_archetype.transfer_to(new_archetype, old_component_storage.slot_index, new_slot_index);

	// Update the entity's component storage.
	entity.set_component_storage(new_archetype_index, new_slot_index);

	return true;
}

u32 World::find_or_create_archetype(Slice<ComponentType const> supported_types) {
	// Find an archetype that supports all the components.
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

	// If no archetype was found, create a new one.
	if (!result.is_set()) {
		auto archetype = Archetype();

		// Add all the component storages required by the caller.
		for (auto type : supported_types) {
			auto& type_info = m_component_registry->find(type);
			archetype.push_storage(type_info.create_storage());
		}

		result = (u32)m_archetypes.len();
		m_archetypes.push(op::move(archetype));
	}

	OP_ASSERT(result.is_set(), "Failed to create an archetype somehow.");
	return result.unwrap();
}

OP_GAME_NAMESPACE_END