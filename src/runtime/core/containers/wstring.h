// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/array.h"
#include "core/containers/wstring_view.h"

EU_CORE_NAMESPACE_BEGIN

wchar utf32_to_utf16(Char c);

class WString {
public:
	EU_ALWAYS_INLINE constexpr WString() : m_chars() {}

	static WString from(WStringView view);
	static WString from(StringView string);

	operator Slice<WChar const>() const;
	operator WStringView() const;

	EU_NO_DISCARD EU_ALWAYS_INLINE WChar* ptr() { return m_chars.begin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE WChar const* ptr() const { return m_chars.cbegin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* operator*() const { return m_chars.cbegin(); }

	EU_NO_DISCARD EU_ALWAYS_INLINE usize len() const { return m_chars.len() > 0 ? m_chars.len() - 1 : 0; }
	EU_NO_DISCARD EU_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

	EU_NO_DISCARD EU_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* cbegin() const { return m_chars.cbegin(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE const WChar* cend() const { return m_chars.cend(); }

	EU_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
	EU_ALWAYS_INLINE WChar operator[](usize index) const { return m_chars[index]; }

	EU_ALWAYS_INLINE void reserve(usize amount) { return m_chars.reserve(amount); }
	WString& push(WChar w);
	WString& push(WStringView string);
	WString& push(StringView string);

private:
	Array<WChar> m_chars;
};

EU_CORE_NAMESPACE_END

EU_NAMESPACE_BEGIN
using WString = core::WString;
EU_NAMESPACE_END