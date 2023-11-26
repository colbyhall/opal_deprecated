// Copyright Colby Hall. All Rights Reserved.

#include "gui/application.h"
#include "gui/builder.h"

#include "core/containers/shared.h"
#include "core/containers/string_view.h"
#include "core/containers/wstring.h"
#include "core/math/matrix4.h"

#include "dxc/dxc.h"
#include "gpu/buffer.h"
#include "gpu/graphics_command_list.h"
#include "gpu/graphics_pipeline.h"
#include "gpu/swapchain.h"
#include "gpu/texture.h"

#if OP_PLATFORM_WINDOWS
	#include "core/os/windows.h"
#endif

OP_GUI_NAMESPACE_BEGIN

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class OSWindow : public SharedFromThis<OSWindow> {
public:
	explicit OSWindow(HWND handle, Unique<gpu::Swapchain>&& swapchain, const Vector2<u32>& size)
		: m_handle(handle)
		, m_swapchain(op::move(swapchain))
		, m_size(size) {}

	/**
	 * Creates a new window.
	 * @param device The gpu device used to create a swapchain.
	 * @param title The title of the window.
	 * @param size The size of the window.
	 * @return The created window.
	 */
	static Shared<OSWindow> spawn(gpu::Device& device, StringView title, const Vector2<u32>& size) {
		DWORD dwStyle = WS_OVERLAPPEDWINDOW;

		WNDCLASSEXW window_class = {};
		window_class.cbSize = sizeof(WNDCLASSEXW);
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = window_proc;
		window_class.hInstance = GetModuleHandleA(nullptr);
		window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
		window_class.lpszClassName = L"sfWindowClass";
		static bool registered = false;
		if (!registered) {
			::RegisterClassExW(&window_class);
			registered = true;
		}

		const auto dpi = (f32)::GetDpiForSystem() / 96.f;

		RECT adjusted_rect = { .left = 0,
							   .top = 0,
							   .right = (LONG)((f32)size.width * dpi),
							   .bottom = (LONG)((f32)size.height * dpi) };
		AdjustWindowRect(&adjusted_rect, dwStyle, 0);

		const LONG width = adjusted_rect.right - adjusted_rect.left;
		const LONG height = adjusted_rect.bottom - adjusted_rect.top;

		const int monitor_width = GetSystemMetrics(SM_CXSCREEN);
		const int monitor_height = GetSystemMetrics(SM_CYSCREEN);

		const int x = monitor_width / 2 - width / 2;
		const int y = monitor_height / 2 - height / 2;

		auto wtitle = WString::from(title);

		HWND window = ::CreateWindowExW(
			0,
			window_class.lpszClassName,
			wtitle.begin(),
			dwStyle,
			x,
			y,
			size.width,
			size.height,
			nullptr,
			nullptr,
			window_class.hInstance,
			nullptr
		);

		auto swapchain = device.create_swapchain(window);
		auto result = Shared<OSWindow>::make(window, op::move(swapchain), size);
		::SetWindowLongPtrW(window, GWLP_USERDATA, (LONG_PTR) & *result);
		return result;
	}

	/**
	 * Closes the window.
	 * @return true if the window was closed, false if it was already closed.
	 */
	bool close() {
		if (!m_closed) {
			::PostMessage(m_handle, WM_CLOSE, 0, 0);
			m_closed = true;
			return true;
		}
		return false;
	}

	/**
	 * Sets the window's visibility.
	 * @return true if the visibility was changed, false if it was already set to the given value.
	 */
	bool set_visible(bool visible) {
		const bool changed = visible != (bool)m_visible;
		if (changed) {
			::ShowWindow(m_handle, visible ? SW_SHOW : SW_HIDE);
			m_visible = visible;
		}
		return changed;
	}

	/**
	 * Sets the window's focus.
	 * @return true if the focus was changed, false if it was already set to the given value.
	 */
	bool request_focus() {
		if (!m_focused && ::SetFocus(m_handle) != NULL) {
			m_focused = true;
		}

		return m_focused;
	}

	void set_min_size(const Vector2<u32>& size) { m_min_size = size; }
	void set_max_size(const Vector2<u32>& size) { m_max_size = size; }

	OP_NO_DISCARD OP_ALWAYS_INLINE Vector2<u32> size() const { return m_size; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_closed() const { return m_closed; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_visible() const { return m_visible; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_focused() const { return m_focused; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_minimized() const { return m_minimized; }
	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_maximized() const { return m_maximized; }

private:
	friend LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	friend class Application;

	HWND m_handle;
	Unique<gpu::Swapchain> m_swapchain;

	Vector2<u32> m_min_size;
	Vector2<u32> m_max_size;
	Vector2<u32> m_size;
	u32 m_closed	: 1 { 0 };
	u32 m_visible	: 1 { 0 };
	u32 m_focused	: 1 { 0 };
	u32 m_minimized : 1 { 0 };
	u32 m_maximized : 1 { 0 };
};

static LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	auto* window_ptr = (OSWindow*)::GetWindowLongPtrW(hwnd, GWLP_USERDATA);
	if (!window_ptr) {
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
	auto& window = *window_ptr;

	switch (msg) {
	case WM_SIZE: {
		const auto width = LOWORD(lparam);
		const auto height = HIWORD(lparam);
		window.m_size = { width, height };
		window.m_swapchain->resize();
		return 0;
	}
	case WM_KILLFOCUS:
		window.m_focused = false;
		return 0;
	case WM_SETFOCUS:
		window.m_focused = true;
		return 0;
	case WM_CLOSE:
		window.m_closed = true;
		return 0;
	case WM_GETMINMAXINFO:
		auto* info = (MINMAXINFO*)lparam;
		if (window.m_min_size.width > 0) {
			info->ptMinTrackSize.x = window.m_min_size.width;
		}
		if (window.m_min_size.height > 0) {
			info->ptMinTrackSize.y = window.m_min_size.height;
		}
		if (window.m_max_size.width > 0) {
			info->ptMaxTrackSize.x = window.m_max_size.width;
		}
		if (window.m_max_size.height > 0) {
			info->ptMaxTrackSize.y = window.m_max_size.height;
		}
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wparam, lparam);
}

static const char* source = R"#(
cbuffer bufs : register(b0) {
	float4x4 local_to_projection;
}

Texture2D texture2d_table[] : register(t0);
SamplerState sampler_table : register(s0);

float srgb_to_linear(float value) {
	if (value <= 0.04045) {
		return value / 12.92;
	} else {
		return pow((value + 0.055) / 1.055, 2.4);
	}
}

float4 uint_to_float4(uint color){
	float r = srgb_to_linear(((color & 0xff000000) >> 24) / 255.0);
	float g = srgb_to_linear(((color & 0xff0000) >> 16) / 255.0);
	float b = srgb_to_linear(((color & 0xff00) >> 8) / 255.0);
	float a = (color & 0xff) / 255.0;
	return float4(r, g, b, a);
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
		input.screen_pos.y <= input.scissor.y &&
		input.screen_pos.x <= input.scissor.z &&
		input.screen_pos.y >= input.scissor.w;

	if (true) { // if (in_scissor) {
		float4 output = input.color;

		if (input.tex2d != 0) {
			float alpha = texture2d_table[input.tex2d].Sample(sampler_table, input.uv, 0).x;
			output.w = alpha;
		}

		return output;
	}

	return float4(0, 0, 0, 0);
}
)#";

void Application::run(FunctionRef<void(Builder&)> callable) {
	auto& device = *m_device;
	auto window = OSWindow::spawn(device, "Application", { 1280, 720 });
	window->set_min_size(m_min_size);
	window->set_max_size(m_max_size);

	// Compile the vertex shader using source
	dxc::Input vertex_input = { source, "vs_main", dxc::ShaderType::Vertex };
	auto vertex_output = dxc::compile(vertex_input).unwrap();
	auto vertex_shader =
		m_device->create_vertex_shader(op::move(vertex_output.binary), op::move(vertex_output.input_parameters));

	// Compile the pixel shader using source
	dxc::Input pixel_input = { source, "ps_main", dxc::ShaderType::Pixel };
	auto pixel_output = dxc::compile(pixel_input).unwrap();
	auto pixel_shader = m_device->create_pixel_shader(op::move(pixel_output.binary));

	// Create the graphics pipeline
	gpu::GraphicsPipelineDefinition definition = {
		.vertex_shader = op::move(vertex_shader),
		.pixel_shader = op::move(pixel_shader),
		.blend_enabled = true,
		.src_color_blend_factor = gpu::BlendFactor::SrcAlpha,
		.dst_color_blend_factor = gpu::BlendFactor::OneMinusSrcAlpha,
	};
	definition.color_attachments.push(gpu::Format::RGBA_U8);
	auto pipeline = m_device->create_graphics_pipeline(op::move(definition));

	window->set_visible(true);
	while (true) {
		// Poll the OS for window events. This is required for the window to be responsive.
#if OP_PLATFORM_WINDOWS
		MSG msg = {};
		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
#endif
		// Kill the main loop if the window closed
		if (window->is_closed()) {
			break;
		}

		auto builder = Builder();
		callable(builder);

		auto command_list = device.record_graphics([&](auto& gcr) {
			auto& backbuffer = window->m_swapchain->backbuffer();
			gcr.texture_barrier(backbuffer, gpu::Layout::Present, gpu::Layout::ColorAttachment)
				.render_pass(
					backbuffer,
					nullopt,
					[&](auto& rpr) {
						const auto mesh = builder.canvas().tessellate();

						// Upload build vertex buffer to the gpu
						auto vertex_buffer = device.create_buffer(
							gpu::BufferUsage::Vertex,
							gpu::Heap::Upload,
							mesh.vertices.len() * sizeof(TessellatedCanvas::Vertex)
						);
						vertex_buffer->map([&](auto slice) {
							core::copy(slice.begin(), mesh.vertices.begin(), slice.len());
						});

						// Upload build index buffer to the gpu
						auto index_buffer = device.create_buffer(
							gpu::BufferUsage::Index,
							gpu::Heap::Upload,
							mesh.indices.len() * sizeof(TessellatedCanvas::Index)
						);
						index_buffer->map([&](auto slice) {
							core::copy(slice.begin(), mesh.indices.begin(), slice.len());
						});

						const auto size = window->size().cast<f32>();
						const auto projection = Matrix4<f32>::orthographic(size.x, size.y, 0.1f, 1000.f);
						const auto view =
							Matrix4<f32>::transform({ -size.x / 2, -size.y / 2, 0 }, Quaternion<f32>::identity, 1);
						const auto local_to_projection = projection * view;

						rpr
							// Upload the view matrix using push constants
							.push_constants(&local_to_projection)
							// Use the graphics pipeline created earlier
							.set_pipeline(pipeline)
							// Clear the screen to black
							.clear_color(LinearColor::white());

						rpr
							// Use the newly created vertex and index buffers
							.set_vertices(vertex_buffer, sizeof(TessellatedCanvas::Vertex))
							.set_indices(index_buffer, sizeof(TessellatedCanvas::Index))
							// Draw the mesh
							.draw_index(mesh.indices.len(), 0);

						// COLBY!!!! Need to upload the vertex and index buffer and then draw the mesh
					}
				)
				.texture_barrier(backbuffer, gpu::Layout::ColorAttachment, gpu::Layout::Present);
		});
		m_device->submit(command_list);
		window->m_swapchain->present();
	}
}

OP_GUI_NAMESPACE_END