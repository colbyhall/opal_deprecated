// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/atomic.h"
#include "core/containers/option.h"

OP_CORE_NAMESPACE_BEGIN

enum class SMode { NonAtomic, Atomic };

template <SMode Mode>
class SharedCounter;

template <typename Base, SMode Mode>
class Weak;

class SharedFromThisBase;

template <typename Base, SMode Mode>
class Shared {
public:
	using Counter = SharedCounter<Mode>;

	template <typename... Args>
	static Shared<Base, Mode> make(Args&&... args) {
		struct Combined {
			SharedCounter<Mode> counter;
			Base base;
		};

		const auto layout = core::Layout::single<Combined>;
		Combined* ptr = new (core::malloc(layout)) Combined{ SharedCounter<Mode>(), Base(op::move(args)...) };

		auto result = Shared(&ptr->counter, &ptr->base);
		if constexpr (std::is_base_of_v<SharedFromThisBase, Base>) {
			result->m_this = result.downgrade();
		}
		return result;
	}

	Shared(const Shared<Base, Mode>& copy) noexcept : m_counter(copy.m_counter), m_base(copy.m_base) {
		auto& c = counter();
		c.add_strong();
	}
	Shared& operator=(const Shared<Base, Mode>& copy) noexcept {
		Shared<Base, Mode> to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		m_counter = copy.m_counter;
		m_base = copy.m_base;

		auto& c = counter();
		c.add_strong();

		return *this;
	}
	template <typename Derived = Base>
	Shared(Shared<Derived, Mode>&& move) noexcept : m_counter(move.m_counter)
												  , m_base(move.m_base) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");
		move.m_counter = nullptr;
		move.m_base = nullptr;
	}
	template <typename Derived = Base>
	Shared& operator=(Shared<Derived, Mode>&& m) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Shared<Base, Mode> to_destroy = op::move(*this);
		m_counter = m.m_counter;
		m_base = m.m_base;
		m.m_counter = nullptr;
		m.m_base = nullptr;
		return *this;
	}
	~Shared();

	Weak<Base, Mode> downgrade() const;

	// Accessors
	OP_ALWAYS_INLINE operator NonNull<Base>() { return &value(); }
	OP_ALWAYS_INLINE operator NonNull<Base const>() const { return &value(); }
	OP_ALWAYS_INLINE operator Base*() { return &value(); }
	OP_ALWAYS_INLINE operator Base const*() const { return &value(); }
	OP_ALWAYS_INLINE operator Base&() { return value(); }
	OP_ALWAYS_INLINE operator Base const&() const { return value(); }
	OP_ALWAYS_INLINE Base* operator->() { return &value(); }
	OP_ALWAYS_INLINE Base const* operator->() const { return &value(); }
	OP_ALWAYS_INLINE Base& operator*() { return value(); }
	OP_ALWAYS_INLINE Base const& operator*() const { return value(); }

	OP_NO_DISCARD OP_ALWAYS_INLINE u32 strong() const { return counter().strong(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE u32 weak() const { return counter().weak(); }

private:
	Shared() = default;
	explicit Shared(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}

	template <typename, SMode>
	friend class Shared;

	template <typename, SMode>
	friend class Weak;

	OP_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }
	OP_ALWAYS_INLINE Base& value() const { return *m_base; }

	Counter* m_counter;
	Base* m_base;
};

template <typename Base, SMode Mode>
class Weak {
public:
	using Counter = SharedCounter<Mode>;

	template <typename Derived = Base>
	Weak(const Weak<Derived, Mode>& copy) noexcept : m_counter(copy.m_counter)
												   , m_base(copy.m_base) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");
		auto& c = counter();
		c.add_weak();
	}
	template <typename Derived = Base>
	Weak& operator=(const Weak<Derived, Mode>& copy) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Weak<Base, Mode> to_destroy = op::move(*this);
		OP_UNUSED(to_destroy);

		m_counter = copy.m_counter;
		m_base = copy.m_base;

		auto& c = counter();
		c.add_weak();

		return *this;
	}
	template <typename Derived = Base>
	Weak(Weak<Derived, Mode>&& move) noexcept : m_counter(move.m_counter)
											  , m_base(move.m_base) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");
		move.m_counter = nullptr;
		move.m_base = nullptr;
	}
	template <typename Derived = Base>
	Weak& operator=(Weak<Derived, Mode>&& m) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Weak<Base, Mode> to_destroy = op::move(*this);
		m_counter = m.m_counter;
		m_base = m.m_base;
		m.m_counter = nullptr;
		m.m_base = nullptr;
		return *this;
	}
	~Weak();

	OP_NO_DISCARD Option<Shared<Base, Mode>> upgrade() const;

	OP_NO_DISCARD OP_ALWAYS_INLINE u32 strong() const { return counter().strong(); }
	OP_NO_DISCARD OP_ALWAYS_INLINE u32 weak() const { return counter().weak(); }

private:
	Weak() = default;
	explicit Weak(Counter* counter, Base* base) : m_counter(counter), m_base(base) {}

	template <typename Derived, SMode>
	friend class Shared;

	template <typename Derived, SMode>
	friend class Weak;

	OP_ALWAYS_INLINE Counter const& counter() const { return *m_counter; }

	Counter* m_counter;
	Base* m_base;
};

template <>
class SharedCounter<SMode::NonAtomic> {
public:
	SharedCounter() = default;

	OP_ALWAYS_INLINE u32 strong() const { return m_strong; }
	OP_ALWAYS_INLINE u32 weak() const { return m_weak; }

	OP_ALWAYS_INLINE u32 add_strong() const {
		m_strong += 1;
		return m_strong - 1;
	}
	OP_ALWAYS_INLINE u32 remove_strong() const {
		m_strong -= 1;
		return m_strong + 1;
	}

	OP_ALWAYS_INLINE u32 add_weak() const {
		m_weak += 1;
		return m_weak - 1;
	}
	OP_ALWAYS_INLINE u32 remove_weak() const {
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

	OP_ALWAYS_INLINE u32 strong() const { return m_strong.load(); }
	OP_ALWAYS_INLINE u32 weak() const { return m_weak.load(); }

	OP_ALWAYS_INLINE u32 add_strong() const { return m_strong.fetch_add(1); }
	OP_ALWAYS_INLINE u32 remove_strong() const { return m_strong.fetch_sub(1); }

	OP_ALWAYS_INLINE u32 add_weak() const { return m_weak.fetch_add(1); }
	OP_ALWAYS_INLINE u32 remove_weak() const { return m_weak.fetch_sub(1); }

private:
	Atomic<u32> m_strong = 1;
	Atomic<u32> m_weak = 0;
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

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/shared.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
template <typename T>
using Shared = core::Shared<T, core::SMode::NonAtomic>;

template <typename T>
using SharedFromThis = core::SharedFromThis<T, core::SMode::NonAtomic>;

template <typename T>
using Weak = core::Weak<T, core::SMode::NonAtomic>;

template <typename T>
using AtomicShared = core::Shared<T, core::SMode::Atomic>;

template <typename T>
using AtomicSharedFromThis = core::SharedFromThis<T, core::SMode::Atomic>;

template <typename T>
using AtomicWeak = core::Weak<T, core::SMode::Atomic>;

OP_NAMESPACE_END
