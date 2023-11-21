// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"

OP_CORE_NAMESPACE_BEGIN

using WChar = wchar_t;

class WStringView {
public:
	OP_ALWAYS_INLINE constexpr WStringView() : m_chars() {}
	OP_ALWAYS_INLINE WStringView(Slice<wchar_t const> bytes) : m_chars(bytes) {}
	OP_ALWAYS_INLINE constexpr WStringView(const wchar_t* ptr) : m_chars(ptr, core::constexpr_strlen(ptr)) {}

	OP_ALWAYS_INLINE operator Slice<wchar_t const>() const { return m_chars; }

	OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_chars.len(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* operator*() const { return &m_chars[0]; }

	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* begin() const { return m_chars.cbegin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* end() const { return m_chars.cend(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

	OP_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

private:
	Slice<WChar const> m_chars;
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::WChar;
using core::WStringView;
OP_NAMESPACE_END