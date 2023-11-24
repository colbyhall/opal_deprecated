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

	template <typename T>
	bool add_component(EntityId id, T&& component) {
		auto entity_opt = m_entities.get(id);
		if (!entity_opt) {
			return false;
		}
		auto& entity = entity_opt.unwrap();
		if (entity.contains(T::type())) {
			return false;
		}
		entity.add_component(T::type());
		auto new_archetype_index = find_or_create_archetype(entity.components());
		auto old_archetype_index_opt = entity.archetype_index();
		entity.set_archetype_index(new_archetype_index);

		auto& new_archetype = m_archetypes[new_archetype_index];
		auto free_index = new_archetype.pop_free_index();
		new_archetype.store(op::forward<T>(component), free_index);

		if (old_archetype_index_opt.is_set()) {
			auto old_archetype_index = old_archetype_index_opt.unwrap();
			auto& old_archetype = m_archetypes[old_archetype_index];
			old_archetype.transfer_to(new_archetype, free_index);
		}

		return true;
	}

private:
	u32 find_or_create_archetype(Slice<ComponentType const> supported_types);

	SlotMap<Entity> m_entities;
	Vector<Archetype> m_archetypes;
	Shared<ComponentRegistry const> m_component_registry;
};

template <typename T>
EntityRefMut& EntityRefMut::add(T&& component) {
	m_world.add_component(m_id, std::forward<T>(component));
	return *this;
}

OP_GAME_NAMESPACE_END