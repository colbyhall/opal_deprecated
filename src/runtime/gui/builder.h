// Copyright Colby Hall. All Rights Reserved.

#include "gui/draw/canvas.h"

OP_GUI_NAMESPACE_BEGIN

class Font;

class Builder {
public:
	explicit Builder(const Font& font) : m_canvas(font) {}

	OP_NO_DISCARD OP_ALWAYS_INLINE Canvas& canvas() { return m_canvas; }
	OP_NO_DISCARD OP_ALWAYS_INLINE Canvas const& canvas() const { return m_canvas; }

private:
	Canvas m_canvas;
};

OP_GUI_NAMESPACE_END