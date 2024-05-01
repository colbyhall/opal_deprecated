// Copyright Colby Hall. All Rights Reserved.

#include "core/os/thread.h"
#include "core/containers/option.h"

// Section for platform independent code
OP_CORE_NAMESPACE_BEGIN

thread_local Option<Thread> this_thread;

OP_CORE_NAMESPACE_END

// Section for windows code
#if OP_PLATFORM_WINDOWS

	#include "core/os/windows.h"

OP_CORE_NAMESPACE_BEGIN

struct ThreadFunction {
	Function<int()> function;
	Thread thread; // We send the thread object to the actual thread for the this_thread local so theres only one
				   // instance per thread.
};

static DWORD WINAPI thread_function(LPVOID lpParam) noexcept {
	auto* const thread_function = static_cast<ThreadFunction*>(lpParam);
	this_thread = op::move(thread_function->thread);

	const int result = (thread_function->function)();

	thread_function->~ThreadFunction();
	core::free(thread_function);

	return static_cast<DWORD>(result);
}

Thread Thread::spawn(Function<int()> f) {
	void* ptr = core::malloc(core::Layout::single<ThreadFunction>);

	DWORD thread_id;
	HANDLE handle = ::CreateThread(nullptr, 0, &thread_function, ptr, CREATE_SUSPENDED, &thread_id);
	OP_ASSERT(handle != nullptr);

	auto result = Thread{ thread_id, handle };
	new (ptr) ThreadFunction{ .function = op::move(f), .thread = result };

	::ResumeThread(handle);

	return result;
}

Thread Thread::current() {
	// Lazily cache the current thread in a thread local global
	if (!this_thread.is_set()) {
		const HANDLE handle = ::GetCurrentThread();
		const DWORD id = ::GetCurrentThreadId();
		this_thread = Thread{ id, handle };
	}
	return this_thread.as_ref().unwrap();
}

// Copied from
// https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2015/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2015&redirectedfrom=MSDN
const DWORD MS_VC_EXCEPTION = 0x406D1388;
	#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO {
	DWORD dwType;	  // Must be 0x1000.
	LPCSTR szName;	  // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags;	  // Reserved for future use, must be zero.
} THREADNAME_INFO;
	#pragma pack(pop)
void SetThreadName(DWORD dwThreadID, const char* threadName) {
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
	#pragma warning(push)
	#pragma warning(disable : 6320 6322)
	__try {
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
	}
	#pragma warning(pop)
}

void Thread::set_name(String&& name) {
	m_inner->name = op::move(name);
	SetThreadName(static_cast<DWORD>(id()), *m_inner->name.as_ref().unwrap());
}

void Thread::join() { ::WaitForSingleObject(m_inner->handle, INFINITE); }

OP_CORE_NAMESPACE_END

#endif
