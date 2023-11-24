// Copyright Colby Hall. All Rights Reserved.

#include "game/game.h"
#include "core/hash.h"

OP_GAME_NAMESPACE_BEGIN

ComponentType::ComponentType(StringView name) {
	core::FNV1Hasher hasher;
	hasher.write(Slice<u8 const>(reinterpret_cast<const u8*>(*name), name.len()));
	m_value = hasher.finish();
}

OP_GAME_NAMESPACE_END
