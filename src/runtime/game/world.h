// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "game/archetype.h"
#include "game/component.h"
#include "game/entity.h"

OP_GAME_NAMESPACE_BEGIN

class World;

class EntityRefMut {
public:
	explicit EntityRefMut(EntityId id, World& world) : m_id(id), m_world(world) {}

	template <typename T>
	OP_ALWAYS_INLINE EntityRefMut& add(T&& component);

	OP_ALWAYS_INLINE EntityRefMut& remove(ComponentType component);

	OP_NO_DISCARD OP_ALWAYS_INLINE EntityId id() const { return m_id; }

private:
	EntityId m_id;
	World& m_world;
};

class EntityRef {
public:
	explicit EntityRef(EntityId id, World const& world) : m_id(id), m_world(world) {}

	OP_NO_DISCARD OP_ALWAYS_INLINE EntityId id() const { return m_id; }

private:
	EntityId m_id;
	World const& m_world;
};

class World {
public:
	explicit World(const ComponentRegistry& component_registry)
		: m_component_registry(component_registry.to_shared()) {}

	OP_NO_DISCARD EntityRefMut spawn();
	OP_NO_DISCARD Option<EntityRef> get(EntityId id) const;
	OP_NO_DISCARD Option<EntityRefMut> get(EntityId id);

private:
	friend class EntityRefMut;
	friend class EntityRef;

	template <typename T>
	bool add_component(EntityId id, T&& component);
	bool remove_component(EntityId id, ComponentType component);

	u32 find_or_create_archetype(Slice<ComponentType const> supported_types);

	SlotMap<Entity> m_entities;
	Vector<Archetype> m_archetypes;
	Shared<ComponentRegistry const> m_component_registry;
};

template <typename T>
bool World::add_component(EntityId id, T&& component) {
	// Find the entity data.
	auto entity_opt = m_entities.get(id);
	if (!entity_opt) {
		return false;
	}

	// Check if the entity already has the component.
	auto& entity = entity_opt.unwrap();
	if (entity.contains(T::type())) {
		return false;
	}

	// Update the entity state and then find an archetype that supports the new component requirements.
	entity.add_component(T::type());
	auto new_archetype_index = find_or_create_archetype(entity.components());
	auto& new_archetype = m_archetypes[new_archetype_index];
	auto new_slot_index = new_archetype.pop_free_index();

	// Store the old storage state before we change it as we need it for the transfer.
	auto old_component_storage_opt = entity.component_storage();

	// Update the entity state to reflect the new component storage and then store the component.
	entity.set_component_storage(new_archetype_index, new_slot_index);
	new_archetype.store(op::forward<T>(component), new_slot_index);

	// Transfer the old component storage to the new archetype.
	if (old_component_storage_opt.is_set()) {
		auto old_component_storage = old_component_storage_opt.unwrap();
		auto& old_archetype = m_archetypes[old_component_storage.archetype_index];
		old_archetype.transfer_to(new_archetype, old_component_storage.slot_index, new_slot_index);
	}

	return true;
}

template <typename T>
EntityRefMut& EntityRefMut::add(T&& component) {
	m_world.add_component(m_id, std::forward<T>(component));
	return *this;
}

EntityRefMut& EntityRefMut::remove(ComponentType component) {
	m_world.remove_component(m_id, component);
	return *this;
}

OP_GAME_NAMESPACE_END