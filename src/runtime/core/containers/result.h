// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/non_copyable.h"
#include "core/os/memory.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Value, typename Error>
class Result final : NonCopyable {
public:
	EU_ALWAYS_INLINE Result(Value&& t) : m_set(true), m_ok(true) {
		auto* p = &m_data[0];
		new (p) Value(eu::forward<Value>(t));
	}

	EU_ALWAYS_INLINE Result(Error&& e) : m_set(true), m_ok(false) {
		auto* p = &m_data[0];
		new (p) Error(eu::forward<Error>(e));
	}

	Result(Result&& other) noexcept : m_set(other.m_set), m_ok(other.m_ok), m_data(other.m_data) {
		other.m_set = false;
		other.m_ok = false;
		other.m_data = {};
	}
	Result& operator=(Result&& other) noexcept {
		auto to_destroy = eu::move(*this);
		EU_UNUSED(to_destroy);

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

	EU_NO_DISCARD EU_ALWAYS_INLINE bool is_ok() const { return m_ok; }
	EU_ALWAYS_INLINE operator bool() const { return is_ok(); }

	EU_ALWAYS_INLINE Value unwrap() {
		EU_ASSERT(is_ok());
		m_set = false;

		auto* p = reinterpret_cast<Value*>(&m_data[0]);
		return eu::move(*p);
	}

	EU_ALWAYS_INLINE Error unwrap_err() {
		EU_ASSERT(!is_ok());
		m_set = false;
		auto* p = reinterpret_cast<Error*>(&m_data[0]);
		return eu::move(*p);
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

EU_CORE_NAMESPACE_END

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Result;
EU_NAMESPACE_END
