// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slot_map.h"
#include "game/game.h"

OP_GAME_NAMESPACE_BEGIN

class Entity {
public:
	explicit Entity() = default;

	OP_ALWAYS_INLINE void set_archetype_index(u32 index) { m_archetype_index = index; }
	OP_ALWAYS_INLINE Option<u32> archetype_index() const { return m_archetype_index; }

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

	Slice<ComponentType const> components() const { return m_components; }

private:
	Option<u32> m_archetype_index = nullopt;
	Vector<ComponentType> m_components;
};
using EntityId = SlotMap<Entity>::Key;

OP_GAME_NAMESPACE_END