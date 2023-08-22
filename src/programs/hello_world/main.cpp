// Copyright Colby Hall. All Rights Reserved.

#include "editor/application.h"
#include "imgui/imgui.h"

SF_SUPPRESS_WARNING_PUSH
SF_SUPPRESS_WARNINGS

int main(int argc, char** argv) {
	sf::editor::Application(argc, argv).run([]() {
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

SF_SUPPRESS_WARNING_POP
