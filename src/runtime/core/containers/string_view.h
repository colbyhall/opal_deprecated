// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/containers/slice.h"

OP_CORE_NAMESPACE_BEGIN

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
	OP_ALWAYS_INLINE explicit CharsIterator(Slice<char const> string)
		: m_string(string)
		, m_index(0)
		, m_decoder_state(0)
		, m_codepoint(0) {}

	OP_ALWAYS_INLINE operator bool() const { return should_continue(); }
	OP_ALWAYS_INLINE CharsIterator& operator++() {
		next();
		return *this;
	}
	OP_ALWAYS_INLINE Char operator*() const { return get(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE usize index() const { return m_index; }

private:
	OP_NO_DISCARD bool should_continue() const;
	void next();
	OP_NO_DISCARD Char get() const;

	Slice<char const> m_string;
	usize m_index;
	u32 m_decoder_state;
	Char m_codepoint;
};

class StringView {
public:
	OP_ALWAYS_INLINE constexpr StringView() = default;
	OP_ALWAYS_INLINE StringView(Slice<char const> bytes) : m_bytes(bytes) {}
	OP_ALWAYS_INLINE constexpr StringView(const char* ptr) : m_bytes(ptr, constexpr_strlen(ptr)) {}
	OP_ALWAYS_INLINE constexpr explicit StringView(const char* ptr, usize size) : m_bytes(ptr, size) {}

	OP_ALWAYS_INLINE operator Slice<char const>() const { return m_bytes; }
	OP_ALWAYS_INLINE const char* operator*() const { return &m_bytes[0]; }

	OP_NO_DISCARD OP_ALWAYS_INLINE usize len() const { return m_bytes.len(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE CharsIterator chars() const { return CharsIterator(m_bytes); }
	bool operator==(const StringView& right) const;
	bool operator!=(const StringView& right) const;

private:
	Slice<char const> m_bytes;
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Char;
using core::CharsIterator;
using core::StringView;
OP_NAMESPACE_END