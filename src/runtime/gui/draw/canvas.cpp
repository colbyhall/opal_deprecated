// Copyright Colby Hall. All Rights Reserved.

#include "gui/draw/canvas.h"
#include "gui/draw/font.h"

OP_GUI_NAMESPACE_BEGIN

Canvas& Canvas::push(Rectangle&& rectangle) {
	const auto index = static_cast<u32>(m_rectangles.len());
	m_rectangles.push(op::move(rectangle));
	m_indices.push({ index, Shape::Rectangle });
	return *this;
}
Canvas& Canvas::push(Text&& text) {
	const auto index = static_cast<u32>(m_texts.len());
	m_texts.push(op::move(text));
	m_indices.push({ index, Shape::Text });
	return *this;
}

Canvas& Canvas::insert(u32 index, Rectangle&& rectangle) {
	const auto rectangle_index = static_cast<u32>(m_rectangles.len());
	m_rectangles.push(op::move(rectangle));
	m_indices.insert(index, { rectangle_index, Shape::Rectangle });
	return *this;
}
Canvas& Canvas::insert(u32 index, Text&& text) {
	const auto text_index = static_cast<u32>(m_texts.len());
	m_texts.push(op::move(text));
	m_indices.insert(index, { text_index, Shape::Text });
	return *this;
}

TessellatedCanvas Canvas::tessellate() const {
	TessellatedCanvas result;
	result.vertices.reserve(m_indices.len() * 4);
	result.indices.reserve(m_indices.len() * 6);

	for (auto& index : m_indices) {
		switch (index.shape) {
		case Shape::Rectangle: {
			const auto& rectangle = m_rectangles[index.index];
			const auto color = rectangle.color.to_srgb();
			const auto& bounds = rectangle.bounds;

			const auto& min = bounds.min();
			const auto& max = bounds.max();

			// TODO: Implement scissor
			const auto scissor =
				Vector4(-core::infinity<f32>, -core::infinity<f32>, core::infinity<f32>, core::infinity<f32>);

			const u32 texture = 0;

			result.vertices.push({ .scissor = scissor,
								   .position = { min.x, min.y },
								   .uv = { 0.0f, 0.0f },
								   .color = (u32)color,
								   .texture = texture });

			result.vertices.push({ .scissor = scissor,
								   .position = { max.x, min.y },
								   .uv = { 1.0f, 0.0f },
								   .color = (u32)color,
								   .texture = texture });

			result.vertices.push({ .scissor = scissor,
								   .position = { max.x, max.y },
								   .uv = { 1.0f, 1.0f },
								   .color = (u32)color,
								   .texture = texture });

			result.vertices.push({ .scissor = scissor,
								   .position = { min.x, max.y },
								   .uv = { 0.0f, 1.0f },
								   .color = (u32)color,
								   .texture = texture });

			const auto vertex_index = static_cast<u32>(result.vertices.len()) - 4;
			result.indices.push(vertex_index + 0);
			result.indices.push(vertex_index + 1);
			result.indices.push(vertex_index + 2);
			result.indices.push(vertex_index + 0);
			result.indices.push(vertex_index + 2);
			result.indices.push(vertex_index + 3);
		} break;
		case Shape::Text: {
			const auto& text = m_texts[index.index];
			const auto color = text.color.to_srgb();
			const auto& bounds = text.bounds;

			const auto& min = bounds.min();
			const auto& max = bounds.max();

			// TODO: Implement scissor
			const auto scissor =
				Vector4(-core::infinity<f32>, -core::infinity<f32>, core::infinity<f32>, core::infinity<f32>);

			const auto texture = m_font.atlas().bindless();

			const Vector2<f32> origin = { min.x, max.y };

			const f32 scale = Font::SDF_SIZE / text.size;
			// const f32 size = (m_font.ascent() - m_font.descent()) * scale;
			const f32 new_line = ((m_font.ascent() - m_font.descent()) + m_font.line_gap()) * scale;

			auto position = origin;
			position.y -= text.size + new_line;

			auto push_rect = [&](const Vector2<f32>& min,
								 const Vector2<f32>& max,
								 const Vector2<f32>& uv_min,
								 const Vector2<f32>& uv_max) {
				result.vertices.push(
					{ .scissor = scissor, .position = min, .uv = uv_min, .color = (u32)color, .texture = texture }
				);

				result.vertices.push({ .scissor = scissor,
									   .position = { max.x, min.y },
									   .uv = { uv_max.x, uv_min.y },
									   .color = (u32)color,
									   .texture = texture });

				result.vertices.push(
					{ .scissor = scissor, .position = max, .uv = uv_max, .color = (u32)color, .texture = texture }
				);

				result.vertices.push({ .scissor = scissor,
									   .position = { min.x, max.y },
									   .uv = { uv_min.x, uv_max.y },
									   .color = (u32)color,
									   .texture = texture });

				const auto vertex_index = static_cast<u32>(result.vertices.len()) - 4;
				result.indices.push(vertex_index + 0);
				result.indices.push(vertex_index + 1);
				result.indices.push(vertex_index + 2);
				result.indices.push(vertex_index + 0);
				result.indices.push(vertex_index + 2);
				result.indices.push(vertex_index + 3);
			};

			for (auto iter = text.text.chars(); iter; ++iter) {
				auto c = *iter;

				switch (c) {
				case '\n': {
					position.x = origin.x;
					position.y -= new_line;
				} break;
				case '\r': {
					position.x = origin.x;
				} break;
				case '\t': {
					auto& space_glyph = m_font.glyph(' ');
					position.x += space_glyph.advance * scale;
				} break;
				default:
					auto& glyph = m_font.glyph(c);
					const auto xy0 = position + glyph.bearing * scale;
					const auto xy1 = xy0 + glyph.size * scale;

					push_rect(xy0, xy1, glyph.uv0, glyph.uv1);
					position.x += glyph.advance * scale;

					auto peek = iter;
					if (peek) {
						++peek;

						const auto next = *peek;
						const auto kern = m_font.lookup_kerning(c, next) * scale;
						if (kern != 0.f) {
							position.x += kern;
						}
					}

					break;
				}
			};

		} break;
		}
	}

	return result;
}

OP_GUI_NAMESPACE_END