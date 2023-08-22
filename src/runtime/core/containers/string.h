// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/string_view.h"
#include "core/containers/vector.h"

SF_CORE_NAMESPACE_BEGIN

class WStringView;

class String {
public:
	String() = default;
	static String from(Vector<char>&& bytes);
	static String from(const StringView& view);
	static String from(const WStringView& view);

	operator Slice<char const>() const;
	operator StringView() const;
	SF_ALWAYS_INLINE char* operator*() { return &m_bytes[0]; }
	SF_ALWAYS_INLINE const char* operator*() const { return &m_bytes[0]; }

	SF_NO_DISCARD SF_ALWAYS_INLINE CharsIterator chars() const {
		return CharsIterator(m_bytes);
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE usize len() const {
		return m_bytes.len() > 0 ? m_bytes.len() - 1 : 0;
	}
	SF_NO_DISCARD SF_ALWAYS_INLINE usize cap() const { return m_bytes.cap(); }

	SF_ALWAYS_INLINE bool operator==(StringView rhs) const {
		StringView view = *this;
		return view == rhs;
	}
	SF_ALWAYS_INLINE bool operator!=(StringView rhs) const {
		StringView view = *this;
		return view != rhs;
	}

	SF_ALWAYS_INLINE void reserve(usize amount) { m_bytes.reserve(amount + 1); }
	String& push(Char c);
	String& push(StringView string);

private:
	Vector<char> m_bytes;
};

SF_CORE_NAMESPACE_END

SF_NAMESPACE_BEGIN
using core::String;
SF_NAMESPACE_END