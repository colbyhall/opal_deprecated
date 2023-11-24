// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/map.h"
#include "core/containers/shared.h"
#include "core/containers/string.h"
#include "core/containers/unique.h"
#include "game/entity.h"
#include "game/storage.h"

OP_GAME_NAMESPACE_BEGIN

struct Component {
	virtual ~Component() = default;
};

class AnonymousStorage;
class ComponentTypeInfo {
public:
	using CreateStorageFn = Unique<AnonymousStorage> (*)(void);

	explicit ComponentTypeInfo(StringView name, usize size, CreateStorageFn create_storage_fn)
		: m_name(name)
		, m_size(size)
		, m_create_storage_fn(create_storage_fn) {}

	struct Property {
		StringView name;
		usize offset;
		usize size;

		enum class Type { Bool, I8, U8, I16, U16, I32, U32, I64, U64, F32, F64, String };
		Type type;
	};
	void add_property(StringView name, usize offset, usize size, Property::Type type);

	OP_ALWAYS_INLINE StringView name() const { return m_name; }
	OP_ALWAYS_INLINE usize size() const { return m_size; }
	OP_ALWAYS_INLINE Slice<Property const> properties() const { return m_properties; }
	OP_ALWAYS_INLINE Unique<AnonymousStorage> create_storage() const { return m_create_storage_fn(); }

private:
	StringView m_name;
	usize m_size;

	Vector<Property> m_properties;
	CreateStorageFn m_create_storage_fn;
};

#define OP_GAME_REGISTER_COMPONENT(registry, component) (registry).register_component<component>(#component)

/**
 * Registers component types and their properties.
 */
class ComponentRegistry : public SharedFromThis<ComponentRegistry> {
public:
	static Shared<ComponentRegistry> make();

	template <typename Component>
	ComponentRegistry& register_component(StringView name) {
		auto create_storage_fn = []() -> Unique<AnonymousStorage> { return Unique<VectorStorage<Component>>::make(); };
		auto info = ComponentTypeInfo(name, sizeof(Component), create_storage_fn);
		Component::fill_type_info(info);
		m_types.insert(Component::type(), op::move(info));
		return *this;
	}

	ComponentTypeInfo const& find(ComponentType type) const;

private:
	explicit ComponentRegistry() = default;

	Map<ComponentType, ComponentTypeInfo> m_types;
};

#define OP_GAME_COMPONENT(component)                                                                                   \
	static ComponentType type() {                                                                                      \
		static auto result = ComponentType(#component);                                                                \
		return result;                                                                                                 \
	}                                                                                                                  \
	static void fill_type_info(ComponentTypeInfo& type_info)

#define OP_GAME_IMPLEMENT_COMPONENT(component) void component::fill_type_info(ComponentTypeInfo& type_info)

template <typename ComponentProperty>
OP_ALWAYS_INLINE void register_property(ComponentTypeInfo& info, StringView name, usize offset) {
	info.add_property(name, offset, sizeof(ComponentProperty), ComponentTypeInfo::Property::Type::F32);
}

#define OP_GAME_REGISTER_PROPERTY(component, property)                                                                 \
	op::game::register_property<decltype(component::property)>(type_info, #property, offsetof(component, property))

OP_GAME_NAMESPACE_END

#include "core/math/quaternion.h"
#include "core/math/vector3.h"

// Test Components
OP_GAME_NAMESPACE_BEGIN

struct Transform : public Component {
	OP_GAME_COMPONENT(Transform);
	Vector3<f32> position = 0.f;
	Quaternion<f32> rotation;
	Vector3<f32> scale = 1.f;
};

struct Link : public Component {
	OP_GAME_COMPONENT(Link);
	Option<EntityId> parent;
	Vector<EntityId> children;
};

OP_GAME_NAMESPACE_END