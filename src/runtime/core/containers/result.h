// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/non_copyable.h"
#include "core/os/memory.h"

GJ_CORE_NAMESPACE_BEGIN

template <typename Value, typename Error>
class Result final : NonCopyable {
public:
	GJ_ALWAYS_INLINE Result(Value&& t) : m_set(true), m_ok(true) {
		auto* p = &m_data[0];
		new (p) Value(gj::forward<Value>(t));
	}

	GJ_ALWAYS_INLINE Result(Error&& e) : m_set(true), m_ok(false) {
		auto* p = &m_data[0];
		new (p) Error(gj::forward<Error>(e));
	}

	Result(Result&& other) noexcept : m_set(other.m_set), m_ok(other.m_ok), m_data(other.m_data) {
		other.m_set = false;
		other.m_ok = false;
		other.m_data = {};
	}
	Result& operator=(Result&& other) noexcept {
		auto to_destroy = gj::move(*this);
		GJ_UNUSED(to_destroy);

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

	GJ_NO_DISCARD GJ_ALWAYS_INLINE bool is_ok() const { return m_ok; }
	GJ_ALWAYS_INLINE operator bool() const { return is_ok(); }

	GJ_ALWAYS_INLINE Value unwrap() {
		GJ_ASSERT(is_ok());
		m_set = false;

		auto* p = reinterpret_cast<Value*>(&m_data[0]);
		return gj::move(*p);
	}

	GJ_ALWAYS_INLINE Error unwrap_err() {
		GJ_ASSERT(!is_ok());
		m_set = false;
		auto* p = reinterpret_cast<Error*>(&m_data[0]);
		return gj::move(*p);
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

GJ_CORE_NAMESPACE_END

// Export to gj namespace
GJ_NAMESPACE_BEGIN
using core::Result;
GJ_NAMESPACE_END
