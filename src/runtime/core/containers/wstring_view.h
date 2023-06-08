// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"

EU_CORE_NAMESPACE_BEGIN

using WChar = wchar_t;

class WStringView {
public:
	EU_ALWAYS_INLINE constexpr WStringView() : m_chars() {}
	EU_ALWAYS_INLINE WStringView(Slice<wchar_t const> bytes) : m_chars(bytes) {}
	EU_ALWAYS_INLINE constexpr WStringView(const wchar_t* ptr) : m_chars(ptr, core::constexpr_strlen(ptr)) {}

	EU_ALWAYS_INLINE operator Slice<wchar_t const>() const { return m_chars; }

	EU_NO_DISCARD EU_ALWAYS_INLINE usize len() const { return m_chars.len(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

	EU_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

private:
	Slice<WChar const> m_chars;
};

EU_CORE_NAMESPACE_END

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::WChar;
using core::WStringView;
EU_NAMESPACE_END