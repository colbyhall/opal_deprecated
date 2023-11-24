// Copyright Colby Hall. All Rights Reserved.

#include "game/component.h"

OP_GAME_NAMESPACE_BEGIN

void ComponentTypeInfo::add_property(StringView name, usize offset, usize size, Property::Type type) {
	Property property;
	property.name = name;
	property.offset = offset;
	property.size = size;
	property.type = type;
	m_properties.push(op::move(property));
}

Shared<ComponentRegistry> ComponentRegistry::make() {
	ComponentRegistry result;
	return Shared<ComponentRegistry>::make(op::move(result));
}

ComponentTypeInfo const& ComponentRegistry::find(ComponentType type) const {
	auto result = m_types.find(type);
	OP_ASSERT(result.is_set(), "Component type not found. Register the component type before using it.");
	return result.unwrap();
}

OP_GAME_IMPLEMENT_COMPONENT(Transform) {
	OP_GAME_REGISTER_PROPERTY(Transform, position);
	OP_GAME_REGISTER_PROPERTY(Transform, rotation);
	OP_GAME_REGISTER_PROPERTY(Transform, scale);
}

OP_GAME_IMPLEMENT_COMPONENT(Link) {
	OP_GAME_REGISTER_PROPERTY(Link, parent);
	OP_GAME_REGISTER_PROPERTY(Link, children);
}

OP_GAME_NAMESPACE_END