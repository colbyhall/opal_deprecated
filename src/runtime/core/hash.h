// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/slice.h"
#include "core/type_traits.h"

OP_CORE_NAMESPACE_BEGIN

class Hasher {
public:
	virtual u64 finish() = 0;
	virtual void write(Slice<u8 const> bytes) = 0;
	virtual ~Hasher() = default;
};

class FNV1Hasher final : public Hasher {
public:
	constexpr FNV1Hasher() = default;

	static const u64 offset_basic;
	static const u64 prime;

	// Hasher
	u64 finish() final;
	void write(Slice<u8 const> bytes) final;
	// ~Hasher

private:
	u64 m_result = 0;
};

OP_CORE_NAMESPACE_END

// Implement hash for op primitives
OP_NAMESPACE_BEGIN

template <typename H, typename T>
struct Hash {
	void operator()(H& hasher, const T& value) {
		static_assert(
			std::is_trivially_copyable_v<T>,
			"Hashing requires trivially copyable types or custom hash specialization"
		);
		hasher.write(Slice<u8 const>((u8 const*)(&value), sizeof(T)));
	}
};

OP_NAMESPACE_END