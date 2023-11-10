// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/os/memory.h"
#include "core/type_traits.h"

SF_CORE_NAMESPACE_BEGIN

template <typename Base>
class Unique {
public:
	template <typename... Args>
	static SF_ALWAYS_INLINE Unique make(Args&&... args) {
		return Unique<Base>(Base(forward<Args>(args)...));
	}

	Unique(const Unique<Base>& copy) noexcept;
	Unique& operator=(const Unique<Base>& copy) noexcept;
	template <typename Derived = Base>
	Unique(Unique<Derived>&& move) noexcept : m_ptr(move.m_ptr) {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		move.m_ptr = nullptr;
	}
	template <typename Derived = Base>
	Unique& operator=(Unique<Derived>&& m) noexcept {
		static_assert(std::is_base_of_v<Base, Derived>, "Base is not a base of Derived");

		Unique<Base> to_destroy = core::move(*this);
		m_ptr = m.m_ptr;
		m.m_ptr = nullptr;
		return *this;
	}
	~Unique();

	SF_ALWAYS_INLINE operator NonNull<Base>() { return m_ptr; }
	SF_ALWAYS_INLINE operator NonNull<Base const>() const;
	SF_ALWAYS_INLINE operator Base*() { return m_ptr; }
	SF_ALWAYS_INLINE operator Base const*() const { return m_ptr; }
	SF_ALWAYS_INLINE Base* operator->() { return m_ptr; }
	SF_ALWAYS_INLINE Base const* operator->() const { return m_ptr; }
	SF_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
	SF_ALWAYS_INLINE Base const& operator*() const { return *m_ptr; }

private:
	Unique() = default;

	template <typename Derived>
	friend class Unique;

	SF_ALWAYS_INLINE explicit Unique(Base&& base) {
		void* ptr = core::malloc(core::Layout::single<Base>);
		m_ptr = new (ptr) Base(sf::forward<Base>(base));
	}

	Base* m_ptr;
};

SF_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/unique.inl"

// Export to sf namespace
SF_NAMESPACE_BEGIN
using core::Unique;
SF_NAMESPACE_END
