// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/memory.h"
#include "core/non_copyable.h"
#include "core/type_traits.h"

EU_CORE_NAMESPACE_BEGIN

template <typename Base>
class Unique : private NonCopyable {
public:
	EU_ALWAYS_INLINE operator NonNull<Base>() { return m_ptr; }
	EU_ALWAYS_INLINE operator NonNull<Base const>() const;
	EU_ALWAYS_INLINE operator Base*() { return m_ptr; }
	EU_ALWAYS_INLINE operator Base const*() const { return m_ptr; }
	EU_ALWAYS_INLINE Base* operator->() { return m_ptr; }
	EU_ALWAYS_INLINE Base const* operator->() const { return m_ptr; }
	EU_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
	EU_ALWAYS_INLINE Base const& operator*() const { return *m_ptr; }

	template <typename Derived = Base>
	Unique(Unique<Derived>&& move) noexcept : m_ptr(move.m_ptr) {
		static_assert(core::is_base_of<Base, Derived>,
					  "Base is not a base of Derived");

		move.m_ptr = nullptr;
	}
	template <typename Derived = Base>
	Unique& operator=(Unique<Derived>&& m) noexcept {
		static_assert(core::is_base_of<Base, Derived>,
					  "Base is not a base of Derived");

		Unique<Base> to_destroy = core::move(*this);
		m_ptr = m.m_ptr;
		m.m_ptr = nullptr;
		return *this;
	}
	~Unique();

private:
	Unique() = default;

	template <typename T, typename... Args>
	friend Unique<T> make_unique(Args&&... args);

	template <typename Derived>
	friend class Unique;

	EU_ALWAYS_INLINE explicit Unique(Base&& base) {
		void* ptr = core::malloc(core::Layout::single<Base>);
		m_ptr = new (ptr) Base(eu::forward<Base>(base));
	}

	Base* m_ptr;
};

template <typename T, typename... Args>
EU_ALWAYS_INLINE Unique<T> make_unique(Args&&... args) {
	return Unique<T>(eu::move(T(forward<Args>(args)...)));
}

EU_CORE_NAMESPACE_END

#include "core/containers/unique.inl"

EU_NAMESPACE_BEGIN

using core::make_unique;
using core::Unique;

EU_NAMESPACE_END
