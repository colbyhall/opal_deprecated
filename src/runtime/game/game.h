// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"
#include "core/hash.h"

// Begin the game namespace
#define OP_GAME_NAMESPACE_BEGIN                                                                                        \
	OP_NAMESPACE_BEGIN                                                                                                 \
	namespace game {

// End the game namespace
#define OP_GAME_NAMESPACE_END                                                                                          \
	}                                                                                                                  \
	OP_NAMESPACE_END

OP_GAME_NAMESPACE_BEGIN

class ComponentType {
public:
	explicit ComponentType(StringView name);

	OP_ALWAYS_INLINE u64 value() const { return m_value; }

	OP_ALWAYS_INLINE bool operator==(const ComponentType& other) const { return m_value == other.m_value; }
	OP_ALWAYS_INLINE bool operator!=(const ComponentType& other) const { return m_value != other.m_value; }

private:
	u64 m_value = 0;
};

OP_GAME_NAMESPACE_END

OP_NAMESPACE_BEGIN
// Implement op::hash for game::ComponentType
template <typename H>
struct Hash<H, game::ComponentType> {
	void operator()(H& hasher, const game::ComponentType& value) {
		Hash<H, u64> hash;
		hash(hasher, value.value());
	}
};
OP_NAMESPACE_END