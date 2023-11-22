// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/slot_map.h"
#include "editor/application.h"
#include "gpu/d3d12/device.h"
#include "imgui/imgui.h"
#include "stb/stb_image.h"

OP_SUPPRESS_WARNING_PUSH
OP_SUPPRESS_WARNINGS

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
	OP_UNUSED(hInst);
	OP_UNUSED(hInstPrev);
	OP_UNUSED(cmdline);
	OP_UNUSED(cmdshow);

	using namespace op;

	SlotMap<int> slot_map;
	auto key = slot_map.insert(5);
	slot_map.remove(key);

	auto device = gpu::D3D12Device::create();

	editor::Application(device).run([]() {
		// Create a window called "My First Tool", with a menu bar.
		static bool a = true;
		ImGui::Begin("My First Tool", &a, ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */
				}
				if (ImGui::MenuItem("Close", "Ctrl+W")) {
					a = false;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// Generate samples and plot them
		float samples[100];
		for (int n = 0; n < 100; n++)
			samples[n] = sinf((float)n * 0.2f + (float)ImGui::GetTime() * 1.5f);
		ImGui::PlotLines("Samples", samples, 100);

		// Display contents in a scrolling region
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
		ImGui::BeginChild("Scrolling");
		for (int n = 0; n < 50; n++)
			ImGui::Text("%04d: Some text", n);
		ImGui::EndChild();
		ImGui::End();
	});

	return 0;
}

OP_SUPPRESS_WARNING_POP
