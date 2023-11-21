// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/non_copyable.h"
#include "core/os/memory.h"

OP_CORE_NAMESPACE_BEGIN

template <typename Value, typename Error>
class Result final : NonCopyable {
public:
	OP_ALWAYS_INLINE Result(Value&& t) : m_set(true), m_ok(true) {
		auto* p = &m_data[0];
		new (p) Value(op::forward<Value>(t));
	}

	OP_ALWAYS_INLINE Result(Error&& e) : m_set(true), m_ok(false) {
		auto* p = &m_data[0];
		new (p) Error(op::forward<Error>(e));
	}

	Result(Result&& other) noexcept : m_set(other.m_set), m_ok(other.m_ok), m_data(other.m_data) {
		other.m_set = false;
		other.m_ok = false;
		other.m_data = {};
	}
	Result& operator=(Result&& other) noexcept {
		auto to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		m_set = other.m_set;
		m_ok = other.m_ok;
		m_data = other.m_data;
		other.m_set = false;
		other.m_ok = false;
	}

	~Result() {
		if (m_set) {
			if (m_ok) {
				auto* p = reinterpret_cast<Value*>(&m_data[0]);
				p->~Value();
			} else {
				auto* p = reinterpret_cast<Error*>(&m_data[0]);
				p->~Error();
			}

			m_set = false;
			m_ok = false;
		}
	}

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_ok() const { return m_ok; }
	OP_ALWAYS_INLINE operator bool() const { return is_ok(); }

	OP_ALWAYS_INLINE Value unwrap() {
		OP_ASSERT(is_ok());
		m_set = false;

		auto* p = reinterpret_cast<Value*>(&m_data[0]);
		return op::move(*p);
	}

	OP_ALWAYS_INLINE Error unwrap_err() {
		OP_ASSERT(!is_ok());
		m_set = false;
		auto* p = reinterpret_cast<Error*>(&m_data[0]);
		return op::move(*p);
	}

private:
	u8 m_set : 1;
	u8 m_ok	 : 1; // Results technically can also be null after move

	union Internal {
		Value t;
		Error e;
		~Internal() {} // This prevents warning C4624
	};
	alignas(Internal) u8 m_data[sizeof(Internal)] = {};
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Result;
OP_NAMESPACE_END
