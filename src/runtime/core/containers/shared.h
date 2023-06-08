// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"
#include "core/non_copyable.h"
#include "core/sync/atomic.h"
#include "core/type_traits.h"

EU_CORE_NAMESPACE_BEGIN

enum class SMode { NonAtomic, Atomic };

template <SMode Mode>
class SharedCounter;

template <typename Base, SMode Mode>
class Weak;

template <typename T, SMode Mode>
class SharedFromThis;

template <>
class SharedCounter<SMode::NonAtomic> {
public:
	SharedCounter() = default;

	EU_ALWAYS_INLINE u32 strong() const { return m_strong; }
	EU_ALWAYS_INLINE u32 weak() const { return m_weak; }

	EU_ALWAYS_INLINE u32 add_strong() const {
		m_strong += 1;
		return m_strong - 1;
	}
	EU_ALWAYS_INLINE u32 remove_strong() const {
		m_strong -= 1;
		return m_strong + 1;
	}

	EU_ALWAYS_INLINE u32 add_weak() const {
		m_weak += 1;
		return m_weak - 1;
	}
	EU_ALWAYS_INLINE u32 remove_weak() const {
		m_weak -= 1;
		return m_weak + 1;
	}

private:
	mutable u32 m_strong = 1;
	mutable u32 m_weak = 0;
};

template <>
class SharedCounter<SMode::Atomic> {
public:
	SharedCounter() = default;

	EU_ALWAYS_INLINE u32 strong() const { return m_strong.load(); }
	EU_ALWAYS_INLINE u32 weak() const { return m_weak.load(); }

	EU_ALWAYS_INLINE u32 add_strong() const { return m_strong.fetch_add(1); }
	EU_ALWAYS_INLINE u32 remove_strong() const { return m_strong.fetch_sub(1); }

	EU_ALWAYS_INLINE u32 add_weak() const { return m_weak.fetch_add(1); }
	EU_ALWAYS_INLINE u32 remove_weak() const { return m_weak.fetch_sub(1); }

private:
	Atomic<u32> m_strong = 1;
	Atomic<u32> m_weak = 0;
};

template <typename Base, SMode Mode = SMode::NonAtomic>
class Shared : private NonCopyable {
public:
	using Counter = SharedCounter<Mode>;

	template <typename... Args>
	static EU_ALWAYS_INLINE Shared<Base, Mode> make(Args&&... args) {
		struct Combined {
			SharedCounter<Mode> counter;
			Base base;
		};

		constexpr auto layout = core::Layout::single<Combined>;
		void* ptr = new (core::malloc(layout)) Combined{ SharedCounter<Mode>(), Base(forward<Args>(args)...) };

		auto result = Shared(ptr);
		if constexpr (std::is_base_of_v<SharedFromThisBase, Base>) {
			result->m_this = result.downgrade();
		}
		return result;
	}

	template <typename Derived = Base>
	Shared(Shared<Derived, Mode>&& move) noexcept : m_ptr(move.m_ptr) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");
		move.m_ptr = nullptr;
	}
	template <typename Derived = Base>
	Shared& operator=(Shared<Derived, Mode>&& m) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Shared<Base, Mode> to_destroy = eu::move(*this);
		m_ptr = m.m_ptr;
		m.m_ptr = nullptr;
		return *this;
	}

	~Shared();

	Weak<Base, Mode> downgrade() const;
	Shared<Base, Mode> clone() const;

	// Accessors
	EU_ALWAYS_INLINE operator NonNull<Base>() { return &value(); }
	EU_ALWAYS_INLINE operator NonNull<Base const>() const { return &value(); }
	EU_ALWAYS_INLINE operator Base*() { return &value(); }
	EU_ALWAYS_INLINE operator Base const*() const { return &value(); }
	EU_ALWAYS_INLINE Base* operator->() { return &value(); }
	EU_ALWAYS_INLINE Base const* operator->() const { return &value(); }
	EU_ALWAYS_INLINE Base& operator*() { return value(); }
	EU_ALWAYS_INLINE Base const& operator*() const { return value(); }

	EU_NO_DISCARD EU_ALWAYS_INLINE u32 strong() const { return counter().strong(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE u32 weak() const { return counter().weak(); }

private:
	Shared() = default;
	explicit Shared(void* ptr) : m_ptr(ptr) {}

	template <typename Derived, SMode Mode>
	friend class Shared;

	template <typename Derived, SMode Mode>
	friend class Weak;

	EU_ALWAYS_INLINE Counter const& counter() const { return *((Counter*)m_ptr); }
	EU_ALWAYS_INLINE Base& value() const {
		void* ptr = &((Counter*)m_ptr)[1];
		return *((Base*)ptr);
	}

	void* m_ptr;
};

template <typename Base, SMode Mode = SMode::NonAtomic>
class Weak : private NonCopyable {
public:
	using Counter = SharedCounter<Mode>;

	template <typename Derived = Base>
	Weak(Weak<Derived, Mode>&& move) noexcept : m_ptr(move.m_ptr) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");
		move.m_ptr = nullptr;
	}
	template <typename Derived = Base>
	Weak& operator=(Weak<Derived, Mode>&& m) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Weak<Base, Mode> to_destroy = eu::move(*this);
		m_ptr = m.m_ptr;
		m.m_ptr = nullptr;
		return *this;
	}

	~Weak();

	EU_NO_DISCARD Option<Shared<Base, Mode>> upgrade() const;
	Weak<Base, Mode> clone() const;

	EU_NO_DISCARD EU_ALWAYS_INLINE u32 strong() const { return counter().strong(); }
	EU_NO_DISCARD EU_ALWAYS_INLINE u32 weak() const { return counter().weak(); }

private:
	Weak() = default;
	explicit Weak(void* ptr) : m_ptr(ptr) {}

	template <typename Derived, SMode>
	friend class Shared;

	template <typename Derived, SMode>
	friend class Weak;

	EU_ALWAYS_INLINE Counter const& counter() const { return *((Counter*)m_ptr); }

	void* m_ptr;
};

class SharedFromThisBase {};

template <typename T, SMode Mode>
class SharedFromThis : SharedFromThisBase {
public:
	using Counter = SharedCounter<Mode>;

	Shared<T, Mode> to_shared() const;

private:
	template <typename Derived, SMode>
	friend class Shared;

	Option<Weak<T, Mode>> m_this;
};

EU_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/shared.inl"

// Export to eu namespace
EU_NAMESPACE_BEGIN
using core::Shared;
using core::SharedFromThis;
using core::SMode;
using core::Weak;
EU_NAMESPACE_END
