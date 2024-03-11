// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string_view.h"
#include "core/containers/vector.h"
#include "core/math/vector2.h"
#include "gpu/texture.h"
#include "gui/gui.h"

struct stbtt_fontinfo;

OP_GUI_NAMESPACE_BEGIN

class Font {
public:
	static constexpr inline f32 SDF_SIZE = 32.f;
	static constexpr inline i32 NUM_GLYPHS = 512;
	struct Glyph {
		Vector2<f32> size;
		Vector2<f32> bearing;
		Vector2<f32> uv0, uv1;
		f32 advance;
	};

	static Option<Font> from_bytes(const gpu::Device& device, Vector<u8>&& bytes);

	Glyph const& glyph(Char c) const;
	OP_ALWAYS_INLINE gpu::Texture const& atlas() const { return m_atlas; }

	OP_ALWAYS_INLINE f32 ascent() const { return m_ascent; }
	OP_ALWAYS_INLINE f32 descent() const { return m_descent; }
	OP_ALWAYS_INLINE f32 line_gap() const { return m_line_gap; }

	f32 lookup_kerning(Char left, Char right) const;

private:
	explicit Font(
		Vector<u8>&& file_data,
		stbtt_fontinfo* stbtt_info,
		Vector<Glyph>&& glyphs,
		Vector<u32>&& codepoints_to_glyphs,
		gpu::Shared<gpu::Texture>&& atlas,
		f32 ascent,
		f32 descent,
		f32 line_gap,
		f32 scale
	);

	Vector<u8> m_bytes;
	stbtt_fontinfo* m_stbtt_info = nullptr;

	Vector<Glyph> m_glyphs;
	Vector<u32> m_codepoints_to_glyphs; // Map of codepoints to glyph indices
	gpu::Shared<gpu::Texture> m_atlas;

	f32 m_ascent;
	f32 m_descent;
	f32 m_line_gap;
	f32 m_scale; // stb scale used for kerning
};

OP_GUI_NAMESPACE_END