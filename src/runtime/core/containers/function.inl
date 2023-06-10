// Copyright Colby Hall. All Rights Reserved.

#include "core/non_copyable.h"
#include "core/os/memory.h"

EU_CORE_NAMESPACE_BEGIN
EU_HIDDEN_NAMESPACE_BEGIN

template <typename Functor, typename FuncType>
struct FunctionRefCaller;

template <typename Functor, typename R, typename... P>
struct FunctionRefCaller<Functor, R(P...)> {
	static R call(void* obj, P&... params) { return std::invoke(*(Functor*)obj, eu::move(params)...); }
};

template <typename Functor, typename... P>
struct FunctionRefCaller<Functor, void(P...)> {
	static void call(void* obj, P&... params) { std::invoke(*(Functor*)obj, eu::move(params)...); }
};

template <typename S, typename F>
class FunctionBase;

template <typename S, typename R, typename... Param>
class FunctionBase<S, R(Param...)> : private NonCopyable {
	template <typename OtherS, typename OtherF>
	friend class FunctionBase;

public:
	using Result = R;

	FunctionBase() = delete;

	// clang-format off
	template <
	    typename F,
		typename = std::enable_if_t<
		    !std::is_same_v<FunctionBase, std::decay_t<F>>
		>
	>
	FunctionBase(F&& f) {
		if (auto* binding = m_storage.bind(eu::forward<F>(f))) {
			EU_UNUSED(binding);
			using DecayedFunctorType = std::remove_pointer_t<decltype(binding)>;
			m_callable = &FunctionRefCaller<DecayedFunctorType, R(Param...)>::call;
		}
	}
	// clang-format on

	FunctionBase(FunctionBase&& other) noexcept : m_callable(other.m_callable), m_storage(eu::move(other.m_storage)) {
		if (m_callable) {
			other.m_callable = nullptr;
		}
	}
	FunctionBase& operator=(FunctionBase&& other) noexcept {
		auto to_destroy = eu::move(*this);
		EU_UNUSED(to_destroy);

		m_callable = eu::move(other.m_callable);
		other.m_callable = nullptr;

		m_storage = eu::move(other.m_storage);
	}

	R operator()(Param... params) const {
		EU_ASSERT(m_callable);
		return (m_callable)(m_storage.ptr(), params...);
	}

private:
	R (*m_callable)(void*, Param&...);
	S m_storage;
};

struct RefStorage final : NonCopyable {
	RefStorage() = default;
	RefStorage(RefStorage&& s) noexcept : m_ptr(s.m_ptr) { s.m_ptr = nullptr; }
	RefStorage& operator=(RefStorage&& s) noexcept {
		m_ptr = s.m_ptr;
		s.m_ptr = nullptr;
	}

	// ~Begin Storage Interface
	template <typename F>
	std::remove_reference_t<F>* bind(F&& f) {
		m_ptr = (void*)&f;
		return &f;
	}
	void* ptr() const { return m_ptr; }
	// ~End Storage Interface

private:
	void* m_ptr = nullptr;
};

struct FunctionWrapperInterface {
	virtual void* ptr() = 0;
	virtual ~FunctionWrapperInterface() = default;
};

template <typename T>
struct FunctionWrapper : public FunctionWrapperInterface {
	template <typename... A>
	explicit FunctionWrapper(A&&... args) : t(eu::forward<A>(args)...) {}
	~FunctionWrapper() override = default;

	void* ptr() override { return &t; }

	T t;
};

struct UniqueStorage : NonCopyable {
	UniqueStorage() = default;
	UniqueStorage(UniqueStorage&& s) noexcept : m_ptr(s.m_ptr) { s.m_ptr = nullptr; }
	UniqueStorage& operator=(UniqueStorage&& s) noexcept {
		m_ptr = s.m_ptr;
		s.m_ptr = nullptr;
	}
	~UniqueStorage() {
		if (m_ptr) {
			auto* f = static_cast<FunctionWrapperInterface*>(m_ptr);
			f->~FunctionWrapperInterface();
			core::free(m_ptr);
			m_ptr = nullptr;
		}
	}

	// ~Begin Storage Interface
	template <typename F>
	std::remove_reference_t<F>* bind(F&& f) {
		void* memory = core::malloc(core::Layout::single<FunctionWrapper<F>>);
		auto* result = new (memory) FunctionWrapper<F>(eu::forward<F>(f));
		m_ptr = memory;

		return (std::remove_reference_t<F>*)result->ptr();
	}
	void* ptr() const {
		auto* f = (FunctionWrapperInterface*)m_ptr;
		return f->ptr();
	}
	// ~End Storage Interface

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

EU_HIDDEN_NAMESPACE_END
EU_CORE_NAMESPACE_END