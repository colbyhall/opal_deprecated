// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include "core/containers/function.h"
#include "core/containers/shared.h"
#include "core/containers/string.h"

OP_CORE_NAMESPACE_BEGIN

/**
 * Represents a thread of execution.
 *
 * Provides functionality for creating and managing operating system threads.
 */
class Thread {
public:
	using Id = u64;

	/**
	 * Spawns a new thread with the specified function.
	 *
	 * This static member function creates a new thread that executes the specified function.
	 * The function should have the signature `int functionName()`.
	 *
	 * @param f The function to be executed by the new thread.
	 * @return The object of the newly created Thread.
	 */
	static Thread spawn(Function<int()> f);

	/**
	 * Retrieves the current thread.
	 * @return The Thread object representing the current thread.
	 */
	static Thread current();

	/**
	 * Sets the name of the thread.
	 *
	 * @safety This function is not thread safe
	 *
	 * @param name The name of the thread.
	 */
	void set_name(String&& name);

	/**
	 * Blocks the current thread until this thread has completed.
	 */
	void join();

	/**
	 * Retrieves the identifier of the thread.
	 * @return The identifier of the thread.
	 */
	OP_ALWAYS_INLINE Id id() const { return m_inner->id; }

	OP_ALWAYS_INLINE bool operator==(const Thread& rhs) const { return m_inner->id == rhs.m_inner->id; }
	OP_ALWAYS_INLINE bool operator!=(const Thread& rhs) const { return m_inner->id != rhs.m_inner->id; }

private:
	explicit Thread(Id id, void* handle) : m_inner(Shared<Inner, SMode::Atomic>::make(id, handle, nullopt)) {}

	struct Inner {
		Id id;
		void* handle;
		Option<String> name;
	};
	Shared<Inner, SMode::Atomic> m_inner;

};

OP_CORE_NAMESPACE_END
