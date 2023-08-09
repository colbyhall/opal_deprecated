// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
GJ_ALWAYS_INLINE Atomic<T>::Atomic(Atomic&& rhs) noexcept
	: m_atomic(rhs.m_atomic.load(std::memory_order_relaxed)) {}

template <typename T>
GJ_ALWAYS_INLINE Atomic<T>& Atomic<T>::operator=(Atomic&& rhs) noexcept {
	const auto order = std::memory_order_relaxed;
	m_atomic.store(rhs.m_atomic.load(order), order);
}

template <typename T>
GJ_ALWAYS_INLINE void Atomic<T>::store(T desired, Order order) const noexcept {
	m_atomic.store(desired, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T Atomic<T>::load(Order order) const noexcept {
	return m_atomic.load(to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::exchange(T desired, Order order) const noexcept {
	return m_atomic.exchange(desired, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE Option<T>
Atomic<T>::compare_exchange_weak(T expected, T desired, Order order)
	const noexcept {
	if (m_atomic.compare_exchange_weak(expected, desired, to_std(order))) {
		return expected;
	}
	return Option<T>{};
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE Option<T>
Atomic<T>::compare_exchange_strong(T expected, T desired, Order order)
	const noexcept {
	if (m_atomic.compare_exchange_strong(expected, desired, to_std(order))) {
		return expected;
	}
	return Option<T>{};
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::fetch_add(T arg, Order order) const noexcept {
	return m_atomic.fetch_add(arg, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::fetch_sub(T arg, Order order) const noexcept {
	return m_atomic.fetch_sub(arg, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::fetch_and(T arg, Order order) const noexcept {
	return m_atomic.fetch_and(arg, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::fetch_or(T arg, Order order) const noexcept {
	return m_atomic.fetch_or(arg, to_std(order));
}

template <typename T>
GJ_NO_DISCARD GJ_ALWAYS_INLINE T
Atomic<T>::fetch_xor(T arg, Order order) const noexcept {
	return m_atomic.fetch_xor(arg, to_std(order));
}

GJ_CORE_NAMESPACE_END
