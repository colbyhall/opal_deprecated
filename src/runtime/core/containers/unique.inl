// Copyright Colby Hall. All Rights Reserved.

GJ_CORE_NAMESPACE_BEGIN

template <typename Base>
Unique<Base>::Unique(const Unique<Base>& copy) noexcept {
	static_assert(
		!std::is_abstract_v<Base>,
		"Can ony perform copy with a concrete class"
	);
	void* ptr = core::malloc(core::Layout::single<Base>);
	m_ptr = new (ptr) Base(*copy);
}

template <typename Base>
Unique<Base>& Unique<Base>::operator=(const Unique<Base>& copy) noexcept {
	static_assert(
		!std::is_abstract_v<Base>,
		"Can ony perform copy with a concrete class"
	);
	Unique<Base> to_destroy = core::move(*this);
	GJ_UNUSED(to_destroy);

	void* ptr = core::malloc(core::Layout::single<Base>);
	m_ptr = new (ptr) Base(*copy);

	return *this;
}

template <typename Base>
GJ_ALWAYS_INLINE Unique<Base>::operator NonNull<Base const>() const {
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

GJ_CORE_NAMESPACE_END
