// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string.h"
#include "core/containers/vector.h"
#include "core/math/aabb2.h"
#include "core/math/color.h"
#include "core/math/vector4.h"
#include "gui/gui.h"

OP_GUI_NAMESPACE_BEGIN

class Font;

struct Rectangle {
	AABB2<f32> bounds;
	LinearColor color;
};
struct Text {
	AABB2<f32> bounds;
	String text;
	f32 size;
	LinearColor color;
};

struct TessellatedCanvas {
	struct Vertex {
		Vector4<f32> scissor;
		Vector2<f32> position;
		Vector2<f32> uv;
		u32 color;
		u32 texture;
	};
	using Index = u32;

	Vector<Vertex> vertices;
	Vector<Index> indices;
};

class Canvas {
public:
	explicit Canvas(const Font& font) : m_font(font) {}

	Canvas& push(Rectangle&& rectangle);
	Canvas& push(Text&& text);

	Canvas& insert(u32 index, Rectangle&& rectangle);
	Canvas& insert(u32 index, Text&& text);

	OP_ALWAYS_INLINE u32 len() const { return static_cast<u32>(m_indices.len()); }

	TessellatedCanvas tessellate() const;

private:
	const Font& m_font;

	enum class Shape { Rectangle, Text };
	Vector<Rectangle> m_rectangles;
	Vector<Text> m_texts;

	struct Index {
		u32 index;
		Shape shape;
	};
	Vector<Index> m_indices;
};

OP_GUI_NAMESPACE_END