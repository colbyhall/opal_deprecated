// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/non_null.h"
#include "core/non_copyable.h"
#include "core/os/memory.h"
#include "core/type_traits.h"

OP_CORE_NAMESPACE_BEGIN

// Alias nullptr for readability
constexpr NullPtr nullopt = nullptr;

template <typename T, typename Enable = void>
class Option {
public:
	// Constructors
	Option() = default;
	OP_ALWAYS_INLINE constexpr Option(NullPtr) : m_set(false), m_data() {}
	OP_ALWAYS_INLINE Option(T&& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(op::forward<T>(t));
	}
	OP_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	// Non trivially copyable Option's do not allow the use of the copy
	// operators
	Option(const Option<T>& copy) = delete;
	Option& operator=(const Option<T>& copy) = delete;

	// Move operators
	OP_ALWAYS_INLINE Option(Option<T>&& move) noexcept : m_set(move.m_set) {
		core::copy(m_data, move.m_data, sizeof(T));
		move.m_set = false;
	}

	OP_ALWAYS_INLINE Option& operator=(Option<T>&& m) noexcept {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		m_set = true;
		m.m_set = false;
		core::copy(m_data, m.m_data, sizeof(T));
		return *this;
	}

	OP_ALWAYS_INLINE Option& operator=(T&& t) {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		auto* p = m_data;
		m_set = true;
		new (p) T(op::forward<T>(t));
		return *this;
	}

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_set; }
	OP_ALWAYS_INLINE operator bool() const { return is_set(); }

	OP_ALWAYS_INLINE T unwrap() {
		OP_ASSERT(is_set(), "Value must be set to be unwrapped");
		m_set = false;

		auto* p = reinterpret_cast<T*>(&m_data[0]);
		return op::move(*p);
	}

	OP_ALWAYS_INLINE Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		}

		return Option<T&>();
	}

	OP_ALWAYS_INLINE Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		}

		return Option<T const&>();
	}

	OP_ALWAYS_INLINE ~Option() {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
			m_set = false;
		}
	}

private:
	bool m_set = false;
	alignas(T) u8 m_data[sizeof(T)] = {};
};

template <typename T>
class Option<T, std::enable_if_t<std::is_trivially_copyable_v<T>>> {
public:
	Option() = default;
	OP_ALWAYS_INLINE constexpr Option(NullPtr) : m_set(false), m_data() {}
	OP_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_set; }
	OP_ALWAYS_INLINE operator bool() const { return is_set(); }

	OP_ALWAYS_INLINE T unwrap() const {
		OP_ASSERT(is_set(), "Value must be set to be unwrapped");

		// Do not reset m_set for trivially copyable types

		auto* p = reinterpret_cast<const T*>(&m_data[0]);
		return *p;
	}

	OP_ALWAYS_INLINE Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		} else {
			return Option<T&>();
		}
	}

	OP_ALWAYS_INLINE Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		} else {
			return Option<T const&>();
		}
	}

	OP_ALWAYS_INLINE ~Option() {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
			m_set = false;
		}
	}

private:
	bool m_set = false;
	alignas(T) u8 m_data[sizeof(T)] = {};
};

template <typename T>
class Option<T&> {
public:
	explicit Option() = default;
	OP_ALWAYS_INLINE constexpr Option(NullPtr) : m_ptr(nullptr) {}
	OP_ALWAYS_INLINE constexpr Option(T& t) : m_ptr(&t) {}

	OP_NO_DISCARD OP_ALWAYS_INLINE bool is_set() const { return m_ptr != nullptr; }
	OP_ALWAYS_INLINE operator bool() const { return is_set(); }

	OP_ALWAYS_INLINE T& unwrap() {
		OP_ASSERT(is_set());
		return *m_ptr;
	}

private:
	T* m_ptr = nullptr;
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::nullopt;
using core::Option;
OP_NAMESPACE_END
