// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/non_null.h"
#include "core/non_copyable.h"
#include "core/os/memory.h"
#include "core/type_traits.h"

SF_CORE_NAMESPACE_BEGIN

// Alias nullptr for readability
constexpr NullPtr none = nullptr;

template <typename T, typename Enable = void>
class Option {
public:
	// Constructors
	Option() = default;
	SF_ALWAYS_INLINE constexpr Option(NullPtr) : m_set(false), m_data() {}
	SF_ALWAYS_INLINE Option(T&& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(sf::forward<T>(t));
	}
	SF_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	// Non trivially copyable Option's do not allow the use of the copy
	// operators
	Option(const Option<T>& copy) = delete;
	Option& operator=(const Option<T>& copy) = delete;

	// Move operators
	SF_ALWAYS_INLINE Option(Option<T>&& move) noexcept : m_set(move.m_set) {
		core::copy(m_data, move.m_data, sizeof(T));
		move.m_set = false;
	}

	SF_ALWAYS_INLINE Option& operator=(Option<T>&& m) noexcept {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		m_set = true;
		m.m_set = false;
		core::copy(m_data, m.m_data, sizeof(T));
		return *this;
	}

	SF_ALWAYS_INLINE Option& operator=(T&& t) {
		if (m_set) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			p->~T();
		}
		auto* p = m_data;
		m_set = true;
		new (p) T(sf::forward<T>(t));
		return *this;
	}

	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_set() const { return m_set; }
	SF_ALWAYS_INLINE operator bool() const { return is_set(); }

	SF_ALWAYS_INLINE T unwrap() {
		SF_ASSERT(is_set(), "Value must be set to be unwrapped");
		m_set = false;

		auto* p = reinterpret_cast<T*>(&m_data[0]);
		return sf::move(*p);
	}

	SF_ALWAYS_INLINE Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		}

		return Option<T&>();
	}

	SF_ALWAYS_INLINE Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		}

		return Option<T const&>();
	}

	SF_ALWAYS_INLINE ~Option() {
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
	SF_ALWAYS_INLINE constexpr Option(NullPtr) : m_set(false), m_data() {}
	SF_ALWAYS_INLINE Option(const T& t) : m_set(true), m_data() {
		auto* p = m_data;
		new (p) T(t);
	}

	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_set() const { return m_set; }
	SF_ALWAYS_INLINE operator bool() const { return is_set(); }

	SF_ALWAYS_INLINE T unwrap() const {
		SF_ASSERT(is_set(), "Value must be set to be unwrapped");

		// Do not reset m_set for trivially copyable types

		auto* p = reinterpret_cast<const T*>(&m_data[0]);
		return *p;
	}

	SF_ALWAYS_INLINE Option<T&> as_mut() {
		if (is_set()) {
			auto* p = reinterpret_cast<T*>(&m_data[0]);
			return Option<T&>(*p);
		} else {
			return Option<T&>();
		}
	}

	SF_ALWAYS_INLINE Option<T const&> as_ref() const {
		if (is_set()) {
			auto* p = reinterpret_cast<T const*>(&m_data[0]);
			return Option<T const&>(*p);
		} else {
			return Option<T const&>();
		}
	}

	SF_ALWAYS_INLINE ~Option() {
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
	SF_ALWAYS_INLINE constexpr Option(NullPtr) : m_ptr(nullptr) {}
	SF_ALWAYS_INLINE constexpr Option(T& t) : m_ptr(&t) {}

	SF_NO_DISCARD SF_ALWAYS_INLINE bool is_set() const { return m_ptr != nullptr; }
	SF_ALWAYS_INLINE operator bool() const { return is_set(); }

	SF_ALWAYS_INLINE T& unwrap() {
		SF_ASSERT(is_set());
		return *m_ptr;
	}

private:
	T* m_ptr = nullptr;
};

SF_CORE_NAMESPACE_END

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::none;
using core::Option;
SF_NAMESPACE_END
