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

	auto last_frame_start = Instant::now();
	auto time = 0.f;
	auto accum = 0.f;
	auto frame_count = 0;

	auto application = gui::Application(device);
	application.set_size({ 1280, 720 });
	application.run([&](auto& b) {
		auto frame_start = Instant::now();
		auto dt = frame_start.duration_since(last_frame_start).as_secs_f32();
		last_frame_start = frame_start;
		time += dt;
		accum += dt;
		if (accum >= 1.f) {
			accum -= 1.f;

			char buffer[1024];
			snprintf(buffer, 1024, "FPS: %d\n", frame_count);
			OutputDebugStringA(buffer);

			frame_count = 0;
		}

		frame_count += 1;

		const auto size = core::cos(time) * 10.f + 20.f;
		b.canvas().push(gui::Text(
			AABB2<f32>::from_min_max(0.f, { 1280, 720 }),
			String::from("Hello, World!\nWhat is up."),
			size,
			LinearColor::green()
		));
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
