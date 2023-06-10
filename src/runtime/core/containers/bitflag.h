// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/type_traits.h"

EU_SUPPRESS_WARNINGS_STD_BEGIN
#include <bitset>
EU_SUPPRESS_WARNINGS_STD_END

EU_CORE_NAMESPACE_BEGIN

template <typename T>
class BitFlag {
	static_assert(std::is_enum_v<T>,
				  "eu::core::BitFlag can only be specialized for enum types");

public:
	using Underlying =
		typename std::make_unsigned_t<typename std::underlying_type_t<T>>;
	static inline constexpr auto size = sizeof(Underlying) * 8;

	BitFlag() = default;

	static EU_ALWAYS_INLINE BitFlag empty() { return {}; }
	static EU_ALWAYS_INLINE BitFlag all() {
		auto result = BitFlag::empty();
		for (auto i = 0; i < size; ++i) {
			result.m_bits.set(i, true);
		}
		return result;
	}

	EU_ALWAYS_INLINE BitFlag& set(T e, bool value = true) noexcept {
		m_bits.set(underlying(e), value);
		return *this;
	}

	EU_ALWAYS_INLINE BitFlag& flip(T e) noexcept {
		m_bits.flip(underlying(e));
		return *this;
	}

	EU_ALWAYS_INLINE BitFlag& reset(T e) noexcept {
		m_bits.set(underlying(e), false);
		return *this;
	}

	EU_ALWAYS_INLINE BitFlag& reset() noexcept {
		m_bits.reset();
		return *this;
	}

	EU_ALWAYS_INLINE bool is_all() const noexcept { m_bits.all(); }

	EU_ALWAYS_INLINE bool has_any() const noexcept { m_bits.any(); }

	EU_ALWAYS_INLINE bool is_empty() const noexcept { m_bits.none(); }

	EU_ALWAYS_INLINE Underlying count() const noexcept {
		return static_cast<Underlying>(m_bits.count());
	}

	constexpr bool operator[](T e) const { return m_bits[underlying(e)]; }

private:
	static constexpr Underlying underlying(T e) {
		return static_cast<Underlying>(e);
	}

	std::bitset<size> m_bits;
};

EU_CORE_NAMESPACE_END

EU_NAMESPACE_BEGIN
using core::BitFlag;
EU_NAMESPACE_END