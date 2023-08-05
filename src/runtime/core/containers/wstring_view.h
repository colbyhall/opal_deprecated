// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"

GJ_CORE_NAMESPACE_BEGIN

using WChar = wchar_t;

class WStringView {
public:
	GJ_ALWAYS_INLINE constexpr WStringView() : m_chars() {}
	GJ_ALWAYS_INLINE WStringView(Slice<wchar_t const> bytes) : m_chars(bytes) {}
	GJ_ALWAYS_INLINE constexpr WStringView(const wchar_t* ptr) : m_chars(ptr, core::constexpr_strlen(ptr)) {}

	GJ_ALWAYS_INLINE operator Slice<wchar_t const>() const { return m_chars; }

	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize len() const { return m_chars.len(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

	GJ_NO_DISCARD GJ_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

	GJ_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

private:
	Slice<WChar const> m_chars;
};

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::WChar;
using core::WStringView;
GJ_NAMESPACE_END