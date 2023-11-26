// Copyright Colby Hall. All Rights Reserved.

#include "gui/canvas.h"

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
	m_indices.push({ index, Shape::Rectangle });
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
			OP_UNIMPLEMENTED;
		} break;
		}
	}

	return result;
}

OP_GUI_NAMESPACE_END