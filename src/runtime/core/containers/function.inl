// Copyright Colby Hall. All Rights Reserved.

#include "core/os/memory.h"

SF_CORE_NAMESPACE_BEGIN
SF_HIDDEN_NAMESPACE_BEGIN

template <typename Functor, typename FuncType>
struct FunctionRefCaller;

template <typename Functor, typename R, typename... P>
struct FunctionRefCaller<Functor, R(P...)> {
	static R call(void* obj, P&... params) { return std::invoke(*(Functor*)obj, sf::forward<P>(params)...); }
};

template <typename Functor, typename... P>
struct FunctionRefCaller<Functor, void(P...)> {
	static void call(void* obj, P&... params) { std::invoke(*(Functor*)obj, sf::forward<P>(params)...); }
};

template <typename S, typename F>
class FunctionBase;

template <typename S, typename R, typename... Param>
class FunctionBase<S, R(Param...)> {
	template <typename OtherS, typename OtherF>
	friend class FunctionBase;

public:
	using Result = R;

	FunctionBase() = delete;

	template <typename F, typename = std::enable_if_t<!std::is_same_v<FunctionBase, std::decay_t<F>>>>
	FunctionBase(F&& f) {
		if (auto* binding = m_storage.bind(sf::forward<F>(f))) {
			SF_UNUSED(binding);
			using DecayedFunctor = std::remove_pointer_t<decltype(binding)>;
			m_callable = &FunctionRefCaller<DecayedFunctor, R(Param...)>::call;
		}
	}

	FunctionBase(FunctionBase&& other) noexcept : m_callable(other.m_callable), m_storage(sf::move(other.m_storage)) {
		if (m_callable) {
			other.m_callable = nullptr;
		}
	}
	FunctionBase& operator=(FunctionBase&& other) noexcept {
		auto to_destroy = sf::move(*this);
		SF_UNUSED(to_destroy);

		m_callable = sf::move(other.m_callable);
		other.m_callable = nullptr;

		m_storage = sf::move(other.m_storage);
	}

	R operator()(Param... params) const {
		SF_ASSERT(m_callable);
		return (m_callable)(m_storage.ptr(), params...);
	}

private:
	R (*m_callable)(void*, Param&...);
	S m_storage;
};

struct RefStorage {
	RefStorage() = default;
	RefStorage(RefStorage&& s) noexcept : m_ptr(s.m_ptr) { s.m_ptr = nullptr; }
	RefStorage& operator=(RefStorage&& s) noexcept {
		m_ptr = s.m_ptr;
		s.m_ptr = nullptr;
	}

	// IStorage
	template <typename F>
	std::remove_reference_t<F>* bind(F&& f) {
		m_ptr = (void*)&f;
		return &f;
	}
	void* ptr() const { return m_ptr; }
	// ~IStorage

private:
	void* m_ptr = nullptr;
};

struct IFunctionWrapper {
	virtual void* ptr() = 0;
	virtual ~IFunctionWrapper() = default;
};

template <typename T>
struct FunctionWrapper : public IFunctionWrapper {
	template <typename... A>
	explicit FunctionWrapper(A&&... args) : t(sf::forward<A>(args)...) {}
	~FunctionWrapper() override = default;

	void* ptr() override { return &t; }

	T t;
};

struct UniqueStorage {
	UniqueStorage() = default;
	UniqueStorage(UniqueStorage&& s) noexcept : m_ptr(s.m_ptr) { s.m_ptr = nullptr; }
	UniqueStorage& operator=(UniqueStorage&& s) noexcept {
		m_ptr = s.m_ptr;
		s.m_ptr = nullptr;
	}
	~UniqueStorage() {
		if (m_ptr) {
			auto* f = static_cast<IFunctionWrapper*>(m_ptr);
			f->~IFunctionWrapper();
			core::free(m_ptr);
			m_ptr = nullptr;
		}
	}

	// ~IStorage
	template <typename F>
	std::remove_reference_t<F>* bind(F&& f) {
		void* memory = core::malloc(core::Layout::single<FunctionWrapper<F>>);
		auto* result = new (memory) FunctionWrapper<F>(sf::forward<F>(f));
		m_ptr = memory;

		return (std::remove_reference_t<F>*)result->ptr();
	}
	void* ptr() const {
		auto* f = (IFunctionWrapper*)m_ptr;
		return f->ptr();
	}
	// ~IStorage

private:
	void* m_ptr = nullptr;
};

template <typename T>
T&& declval();

template <typename F, typename R, typename... P>
inline constexpr bool functor_return_type_is_compatible =
	std::is_constructible_v<R, decltype(declval<F>()(declval<P>()...))>;
template <typename MR, typename Class, typename R, typename... P>
inline constexpr bool functor_return_type_is_compatible<MR Class::*, R, P...> = std::is_constructible_v<R, MR>;
template <typename MR, typename Class, typename R, typename... P>
inline constexpr bool functor_return_type_is_compatible<MR Class::*const, R, P...> = std::is_constructible_v<R, MR>;
template <typename MR, typename Class, typename... MP, typename R, typename... P>
inline constexpr bool functor_return_type_is_compatible<MR (Class::*)(MP...), R, P...> = std::is_constructible_v<R, MR>;
template <typename MR, typename Class, typename... MP, typename R, typename... P>
inline constexpr bool functor_return_type_is_compatible<MR (Class::*)(MP...) const, R, P...> =
	std::is_constructible_v<R, MR>;

template <typename A, typename B>
inline constexpr bool func_can_bind_to_functor = false;

template <typename F, typename R, typename... P>
inline constexpr bool func_can_bind_to_functor<R(P...), F> =
	std::is_invocable_v<F, P...> && functor_return_type_is_compatible<F, R, P...>;

template <typename F, typename... P>
inline constexpr bool func_can_bind_to_functor<void(P...), F> = std::is_invocable_v<F, P...>;

SF_HIDDEN_NAMESPACE_END
SF_CORE_NAMESPACE_END
