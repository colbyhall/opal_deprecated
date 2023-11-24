// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"
#include "core/containers/wstring_view.h"

OP_CORE_NAMESPACE_BEGIN

WChar utf32_to_utf16(Char c);

class WString {
public:
	OP_ALWAYS_INLINE constexpr WString() : m_chars() {}

	static WString from(WStringView view);
	static WString from(StringView string);

	operator Slice<WChar const>() const;
	operator WStringView() const;

	OP_NO_DISCARD OP_ALWAYS_INLINE WChar* ptr() { return m_chars.begin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE WChar const* ptr() const { return m_chars.begin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* operator*() const { return m_chars.begin(); }

	OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
	OP_NO_DISCARD OP_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

	OP_NO_DISCARD OP_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* begin() const { return m_chars.begin(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE const WChar* end() const { return m_chars.end(); }

	OP_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
	OP_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

	OP_ALWAYS_INLINE void reserve(usize amount) { return m_chars.reserve(amount); }
	WString& push(WChar w);
	WString& push(WStringView string);
	WString& push(StringView string);

private:
	Vector<WChar> m_chars;
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::WString;
OP_NAMESPACE_END