// Copyright Colby Hall. All Rights Reserved.

SF_CORE_NAMESPACE_BEGIN

template <typename Base, SMode Mode>
Shared<Base, Mode>::~Shared() {
	if (m_counter) {
		auto& c = counter();

		// Decrement the strong count
		const auto strong_count = c.remove_strong();
		const auto weak_count = c.weak();

		// If there are no strong references deconstruct the object
		if (strong_count == 1) {
			m_base->~Base();

			// Base that are derived from SharedFromThis will have a weak ptr to itself. Free when weak ptr is 1
			// to account for this
			if constexpr (std::is_base_of_v<SharedFromThisBase, Base>) {
				if (weak_count == 1) {
					core::free(m_counter);
				}
			}
			// Free the memory if we have no weak references
			else {
				if (weak_count == 0) {
					core::free(m_counter);
				}
			}

			m_counter = nullptr;
			m_base = nullptr;
		}
	}
}

template <typename Base, SMode Mode>
Weak<Base, Mode> Shared<Base, Mode>::downgrade() const {
	auto& c = counter();
	c.add_weak();
	return Weak<Base, Mode>(m_counter, m_base);
}

template <typename Base, SMode Mode>
Weak<Base, Mode>::~Weak() {
	if (m_counter) {
		auto& c = counter();

		const auto strong_count = c.strong();
		const auto weak_count = c.remove_weak();

		if (strong_count == 0 && weak_count == 0) {
			core::free(m_counter);
			m_counter = nullptr;
		}
	}
}

template <typename Base, SMode Mode>
Option<Shared<Base, Mode>> Weak<Base, Mode>::upgrade() const {
	auto& c = counter();
	const auto strong_count = c.strong();
	if (strong_count > 0) {
		c.add_strong();
		return Shared<Base, Mode>(m_counter, m_base);
	}
	return nullptr;
}

template <typename T, SMode Mode>
Shared<T, Mode> SharedFromThis<T, Mode>::to_shared() const {
	return m_this.as_ref().unwrap().upgrade().unwrap();
}

SF_CORE_NAMESPACE_END
