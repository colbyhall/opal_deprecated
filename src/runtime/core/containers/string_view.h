// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
constexpr usize constexpr_strlen(const T* string) {
	if (*string == 0) return 0;
	string += 1;
	return 1 + constexpr_strlen(string);
}

using Char = u32;
constexpr Char EOS = 0;
constexpr Char EOL = '\n';
constexpr Char UTF8_BOM = 0xfeff;

constexpr u32 utf8_accept = 0;
constexpr u32 utf8_reject = 12;

u32 utf8_decode(u32* state, u32* code_p, u32 byte);
i32 utf8_encode(u32 c, void* dest, u32* errors);

class CharsIterator {
public:
	GJ_ALWAYS_INLINE explicit CharsIterator(Slice<char const> string)
		: m_string(string)
		, m_index(0)
		, m_decoder_state(0)
		, m_codepoint(0) {}

	GJ_ALWAYS_INLINE operator bool() const { return should_continue(); }
	GJ_ALWAYS_INLINE CharsIterator& operator++() {
		next();
		return *this;
	}
	GJ_ALWAYS_INLINE Char operator*() const { return get(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize index() const { return m_index; }

private:
	GJ_NO_DISCARD bool should_continue() const;
	void next();
	GJ_NO_DISCARD Char get() const;

	Slice<char const> m_string;
	usize m_index;
	u32 m_decoder_state;
	Char m_codepoint;
};

class StringView {
public:
	GJ_ALWAYS_INLINE constexpr StringView() = default;
	GJ_ALWAYS_INLINE StringView(Slice<char const> bytes) : m_bytes(bytes) {}
	GJ_ALWAYS_INLINE constexpr StringView(const char* ptr)
		: m_bytes(ptr, constexpr_strlen(ptr)) {}
	GJ_ALWAYS_INLINE constexpr explicit StringView(const char* ptr, usize size)
		: m_bytes(ptr, size) {}

	GJ_ALWAYS_INLINE operator Slice<char const>() const { return m_bytes; }
	GJ_ALWAYS_INLINE const char* operator*() const { return &m_bytes[0]; }

	GJ_NO_DISCARD GJ_ALWAYS_INLINE usize len() const { return m_bytes.len(); }
	GJ_NO_DISCARD GJ_ALWAYS_INLINE CharsIterator chars() const {
		return CharsIterator(m_bytes);
	}
	bool operator==(const StringView& right) const;
	bool operator!=(const StringView& right) const;

private:
	Slice<char const> m_bytes;
};

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::CharsIterator;
using core::StringView;
GJ_NAMESPACE_END