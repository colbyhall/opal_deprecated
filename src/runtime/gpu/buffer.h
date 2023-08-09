// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/slice.h"
#include "gpu/gpu.h"

GJ_GPU_NAMESPACE_BEGIN

class IBuffer;

class Buffer {
public:
	enum class Kind : u8 { Storage, Upload, Download };
	enum class Usage : u8 {
		TransferSrc = (1 << 0),
		TransferDst = (1 << 1),
		Vertex = (1 << 2),
		Index = (1 << 3),
		Constant = (1 << 4),
	};

	static Buffer make(Usage usage, Kind kind, usize len);

	GJ_ALWAYS_INLINE Buffer::Usage usage() const;
	GJ_ALWAYS_INLINE Buffer::Kind kind() const;
	GJ_ALWAYS_INLINE usize len() const;
	GJ_ALWAYS_INLINE void map(FunctionRef<void(Slice<u8>)> func);

	template <typename T = IBuffer>
	GJ_ALWAYS_INLINE T const& cast() const {
		static_assert(
			std::is_base_of_v<IBuffer, T>,
			"T is not derived of IBuffer"
		);
		return static_cast<const T&>(*m_interface);
	}

private:
	GJ_ALWAYS_INLINE explicit Buffer(Shared<IBuffer>&& interface)
		: m_interface(gj::move(interface)) {}

	Shared<IBuffer> m_interface;
};

GJ_ENUM_CLASS_BITFIELD(Buffer::Usage)

class IBuffer {
public:
	virtual Buffer::Usage usage() const = 0;
	virtual Buffer::Kind kind() const = 0;
	virtual usize len() const = 0;
	virtual void map(FunctionRef<void(Slice<u8>)>& func) = 0;
	virtual ~IBuffer() = default;
};

GJ_ALWAYS_INLINE Buffer::Usage Buffer::usage() const {
	return m_interface->usage();
}
GJ_ALWAYS_INLINE Buffer::Kind Buffer::kind() const {
	return m_interface->kind();
}
GJ_ALWAYS_INLINE usize Buffer::len() const { return m_interface->len(); }
GJ_ALWAYS_INLINE void Buffer::map(FunctionRef<void(Slice<u8>)> func) {
	m_interface->map(func);
}

GJ_GPU_NAMESPACE_END
