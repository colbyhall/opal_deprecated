// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/vector.h"
#include "core/containers/wstring_view.h"

SF_CORE_NAMESPACE_BEGIN

WChar utf32_to_utf16(Char c);

class WString {
public:
	SF_ALWAYS_INLINE constexpr WString() : m_chars() {}

	static WString from(WStringView view);
	static WString from(StringView string);

	operator Slice<WChar const>() const;
	operator WStringView() const;

	SF_NO_DISCARD SF_ALWAYS_INLINE WChar* ptr() { return m_chars.begin(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE WChar const* ptr() const {
		return m_chars.cbegin();
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* operator*() const {
		return m_chars.cbegin();
	}

	SF_NO_DISCARD SF_ALWAYS_INLINE usize len() const {
		return m_chars.len() > 0 ? m_chars.len() - 1 : 0;
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE usize cap() const { return m_chars.cap(); }

	SF_NO_DISCARD SF_ALWAYS_INLINE WChar* begin() { return m_chars.begin(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE WChar* end() { return m_chars.end(); }
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* cbegin() const {
		return m_chars.cbegin();
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE const WChar* cend() const {
		return m_chars.cend();
	}

	SF_ALWAYS_INLINE WChar& operator[](usize index) { return m_chars[index]; }
	SF_ALWAYS_INLINE WChar operator[](usize index) const {
		return m_chars[index];
	}

	SF_ALWAYS_INLINE void reserve(usize amount) {
		return m_chars.reserve(amount);
	}
	WString& push(WChar w);
	WString& push(WStringView string);
	WString& push(StringView string);

private:
	Vector<WChar> m_chars;
};

SF_CORE_NAMESPACE_END

// Export to gj namespace
SF_NAMESPACE_BEGIN
using core::WString;
SF_NAMESPACE_END