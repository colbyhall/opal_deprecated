// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/string_view.h"
#include "core/non_copyable.h"

GJ_CORE_NAMESPACE_BEGIN

class Library : NonCopyable {
public:
	static Option<Library> open(const StringView& path);

	template <typename F>
	F find(const StringView& path) {
		static_assert(std::is_function_v<F> || std::is_trivial_v<F>);
		void* f = find_internal(path);
		if (f != nullptr) {
			F casted = (F)(f);
			return casted;
		}
		return nullptr;
	}

	GJ_ALWAYS_INLINE Library(Library&& move) noexcept
		: m_handle(move.m_handle) {
		move.m_handle = nullptr;
	}
	GJ_ALWAYS_INLINE Library& operator=(Library&& move) noexcept {
		auto to_destroy = gj::move(*this);
		GJ_UNUSED(to_destroy);

		m_handle = move.m_handle;
		move.m_handle = nullptr;

		return *this;
	}
	~Library();

private:
	GJ_ALWAYS_INLINE explicit Library(void* handle) : m_handle(handle) {}
	void* find_internal(const StringView& name);

	void* m_handle;
};

GJ_CORE_NAMESPACE_END