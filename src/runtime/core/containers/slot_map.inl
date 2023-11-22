// Copyright Colby Hall. All Rights Reserved.

OP_CORE_NAMESPACE_BEGIN

template <typename T>
SlotMap<T>::Key SlotMap<T>::insert(T&& value) {
	u32 index;
	constexpr u32 initial_version = 1;
	if (m_free_indices.is_empty()) {
		index = static_cast<u32>(m_elements.len());
		m_elements.push({ initial_version, op::move(value) });
	} else {
		index = m_free_indices.pop();
		m_elements[index] = { initial_version, op::move(value) };
	}
	return Key(index, m_elements[index].version);
}

template <typename T>
Option<T&> SlotMap<T>::get(const Key& key) {
	if (is_valid(key)) {
		return m_elements[key.m_index].value;
	}
	return nullopt;
}

template <typename T>
Option<T const&> SlotMap<T>::get(const Key& key) const {
	if (is_valid(key)) {
		return m_elements[key.m_index].value;
	}
	return nullopt;
}

template <typename T>
bool SlotMap<T>::is_valid(const Key& key) const {
	return key.m_index < m_elements.len() && m_elements[key.m_index].version == key.m_version;
}

template <typename T>
Option<T> SlotMap<T>::remove(const Key& key) {
	if (is_valid(key)) {
		m_free_indices.push(key.m_index);
		return op::move(m_elements[key.m_index].value);
	}
	return nullopt;
}

OP_CORE_NAMESPACE_END