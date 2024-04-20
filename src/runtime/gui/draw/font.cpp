// Copyright Colby Hall. All Rights Reserved.

#include "gui/draw/font.h"
#include "gpu/buffer.h"
#include "gpu/device.h"
#include "gpu/graphics_command_list.h"

// Include stb headers
#include "stb/stb.h"
OP_SUPPRESS_WARNING_PUSH
OP_STB_SUPRESS_WARNINGS
#include "stb/stb_rect_pack.h"
#include "stb/stb_truetype.h"
OP_SUPPRESS_WARNING_POP

#include "core/os/windows.h"

OP_GUI_NAMESPACE_BEGIN

Option<Font> Font::from_bytes(const gpu::Device& device, Vector<u8>&& bytes) {
	stbtt_fontinfo* font_info = new stbtt_fontinfo{};
	stbtt_InitFont(font_info, bytes.begin(), stbtt_GetFontOffsetForIndex(bytes.begin(), 0));

	Vector<Glyph> glyphs;
	glyphs.reserve(font_info->numGlyphs);

	const auto scale = stbtt_ScaleForPixelHeight(font_info, Font::SDF_SIZE);
	const auto padding = 5;
	const auto on_edge_value = 180;
	const auto pixel_dist_scale = (f32)on_edge_value / (f32)padding;

	int ascent, descent, line_gap;
	stbtt_GetFontVMetrics(font_info, &ascent, &descent, &line_gap);

	const auto scaled_ascent = (f32)ascent * scale;
	const auto scaled_descent = (f32)descent * scale;
	const auto scaled_line_gap = (f32)line_gap * scale;

	// TODO: Look into adjusting the size of the atlas based on space available. The current size is arbitrary, but it
	// seems to work well enough
	const Vector2<u32> atlas_size = { 4096 };

	Vector<stbrp_node> rect_nodes;
	rect_nodes.reserve(atlas_size.width * atlas_size.height);

	stbrp_context packer = {};
	stbrp_init_target(&packer, atlas_size.width, atlas_size.height, rect_nodes.begin(), (int)rect_nodes.cap());

	Vector<u8> bitmap;
	bitmap.reserve(atlas_size.width * atlas_size.height);
	for (usize index = 0; index < bitmap.cap(); index += 1) {
		bitmap.push(0);
	}

	for (auto glyph = 0; glyph < NUM_GLYPHS; ++glyph) {
		int width, height, xoff, yoff;
		auto* sdf_bitmap = stbtt_GetGlyphSDF(
			font_info,
			scale,
			glyph,
			padding,
			on_edge_value,
			pixel_dist_scale,
			&width,
			&height,
			&xoff,
			&yoff
		);

		if (!sdf_bitmap) {
			glyphs.push(Glyph{});
			continue;
		}

		stbrp_rect rect = {};
		rect.w = width;
		rect.h = height;

		if (stbrp_pack_rects(&packer, &rect, 1)) {
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					const auto z = height - y;
					bitmap[(x + rect.x) + (z + rect.y) * (int)atlas_size.width] = sdf_bitmap[x + y * width];
				}
			}
		}

		stbtt_FreeSDF(sdf_bitmap, nullptr);

		const Vector2<f32> size = { (f32)width, (f32)height };
		const Vector2<f32> bearing = { (f32)(xoff), -(f32)((height + yoff)) };
		const Vector2<f32> uv0 = { (f32)rect.x / (f32)atlas_size.width, (f32)rect.y / (f32)atlas_size.height };
		const Vector2<f32> uv1 = { (f32)(rect.x + rect.w) / (f32)atlas_size.width,
								   (f32)(rect.y + rect.h) / (f32)atlas_size.height };
		glyphs.push(Glyph{ size, bearing, uv0, uv1, 0.f });
	}

	Vector<Char> codepoints_to_glyphs;
	codepoints_to_glyphs.reserve(0x110000);
	for (Char c = 0; c < 0x110000; ++c) {
		const auto index = stbtt_FindGlyphIndex(font_info, (int)c);
		codepoints_to_glyphs.push(index == -1 ? 0 : (u32)index);

		int advance, left_bearing;
		stbtt_GetCodepointHMetrics(font_info, c, &advance, &left_bearing);

		if (glyphs.is_valid_index((usize)index)) {
			glyphs[(usize)index].advance = (f32)advance * scale;
		}
	}

	using namespace gpu;
	auto pixels_buffer = device.create_buffer(BufferUsage::TransferSrc, Heap::Upload, bitmap.len());
	pixels_buffer->map([&](auto memory) { core::copy(memory.begin(), bitmap.begin(), bitmap.len()); });

	auto atlas =
		device.create_texture(TextureUsage::Sampled | TextureUsage::TransferDst, Format::R_U8, { atlas_size, 1 });

	auto command_list = device.record_graphics([&](auto& gcr) {
		gcr.texture_barrier(atlas, Layout::General, Layout::TransferDst);
		gcr.copy_buffer_to_texture(atlas, pixels_buffer);
		gcr.texture_barrier(atlas, Layout::TransferDst, Layout::General);
	});
	device.submit(command_list);

	return Font(
		op::move(bytes),
		font_info,
		op::move(glyphs),
		op::move(codepoints_to_glyphs),
		op::move(atlas),
		scaled_ascent,
		scaled_descent,
		scaled_line_gap,
		scale
	);
}

Font::Glyph const& Font::glyph(Char c) const {
	const auto index = m_codepoints_to_glyphs[(usize)c];
	if (!m_glyphs.is_valid_index(index)) return m_glyphs[0];
	return m_glyphs[(usize)index];
}

f32 Font::lookup_kerning(Char left, Char right) const {
	const auto left_index = m_codepoints_to_glyphs[(usize)left];
	const auto right_index = m_codepoints_to_glyphs[(usize)right];
	if (!m_glyphs.is_valid_index(left_index) || !m_glyphs.is_valid_index(right_index)) return 0.f;
	return stbtt_GetGlyphKernAdvance(m_stbtt_info, left_index, right_index) * m_scale;
}

Font::Font(
	Vector<u8>&& bytes,
	stbtt_fontinfo* stbtt_info,
	Vector<Glyph>&& glyphs,
	Vector<u32>&& codepoints_to_glyphs,
	gpu::Shared<gpu::Texture>&& atlas,
	f32 ascent,
	f32 descent,
	f32 line_gap,
	f32 scale
)
	: m_bytes(op::move(bytes))
	, m_stbtt_info(stbtt_info)
	, m_glyphs(op::move(glyphs))
	, m_codepoints_to_glyphs(op::move(codepoints_to_glyphs))
	, m_atlas(op::move(atlas))
	, m_ascent(ascent)
	, m_descent(descent)
	, m_line_gap(line_gap)
	, m_scale(scale) {}

OP_GUI_NAMESPACE_END
