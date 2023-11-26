// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/string_view.h"
#include "gui/builder.h"

OP_GUI_NAMESPACE_BEGIN

class Label {
public:
	explicit Label(StringView text) : m_text(text) {}

	void render(Builder& builder);

private:
	StringView m_text;
};

OP_GUI_NAMESPACE_END