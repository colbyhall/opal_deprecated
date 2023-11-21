// Copyright Colby Hall. All Rights Reserved.

#include "core/os/library.h"

#ifdef OP_PLATFORM_WINDOWS
	#include "core/containers/wstring.h"
	#include "core/os/windows.h"
#else
	#error "op::core::Library unimplemented on this platform"
#endif

OP_CORE_NAMESPACE_BEGIN

Option<Library> Library::open(const StringView& path) {
	WString wpath;
	wpath.reserve(path.len());
	wpath.push(path);

	void* handle = LoadLibraryW(wpath.ptr());
	if (handle != nullptr) return Library{ handle };
	return nullptr;
}

Library::~Library() {
	if (m_handle) {
		FreeLibrary((HMODULE)m_handle);
		m_handle = nullptr;
	}
}

void* Library::find_internal(const StringView& name) { return (void*)GetProcAddress((HMODULE)m_handle, *name); }

OP_CORE_NAMESPACE_END