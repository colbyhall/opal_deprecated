// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"

SF_CORE_NAMESPACE_BEGIN

using WChar = wchar_t;

class WStringView {
public:
	SF_ALWAYS_INLINE constexpr WStringView() : m_chars() {}
	SF_ALWAYS_INLINE WStringView(Slice<wchar_t const> bytes) : m_chars(bytes) {}
	SF_ALWAYS_INLINE constexpr WStringView(const wchar_t* ptr) : m_chars(ptr, core::constexpr_strlen(ptr)) {}

	SF_ALWAYS_INLINE operator Slice<wchar_t const>() const { return m_chars; }

	SF_NO_DISCARD SF_ALWAYS_INLINE usize len() const { return m_chars.len(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

	SF_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

private:
	Slice<WChar const> m_chars;
};

SF_CORE_NAMESPACE_END

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::WChar;
using core::WStringView;
SF_NAMESPACE_END