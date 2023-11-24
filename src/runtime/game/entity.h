// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slot_map.h"
#include "game/game.h"

OP_GAME_NAMESPACE_BEGIN

class Entity {
public:
	explicit Entity() = default;

	struct ComponentStorage {
		u32 archetype_index;
		u32 slot_index;
	};
	OP_ALWAYS_INLINE void set_component_storage(u32 archetype_index, u32 slot_index) {
		m_component_storage = ComponentStorage{ archetype_index, slot_index };
	}
	OP_ALWAYS_INLINE Option<ComponentStorage> component_storage() const { return m_component_storage; }

	OP_ALWAYS_INLINE void add_component(ComponentType type) { m_components.push(type); }
	OP_ALWAYS_INLINE bool remove_component(ComponentType type) {
		for (usize index = 0; index < m_components.len(); ++index) {
			if (m_components[index] == type) {
				m_components.remove(index);
				return true;
			}
		}
		return false;
	}
	OP_ALWAYS_INLINE bool contains(ComponentType type) const {
		for (auto component : m_components) {
			if (component == type) {
				return true;
			}
		}
		return false;
	}

	OP_ALWAYS_INLINE Slice<ComponentType const> components() const { return m_components; }

private:
	Option<ComponentStorage> m_component_storage;
	Vector<ComponentType> m_components;
};
using EntityId = SlotMap<Entity>::Key;

OP_GAME_NAMESPACE_END