// Copyright Colby Hall. All Rights Reserved.

#include "game/query.h"
#include "game/world.h"

#include "core/os/time.h"
#include "core/os/windows.h"

OP_NAMESPACE_BEGIN

int main() {
	auto component_registry = game::ComponentRegistry::make();
	{
		auto& registry = *component_registry;
		OP_GAME_REGISTER_COMPONENT(registry, game::Transform);
		OP_GAME_REGISTER_COMPONENT(registry, game::Link);
	}

	auto world = game::World(component_registry);

	for (auto index = 0; index < 1024; index += 1) {
		world.spawn().add(game::Transform{}).add(game::Link{});
	}

	auto query = game::Query();
	query.read(game::Transform::type()).read(game::Link::type());

	auto accum = 0;
	query.execute(world, [&](auto& view) {
		auto& transform = view.read<game::Transform>();
		OP_UNUSED(transform);
		accum += 1;
	});
	OP_ASSERT(accum == 1024);

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
