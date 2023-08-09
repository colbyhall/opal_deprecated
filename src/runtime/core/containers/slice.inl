// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename T>
Slice<T>& Slice<T>::operator=(const Slice<T>& c) {
	m_ptr = c.m_ptr;
	m_len = c.m_len;
	return *this;
}

template <typename T>
Slice<T>::Slice(Slice<T>&& m) noexcept : m_ptr(m.m_ptr)
									   , m_len(m.m_len) {
	m.m_ptr = nullptr;
	m.m_len = 0;
}

template <typename T>
Slice<T>& Slice<T>::operator=(Slice<T>&& m) noexcept {
	m_ptr = m.m_ptr;
	m_len = m.m_len;

	m.m_ptr = nullptr;
	m.m_len = 0;

	return *this;
}

template <typename T>
Slice<T const>& Slice<T const>::operator=(const Slice<T const>& c) {
	m_ptr = c.m_ptr;
	m_len = c.m_len;
	return *this;
}

template <typename T>
Slice<T const>::Slice(Slice<T const>&& m) noexcept
	: m_ptr(m.m_ptr)
	, m_len(m.m_len) {
	m.m_ptr = nullptr;
	m.m_len = 0;
}

template <typename T>
Slice<T const>& Slice<T const>::operator=(Slice<T const>&& m) noexcept {
	m_ptr = m.m_ptr;
	m_len = m.m_len;

	m.m_ptr = nullptr;
	m.m_len = 0;

	return *this;
}

GJ_CORE_NAMESPACE_END
