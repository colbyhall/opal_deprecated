// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/string_view.h"
#include "core/non_copyable.h"

SF_CORE_NAMESPACE_BEGIN

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

	SF_ALWAYS_INLINE Library(Library&& move) noexcept
		: m_handle(move.m_handle) {
		move.m_handle = nullptr;
	}
	SF_ALWAYS_INLINE Library& operator=(Library&& move) noexcept {
		auto to_destroy = sf::move(*this);
		SF_UNUSED(to_destroy);

		m_handle = move.m_handle;
		move.m_handle = nullptr;

		return *this;
	}
	~Library();

private:
	SF_ALWAYS_INLINE explicit Library(void* handle) : m_handle(handle) {}
	void* find_internal(const StringView& name);

	void* m_handle;
};

SF_CORE_NAMESPACE_END