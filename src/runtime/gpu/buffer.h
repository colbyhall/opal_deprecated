// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/slice.h"
#include "gpu/gpu.h"

EU_GPU_NAMESPACE_BEGIN

class IBuffer;

class Buffer {
public:
	enum class Kind : u8 { Storage, Upload, Download };
	enum class Usage : u8 {
		Vertex = (1 << 0),
		Index = (1 << 1),
		Constant = (1 << 2),
	};

	static Buffer make(Usage usage, Kind kind, usize len);

	template <typename T = IBuffer>
	T const& interface() const {
		static_assert(std::is_base_of_v<IBuffer, T>, "T is not derived of IBuffer");
		return static_cast<const T&>(*m_interface);
	}

private:
	EU_ALWAYS_INLINE explicit Buffer(Shared<IBuffer>&& interface) : m_interface(eu::move(interface)) {}

	Shared<IBuffer> m_interface;
};

EU_ENUM_CLASS_BITFIELD(Buffer::Usage)

class IBuffer {
public:
	virtual Buffer::Usage usage() const = 0;
	virtual Buffer::Kind kind() const = 0;
	virtual usize len() const = 0;
	virtual void map(FunctionRef<void(Slice<u8>)>& func) = 0;
	virtual ~IBuffer() = default;
};

EU_GPU_NAMESPACE_END