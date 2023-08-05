// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slice.h"
#include "core/type_traits.h"

GJ_CORE_NAMESPACE_BEGIN

class IHasher {
public:
	virtual u64 finish() = 0;
	virtual void write(Slice<u8 const> bytes) = 0;
	virtual ~IHasher() = default;
};

class FNV1Hasher final : public IHasher {
public:
	constexpr FNV1Hasher() = default;

	static const u64 offset_basic;
	static const u64 prime;

	// IHasher
	u64 finish() final;
	void write(Slice<u8 const> bytes) final;
	// ~IHasher

private:
	u64 m_result = 0;
};

GJ_CORE_NAMESPACE_END

// Implement hash for gj primitives
GJ_NAMESPACE_BEGIN

template <typename H>
void hash(H& hasher, const u8& value) {
	hasher.write(Slice<u8 const>({ &value, sizeof(u8) }));
}

template <typename H>
void hash(H& hasher, const u16& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u16)));
}

template <typename H>
void hash(H& hasher, const u32& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u32)));
}

template <typename H>
void hash(H& hasher, const u64& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(u64)));
}

template <typename H>
void hash(H& hasher, const i8& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i8)));
}

template <typename H>
void hash(H& hasher, const i16& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i16)));
}

template <typename H>
void hash(H& hasher, const i32& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i32)));
}

template <typename H>
void hash(H& hasher, const i64& value) {
	hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(i64)));
}

GJ_NAMESPACE_END