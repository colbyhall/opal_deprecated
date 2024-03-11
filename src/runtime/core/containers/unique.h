// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/os/memory.h"
#include "core/type_traits.h"

OP_CORE_NAMESPACE_BEGIN

template <typename Base>
class Unique {
public:
	template <typename... Args>
	static OP_ALWAYS_INLINE Unique make(Args&&... args) {
		return Unique<Base>(Base(forward<Args>(args)...));
	}

	Unique(const Unique<Base>& copy) noexcept
		requires std::is_copy_constructible_v<Base>;
	Unique& operator=(const Unique<Base>& copy) noexcept
		requires std::is_copy_constructible_v<Base>;
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

	OP_ALWAYS_INLINE operator NonNull<Base>() { return m_ptr; }
	OP_ALWAYS_INLINE operator NonNull<Base const>() const;
	OP_ALWAYS_INLINE operator Base*() { return m_ptr; }
	OP_ALWAYS_INLINE operator Base const*() const { return m_ptr; }
	OP_ALWAYS_INLINE Base* operator->() { return m_ptr; }
	OP_ALWAYS_INLINE Base const* operator->() const { return m_ptr; }
	OP_ALWAYS_INLINE Base& operator*() { return *m_ptr; }
	OP_ALWAYS_INLINE Base const& operator*() const { return *m_ptr; }

private:
	Unique() = default;

	template <typename Derived>
	friend class Unique;

	OP_ALWAYS_INLINE explicit Unique(Base&& base) {
		void* ptr = core::malloc(core::Layout::single<Base>);
		m_ptr = new (ptr) Base(op::forward<Base>(base));
	}

	Base* m_ptr;
};

OP_CORE_NAMESPACE_END

// Include the implementation
#include "core/containers/unique.inl"

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Unique;
OP_NAMESPACE_END
