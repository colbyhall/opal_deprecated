// Copyright Colby Hall. All Rights Reserved.

#include "editor/application.h"
#include "core/containers/string_view.h"
#include "core/containers/wstring.h"
#include "core/math/matrix4.h"
#include "core/math/vector2.h"
#include "core/os/library.h"
#include "core/os/windows.h"
#undef interface

#include "dxc/dxc.h"
#include "gpu/buffer.h"
#include "gpu/context.h"
#include "gpu/graphics_command_list.h"
#include "gpu/graphics_pipeline.h"
#include "gpu/swapchain.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

#ifdef GJ_PLATFORM_WINDOWS

static const wchar_t* window_class_name = L"gj_editor_window";

typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE,
	PROCESS_SYSTEM_DPI_AWARE,
	PROCESS_PER_MONITOR_DPI_AWARE
} PROCESS_DPI_AWARENESS;

enum MONITOR_DPI_TYPE {
	MDT_EFFECTIVE_DPI,
	MDT_ANGULAR_DPI,
	MDT_RAW_DPI,
	MDT_DEFAULT
};

typedef HRESULT (*SetProcessDPIAwareness)(PROCESS_DPI_AWARENESS value);
typedef HRESULT (*GetDPIForMonitor
)(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType, UINT* dpiX, UINT* dpiY);
#endif // #ifdef GJ_PLATFORM_WINDOWS

GJ_EDITOR_NAMESPACE_BEGIN

static const char* source = R"#(
cbuffer bufs : register(b0) {
	float4x4 local_to_projection;
}

Texture2D texture2d_table[] : register(t0);
SamplerState sampler_table : register(s0);

float4 uint_to_float4(uint color){
	if (color==0xffffffff) return float4(1, 1, 1, 1);

	float a = ((color & 0xff000000) >>24);
	float r = ((color & 0xff0000) >> 16);
	float g = ((color & 0xff00) >> 8);
	float b = ((color & 0xff));
	return float4(r, g, b, a) / 255.0;
}

struct PSInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float4 scissor : SCISSOR;
	float2 uv : UV;
	float2 screen_pos : SCREEN_POS;
	uint tex2d : TEX;
};

struct VSInput {
	float4 scissor : SCISSOR;
	float2 position : POSITION;
	float2 uv : UV;
	uint color : COLOR;
	uint tex2d : TEX;
};

PSInput vs_main(VSInput input) {
	PSInput result;

	result.position = mul(local_to_projection, float4(input.position.x, input.position.y, 0.0, 1.0));
	result.color = uint_to_float4(input.color);
	result.scissor = input.scissor;
	result.uv = input.uv;
	result.screen_pos = input.position;
	result.tex2d = input.tex2d;

	return result;
}

float4 ps_main(PSInput input) : SV_TARGET {
	bool in_scissor =
		input.screen_pos.x >= input.scissor.x &&
		input.screen_pos.y >= input.scissor.y &&
		input.screen_pos.x <= input.scissor.z &&
		input.screen_pos.y <= input.scissor.w;

	float4 output = input.color;
	if (true) {
		if (input.tex2d == 0) {
			return input.color;
		}
		float alpha = texture2d_table[input.tex2d].Sample(sampler_table, input.uv, 0).x;
		output.w = alpha;
	}
	return output;
}
)#";

class Window {
public:
	using Handle = void*;

	static Window spawn(StringView title, const Vector2<u32>& size) {
		const auto dpi = (f32)::GetDpiForSystem() / 96.f;
		HINSTANCE hInstance = ::GetModuleHandleA(nullptr);

		const DWORD dwStyle = WS_OVERLAPPEDWINDOW;

		RECT adjusted_rect = { .left = 0,
							   .top = 0,
							   .right = (LONG)((f32)size.width * dpi),
							   .bottom = (LONG)((f32)size.height * dpi) };
		::AdjustWindowRect(&adjusted_rect, dwStyle, 0);

		const int width = adjusted_rect.right - adjusted_rect.left;
		const int height = adjusted_rect.bottom - adjusted_rect.top;

		const int monitor_width = ::GetSystemMetrics(SM_CXSCREEN);
		const int monitor_height = ::GetSystemMetrics(SM_CYSCREEN);

		const int x = monitor_width / 2 - width / 2;
		const int y = monitor_height / 2 - height / 2;

		WString w_title;
		w_title.push(title);

		HWND handle = ::CreateWindowExW(
			0,
			window_class_name,
			w_title.ptr(),
			dwStyle,
			x,
			y,
			width,
			height,
			nullptr,
			nullptr,
			hInstance,
			nullptr
		);
		GJ_ASSERT(handle != nullptr);

		auto swapchain = gpu::Swapchain::make(handle);

		::SetWindowLongPtrA(handle, GWLP_USERDATA, (LONG_PTR)&swapchain.cast());

		::ShowWindow(handle, SW_SHOWDEFAULT);

		return Window{ handle, gj::move(swapchain) };
	}

	GJ_ALWAYS_INLINE Handle handle() const { return m_handle; }
	GJ_ALWAYS_INLINE gpu::Swapchain& swapchain() { return m_swapchain; }
	GJ_ALWAYS_INLINE const gpu::Swapchain& swapchain() const {
		return m_swapchain;
	}

private:
	GJ_ALWAYS_INLINE explicit Window(Handle handle, gpu::Swapchain&& swapchain)
		: m_handle(handle)
		, m_swapchain(gj::move(swapchain)) {}

	Handle m_handle;
	gpu::Swapchain m_swapchain;
};

static LRESULT CALLBACK
window_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam)) return true;

	gpu::ISwapchain* const swapchain =
		(gpu::ISwapchain*)::GetWindowLongPtrA(hWnd, GWLP_USERDATA);

	switch (Msg) {
	case WM_SIZE: {
		swapchain->resize();
		return 0;
	}
	}

	return ::DefWindowProcW(hWnd, Msg, wParam, lParam);
}

Application::Application(int argc, char** argv) {
	GJ_UNUSED(argc);
	GJ_UNUSED(argv);
}

void Application::run(FunctionRef<void()> f) {
	gpu::init();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

#ifdef GJ_PLATFORM_WINDOWS
	static auto shcore = core::Library::open("shcore.dll");
	if (shcore) {
		auto& actual = shcore.as_mut().unwrap();
		auto SetProcessDpiAwareness =
			actual.find<SetProcessDPIAwareness>("SetProcessDpiAwareness");
		if (SetProcessDpiAwareness)
			SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
	}

	HINSTANCE hInstance = ::GetModuleHandleA(nullptr);

	WNDCLASSEXW window_class = {};
	window_class.cbSize = sizeof(WNDCLASSEXW);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = &window_proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = hInstance;
	window_class.hIcon = ::LoadIconW(hInstance, nullptr);
	window_class.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = window_class_name;
	window_class.hIconSm = ::LoadIconW(hInstance, nullptr);

	const ATOM atom = RegisterClassExW(&window_class);
	GJ_ASSERT(atom != 0);
#endif // #ifdef GJ_PLATFORM_WINDOWS

	auto window = Window::spawn("test", { 1280, 720 });
	ImGui_ImplWin32_Init(window.handle());

	// Compile the vertex shader using source
	dxc::Input vertex_input = { source, "vs_main", dxc::ShaderType::Vertex };
	auto vertex_output = dxc::compile(vertex_input).unwrap();
	auto vertex_shader = gpu::VertexShader::make(
		gj::move(vertex_output.binary),
		gj::move(vertex_output.input_parameters)
	);

	// Compile the pixel shader using source
	dxc::Input pixel_input = { source, "ps_main", dxc::ShaderType::Pixel };
	auto pixel_output = dxc::compile(pixel_input).unwrap();
	auto pixel_shader = gpu::PixelShader::make(gj::move(pixel_output.binary));

	// Create the graphics pipeline
	gpu::GraphicsPipeline::Definition definition = {
		.vertex_shader = gj::move(vertex_shader),
		.pixel_shader = gj::move(pixel_shader),
		.blend_enabled = true,
		.src_color_blend_factor = gpu::BlendFactor::SrcAlpha,
		.dst_color_blend_factor = gpu::BlendFactor::OneMinusSrcAlpha,
	};
	definition.color_attachments.push(gpu::Format::RGBA_U8);
	auto pipeline = gpu::GraphicsPipeline::make(gj::move(definition));

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

	auto pixels_buffer = gpu::Buffer::make(
		gpu::Buffer::Usage::TransferSrc,
		gpu::Buffer::Kind::Upload,
		width * height
	);
	pixels_buffer.map([&](auto slice) {
		core::copy(slice.begin(), pixels, slice.len());
	});

	auto pixels_texture = gpu::Texture::make(
		gpu::Texture::Usage::TransferDst | gpu::Texture::Usage::Sampled,
		gpu::Format::R_U8,
		{ (u32)width, (u32)height, 1 }
	);

	io.Fonts->SetTexID((void*)(usize)pixels_texture.bindless());

	auto upload_font_atlas = gpu::GraphicsCommandList::record([&](auto& gcr) {
		gcr.texture_barrier(
			pixels_texture,
			gpu::Layout::General,
			gpu::Layout::TransferDst
		);
		gcr.copy_buffer_to_texture(pixels_texture, pixels_buffer);
		gcr.texture_barrier(
			pixels_texture,
			gpu::Layout::TransferDst,
			gpu::Layout::General
		);
	});
	gpu::Context::the().submit(upload_font_atlas);

	for (;;) {
#ifdef GJ_PLATFORM_WINDOWS
		MSG msg;
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif // #ifdef GJ_PLATFORM_WINDOWS

		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		f();
		ImGui::Render();

		struct Vertex {
			ImVec4 scissor;
			ImVec2 position;
			ImVec2 uv;
			u32 color;
			u32 tex2d;
		};

		auto* draw_data = ImGui::GetDrawData();
		GJ_UNUSED(draw_data);

		if (draw_data->Valid && draw_data->TotalVtxCount > 0) {
			Vector<gpu::GraphicsCommandList> command_lists;
			command_lists.reserve(draw_data->CmdListsCount);

			for (ImDrawList* draw_list : draw_data->CmdLists) {
				auto vertices = Vector<Vertex>();
				vertices.reserve(draw_data->TotalVtxCount);

				const auto bounds = draw_data->DisplaySize;

				for (const ImDrawCmd& draw_cmd : draw_list->CmdBuffer) {
					for (u32 i = 0; i < draw_cmd.ElemCount; ++i) {
						const auto idx =
							draw_list->IdxBuffer[draw_cmd.IdxOffset + i];
						auto& vtx = draw_list->VtxBuffer[idx];
						vertices.push(Vertex{
							draw_cmd.ClipRect,
							{ vtx.pos.x, bounds.y - vtx.pos.y },
							vtx.uv,
							vtx.col,
							(u32) reinterpret_cast<usize>(draw_cmd.GetTexID()) }
						);
					}
				}

				auto vertex_buffer = gpu::Buffer::make(
					gpu::Buffer::Usage::Vertex,
					gpu::Buffer::Kind::Upload,
					vertices.len() * sizeof(Vertex)
				);
				vertex_buffer.map([&](auto slice) {
					core::copy(slice.begin(), vertices.cbegin(), slice.len());
				});

				const auto projection = Matrix4<f32>::orthographic(
					bounds.x,
					bounds.y,
					0.1f,
					1000.f
				);

				const auto view = Matrix4<f32>::transform(
					{ -bounds.x / 2, -bounds.y / 2, 0 },
					Quaternion<f32>::identity,
					1
				);

				const auto local_to_projection = projection * view;

				auto& backbuffer = window.swapchain().backbuffer();
				auto command_list =
					gpu::GraphicsCommandList::record([&](auto& gcr) {
						gcr.texture_barrier(
							   backbuffer,
							   gpu::Layout::Present,
							   gpu::Layout::ColorAttachment
						)
							.render_pass(
								backbuffer,
								nullptr,
								[&](auto& rpr) {
									rpr.push_constants(&local_to_projection)
										.set_pipeline(pipeline)
										.clear_color(0)
										.set_vertices(
											vertex_buffer,
											sizeof(Vertex)
										)
										// .set_indices(index_buffer)
										.draw(vertices.len(), 0);
								}
							)
							.texture_barrier(
								backbuffer,
								gpu::Layout::ColorAttachment,
								gpu::Layout::Present
							);
					});
				command_lists.push(gj::move(command_list));
			}

			gpu::Context::the().submit(command_lists[0]);
			window.swapchain().present();
		}
	}

	// ImGui_ImplWin32_Shutdown();
	// ImGui::DestroyContext();
}

GJ_EDITOR_NAMESPACE_END
