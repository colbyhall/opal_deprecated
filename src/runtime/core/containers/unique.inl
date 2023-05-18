// Copyright Colby Hall. All Rights Reserved.

EU_CORE_NAMESPACE_BEGIN

template <typename Base>
EU_ALWAYS_INLINE Unique<Base>::operator NonNull<Base const>() const {
	return m_ptr;
}

template <typename Base>
Unique<Base>::~Unique() {
	if (m_ptr) {
		m_ptr->~Base();
		core::free(m_ptr);
		m_ptr = nullptr;
	}
}

EU_CORE_NAMESPACE_END
