// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/option.h"

OP_SUPPRESS_WARNINGS_STD_BEGIN
#include <atomic>
OP_SUPPRESS_WARNINGS_STD_END

OP_CORE_NAMESPACE_BEGIN

enum class Order : u8 { Relaxed, Release, Acquire, AcqRel, SeqCst };

template <typename T>
class Atomic {
public:
	Atomic() noexcept = default;
	constexpr Atomic(T desired) noexcept : m_atomic(desired) {}
	OP_ALWAYS_INLINE Atomic(Atomic&& rhs) noexcept;
	OP_ALWAYS_INLINE Atomic& operator=(Atomic&& rhs) noexcept;

	OP_ALWAYS_INLINE void store(T desired, Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T load(Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T exchange(T desired, Order order = Order::SeqCst) const noexcept;

	OP_NO_DISCARD OP_ALWAYS_INLINE Option<T>
	compare_exchange_weak(T expected, T desired, Order order = Order::SeqCst) const noexcept;

	OP_NO_DISCARD OP_ALWAYS_INLINE Option<T>
	compare_exchange_strong(T expected, T desired, Order order = Order::SeqCst) const noexcept;

	OP_NO_DISCARD OP_ALWAYS_INLINE T fetch_add(T arg, Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T fetch_sub(T arg, Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T fetch_and(T arg, Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T fetch_or(T arg, Order order = Order::SeqCst) const noexcept;
	OP_NO_DISCARD OP_ALWAYS_INLINE T fetch_xor(T arg, Order order = Order::SeqCst) const noexcept;

private:
	OP_ALWAYS_INLINE std::memory_order to_std(Order order) const {
		static const std::memory_order convert[] = { std::memory_order_relaxed,
													 std::memory_order_release,
													 std::memory_order_acquire,
													 std::memory_order_acq_rel,
													 std::memory_order_seq_cst };
		return convert[(u8)order];
	}

	mutable std::atomic<T> m_atomic;
};

OP_CORE_NAMESPACE_END

#include "atomic.inl"

OP_NAMESPACE_BEGIN
using core::Atomic;
OP_NAMESPACE_END
