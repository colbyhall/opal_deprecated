// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slice.h"
#include "core/type_traits.h"

EU_CORE_NAMESPACE_BEGIN

class Hasher {
public:
	virtual u64 finish() = 0;
	virtual void write(Slice<u8 const> bytes) = 0;
	virtual ~Hasher() = default;
};

class FNV1Hasher : public Hasher {
public:
	constexpr FNV1Hasher() = default;

	static const u64 offset_basic;
	static const u64 prime;

	// Hasher
	u64 finish() override;
	void write(Slice<u8 const> bytes) override;
	// ~Hasher

private:
	u64 m_result = 0;
};

EU_CORE_NAMESPACE_END

// Implement hash for eu primitives
EU_NAMESPACE_BEGIN

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

EU_NAMESPACE_END