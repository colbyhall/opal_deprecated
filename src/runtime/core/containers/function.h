// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Include the implementation early
#include "core/containers/function.inl"

OP_CORE_NAMESPACE_BEGIN

template <typename F>
class Function;

template <typename T>
inline constexpr bool is_function = false;
template <typename T>
inline constexpr bool is_function<Function<T>> = true;

template <typename F>
class FunctionRef;

template <typename T>
inline constexpr bool is_function_ref = false;
template <typename T>
inline constexpr bool is_function_ref<FunctionRef<T>> = true;

template <typename F>
class FunctionRef final : public hidden::FunctionBase<hidden::RefStorage, F> {
	using Super = hidden::FunctionBase<hidden::RefStorage, F>;

public:
	using Result = typename Super::Result;

	template <
		typename Functor,
		typename = std::enable_if_t<
			!is_function_ref<std::decay_t<Functor>> && hidden::func_can_bind_to_functor<F, std::decay_t<Functor>>>>
	FunctionRef(Functor&& f) : Super(op::forward<Functor>(f)) {}

	FunctionRef(const FunctionRef&) = default;
	FunctionRef& operator=(const FunctionRef&) const = delete;

	~FunctionRef() = default;
};

template <typename F>
class Function final : public hidden::FunctionBase<hidden::UniqueStorage, F> {
	using Super = hidden::FunctionBase<hidden::UniqueStorage, F>;

public:
	using Result = typename Super::Result;

	template <
		typename Functor,
		typename = std::enable_if_t<
			!is_function<std::decay_t<Functor>> && hidden::func_can_bind_to_functor<F, std::decay_t<Functor>>>>
	Function(Functor&& f) : Super(op::forward<Functor>(f)) {}

	Function(Function&& move) noexcept = default;

	~Function() = default;
};

OP_CORE_NAMESPACE_END

// Export to op namespace
OP_NAMESPACE_BEGIN
using core::Function;
using core::FunctionRef;
OP_NAMESPACE_END
