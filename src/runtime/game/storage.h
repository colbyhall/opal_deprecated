// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/unique.h"
#include "core/containers/vector.h"
#include "game/game.h"

OP_GAME_NAMESPACE_BEGIN

class AnonymousStorage {
public:
	virtual bool transfer_to(AnonymousStorage& other, u32 index) = 0;
	virtual ComponentType type() const = 0;
	virtual u32 len() const = 0;
	virtual ~AnonymousStorage() = default;
};

template <typename T>
class Storage : public AnonymousStorage {
public:
	explicit Storage() = default;

	virtual void store(T&& component, u32 index) = 0;
	virtual Option<T> remove(u32 index) = 0;

	// Storage
	ComponentType type() const override { return T::type(); }
	// ~Storage
};

template <typename T>
class VectorStorage : public Storage<T> {
public:
	explicit VectorStorage() = default;

	// AnonymousStorage
	bool transfer_to(AnonymousStorage& other, u32 index) override {
		auto& typed_storage = static_cast<Storage<T>&>(other);
		auto component_opt = remove(index);
		if (component_opt.is_set()) {
			auto component = component_opt.unwrap();
			typed_storage.store(op::move(component), index);
			return true;
		}
		return false;
	}
	u32 len() const override { return (u32)m_components.len(); }
	// ~AnonymousStorage

	// Storage
	void store(T&& component, u32 index) override {
		if (index == m_components.len()) {
			m_components.push(op::move(component));
		} else {
			m_components[index] = op::move(component);
		}
	}
	Option<T> remove(u32 index) override {
		if (index >= m_components.len()) {
			return nullopt;
		}
		return m_components[index].unwrap();
	}
	// ~Storage

private:
	Vector<Option<T>> m_components;
};

OP_GAME_NAMESPACE_END