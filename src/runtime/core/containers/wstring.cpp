// Copyright Colby Hall. All Rights Reserved.

#include "core/containers/wstring.h"

SF_CORE_NAMESPACE_BEGIN

WChar utf32_to_utf16(Char c) {
	u32 h;
	u32 l;

	if (c < 0x10000) {
		h = 0;
		l = c;
		return static_cast<WChar>(c);
	}
	Char t = c - 0x10000;
	h = (((t << 12) >> 22) + 0xD800);
	l = (((t << 22) >> 22) + 0xDC00);
	Char ret = ((h << 16) | (l & 0x0000FFFF));
	return static_cast<WChar>(ret);
}

WString WString::from(WStringView view) {
	WString string;
	string.push(view);
	return string;
}

WString WString::from(StringView string) {
	WString result;
	result.reserve(string.len());

	for (auto iter = string.chars(); iter; ++iter) {
		const Char c = *iter;
		result.push(utf32_to_utf16(c));
	}

	return result;
}

WString::operator Slice<WChar const>() const {
	Slice<WChar const> result = m_chars;
	if (m_chars.len() > 0) {
		result.shrink(1);
	}
	return result;
}

WString::operator WStringView() const {
	Slice<WChar const> bytes = m_chars;

	// Exclude the null terminator
	bytes.shrink(m_chars.len() - len());

	return bytes;
}

WString& WString::push(WChar w) {
	const auto start_len = m_chars.len();

	if (start_len == 0) {
		m_chars.push(w);
		m_chars.push(0);
	} else {
		m_chars.insert(start_len - 1, w);
	}

	return *this;
}

WString& WString::push(WStringView string) {
	const usize slag = m_chars.cap() - m_chars.len();
	if (slag < string.len()) m_chars.reserve(string.len());

	if (m_chars.len() == 0) m_chars.push(0);
	for (WChar w : string)
		m_chars.insert(m_chars.len() - 1, w);

	return *this;
}

WString& WString::push(StringView string) {
	const usize slag = m_chars.cap() - m_chars.len();
	if (slag < string.len()) m_chars.reserve(string.len());

	for (auto iter = string.chars(); iter; ++iter) {
		push(utf32_to_utf16(*iter));
	}

	return *this;
}

SF_CORE_NAMESPACE_END