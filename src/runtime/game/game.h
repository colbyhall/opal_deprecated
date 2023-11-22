// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/core.h"

// Begin the game namespace
#define OP_GAME_NAMESPACE_BEGIN                                                                                        \
	OP_NAMESPACE_BEGIN                                                                                                 \
	namespace game {

// End the game namespace
#define OP_GAME_NAMESPACE_END                                                                                          \
	}                                                                                                                  \
	OP_NAMESPACE_END

#include "core/containers/map.h"
#include "core/containers/slot_map.h"
#include "core/containers/string.h"
#include "core/containers/unique.h"
#include "core/containers/vector.h"
#include "core/math/quaternion.h"
#include "core/math/vector3.h"

OP_GAME_NAMESPACE_BEGIN

class Entity {
public:
	explicit Entity() = default;

private:
};
using EntityId = SlotMap<Entity>::Key;

struct Component {
	virtual ~Component() = default;
};

class ComponentTypeId {
public:
	explicit ComponentTypeId(StringView name) {}

private:
};

class Storage;

class ComponentRTTI {
public:
	explicit ComponentRTTI() {}

private:
	String m_name;
	usize m_size;

	using CreateStorageFn = Unique<Storage> (*)(void);
	CreateStorageFn m_create_storage_fn;
};

class ComponentRTTIRegister {
public:
	explicit ComponentRTTIRegister() {}

private:
	Map<ComponentTypeId, ComponentRTTI> m_types;
};

#define DECLARE_COMPONENT(name)                                                                                        \
	static ComponentTypeId type_id() { return ComponentTypeId(#name); };

struct Transform : Component {
	DECLARE_COMPONENT(Transform);

	Vector3<f32> position = 0.f;
	Quaternion<f32> rotation;
	Vector3<f32> scale = 1.f;
};

struct Link : Component {
	EntityId parent;
	Vector<EntityId> children;
};

class Storage {
public:
	virtual ~Storage() = default;
};

template <typename T>
class VectorStorage : public Storage {
public:
	explicit VectorStorage() = default;

private:
	Vector<Option<T>> m_components;
	Vector<u32> m_free_indices;
};

class Archetype {
public:
	explicit Archetype() = default;

private:
	Vector<Unique<Storage>> m_storages;
};

class World {
public:
	explicit World() = default;

private:
	SlotMap<Entity> m_entities;
};

OP_GAME_NAMESPACE_END