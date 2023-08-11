// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "editor/editor.h"

GJ_EDITOR_NAMESPACE_BEGIN

class Application {
public:
	explicit Application(int argc, char** argv);

	void run(FunctionRef<void()> f);

private:
};

#define GJ_EDITOR_MAIN(f)                                                      \
	int main(int argc, char** argv) {                                          \
		gj::editor::Application(argc, argv).run([]() f);                       \
		return 0;                                                              \
	}

GJ_EDITOR_NAMESPACE_END
