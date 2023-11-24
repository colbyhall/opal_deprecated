// Copyright Colby Hall. All Rights Reserved.

#include "game/component.h"
#include "game/world.h"

#include "core/os/windows.h"

OP_NAMESPACE_BEGIN

int main() {
	auto component_registry = game::ComponentRegistry::make();
	OP_GAME_REGISTER_COMPONENT(*component_registry, game::Transform);
	OP_GAME_REGISTER_COMPONENT(*component_registry, game::Link);

	auto world = game::World{ component_registry };
	auto id = world.spawn().add(game::Transform{}).add(game::Link{}).id();

	return 0;
}

OP_NAMESPACE_END

OP_SUPPRESS_WARNING_PUSH
OP_SUPPRESS_WARNINGS

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	OP_UNUSED(hInst);
	OP_UNUSED(hInstPrev);
	OP_UNUSED(cmdline);
	OP_UNUSED(cmdshow);
	return op::main();
}

OP_SUPPRESS_WARNING_POP
