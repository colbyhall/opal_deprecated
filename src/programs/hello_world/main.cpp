// Copyright Colby Hall. All Rights Reserved.

#include "core/os/time.h"
#include "game/query.h"
#include "game/world.h"
#include "gpu/d3d12/device.h"
#include "gui/application.h"
#include "gui/builder.h"

OP_NAMESPACE_BEGIN

int main() {
	auto device = gpu::D3D12Device::make();

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

	auto last_frame = Instant::now();
	auto time = 0.f;

	auto application = gui::Application(device);
	application.set_size({ 1280, 720 });
	application.run([&](auto& b) {
		auto start_this_frame = Instant::now();
		auto dt = start_this_frame.duration_since(last_frame).as_secs_f32();
		last_frame = start_this_frame;

		time += dt;

		auto& canvas = b.canvas();

		const Vector2<f32> min = { 360.f + core::cos(time * 0.3f) * 360.f,
								   128.f + core::abs(core::sin(time * 2.f)) * 16.f };
		const auto max = min + 128.f;

		canvas.push(gui::Rectangle(AABB2<f32>::from_min_max(min, max), LinearColor::green()));

		auto query = game::Query();
		query.read(game::Transform::type()).read(game::Link::type());

		auto accum = 0;
		query.execute(world, [&](auto& view) {
			auto& transform = view.read<game::Transform>();
			OP_UNUSED(transform);
			accum += 1;
		});
		OP_ASSERT(accum == 1024);
	});

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
