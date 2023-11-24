// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/unique.h"
#include "core/containers/vector.h"
#include "game/game.h"

OP_GAME_NAMESPACE_BEGIN

class Storage {
public:
	virtual bool transfer_to(Storage& other, u32 from, u32 to) = 0;
	virtual bool discard(u32 index) = 0;
	virtual ComponentType type() const = 0;
	virtual u32 total_slots() const = 0;
	virtual bool is_slot_used(u32 index) const = 0;
	virtual ~Storage() = default;
};

template <typename T>
class TypedStorage : public Storage {
public:
	explicit TypedStorage() = default;

	virtual Option<T&> write(u32 index) = 0;
	virtual Option<T const&> read(u32 index) const = 0;

	virtual void store(T&& component, u32 index) = 0;
	virtual Option<T> remove(u32 index) = 0;

	// Storage
	ComponentType type() const override { return T::type(); }
	// ~Storage
};

template <typename T>
class VectorStorage : public TypedStorage<T> {
public:
	explicit VectorStorage() = default;

	// Storage
	bool transfer_to(Storage& other, u32 from, u32 to) override {
		auto& typed_storage = static_cast<TypedStorage<T>&>(other);
		auto component_opt = remove(from);
		if (component_opt.is_set()) {
			auto component = component_opt.unwrap();
			typed_storage.store(op::move(component), to);
			return true;
		}
		return false;
	}
	u32 total_slots() const override { return (u32)m_components.len(); }
	bool discard(u32 index) override {
		if (index >= m_components.len()) {
			return false;
		}
		auto discarded = m_components[index].unwrap();
		OP_UNUSED(discarded);

		return true;
	}
	bool is_slot_used(u32 index) const override {
		if (index >= m_components.len()) {
			return false;
		}
		return m_components[index].is_set();
	}
	// ~Storage

	// TypedStorage
	Option<T&> write(u32 index) override {
		auto& component = m_components[index];
		if (component.is_set()) {
			return component.as_mut();
		}
		return nullopt;
	}
	Option<T const&> read(u32 index) const override {
		auto& component = m_components[index];
		if (component.is_set()) {
			return component.as_ref();
		}
		return nullopt;
	}
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
	// ~TypedStorage

private:
	Vector<Option<T>> m_components;
};

OP_GAME_NAMESPACE_END