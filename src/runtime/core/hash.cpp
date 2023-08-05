// Copyright Colby Hall. All Rights Reserved.

#include "core/hash.h"

GJ_CORE_NAMESPACE_BEGIN

const u64 FNV1Hasher::offset_basic = 0xcbf29ce484222325;
const u64 FNV1Hasher::prime = 0x100000001b3;

u64 FNV1Hasher::finish() { return m_result; }

void FNV1Hasher::write(Slice<u8 const> bytes) {
	u64 hash = FNV1Hasher::offset_basic;
	for (usize i = 0; i < bytes.len(); ++i) {
		hash *= FNV1Hasher::prime;
		hash = hash ^ bytes[i];
	}
	m_result |= hash;
}

GJ_CORE_NAMESPACE_END