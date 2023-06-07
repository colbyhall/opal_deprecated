// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define EU_PLATFORM_WINDOWS_UWP
	#endif
	#define EU_PLATFORM_WINDOWS
#else
	#error Unsupported platform
#endif

// Determine compiler
#if defined(__clang__)
	#define EU_COMPILER_CLANG
#elif defined(_MSC_VER)
	#define EU_COMPILER_MSVC
#else
	#error Unsupported compiler
#endif

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define EU_CPU_X86
	#if defined(__x86_64__) || defined(_M_X64)
		#define EU_CPU_ADDRESS_BITS 64
	#else
		#define EU_CPU_ADDRESS_BITS 32
	#endif
#else
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#ifdef EU_COMPILER_CLANG
	#define EU_PRAGMA(x)				 _Pragma(#x)
	#define EU_SUPPRESS_WARNING_PUSH	 EU_PRAGMA(clang diagnostic push)
	#define EU_SUPPRESS_WARNING_POP		 EU_PRAGMA(clang diagnostic pop)
	#define EU_CLANG_SUPPRESS_WARNING(w) EU_PRAGMA(clang diagnostic ignored w)
#else
	#define EU_CLANG_SUPPRESS_WARNING(w)
#endif

#ifdef EU_COMPILER_MSVC
	#define EU_PRAGMA(x)				__pragma(x)
	#define EU_SUPPRESS_WARNING_PUSH	EU_PRAGMA(warning(push))
	#define EU_SUPPRESS_WARNING_POP		EU_PRAGMA(warning(pop))
	#define EU_MSVC_SUPPRESS_WARNING(w) EU_PRAGMA(warning(disable : w))
#else
	#define EU_MSVC_SUPPRESS_WARNING(w)
#endif

// Define inline macro
#if defined(EU_COMPILER_CLANG)
	#define EU_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#elif defined(EU_COMPILER_MSVC)
	#define EU_ALWAYS_INLINE __forceinline
#else
	#error Undefined inline
#endif

#define EU_NO_DISCARD [[nodiscard]]

// Cache line size (used for aligning to cache line)
#ifndef EU_CACHE_LINE_SIZE
	#define EU_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if defined(EU_COMPILER_CLANG) || defined(COMPILER_GCC)
	#define EU_FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(EU_COMPILER_MSVC)
	#define EU_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined
#endif

// OS-specific includes
#if defined(EU_PLATFORM_WINDOWS)
	#define EU_BREAKPOINT __debugbreak()
#else
	#error Unknown platform
#endif

#ifdef _DEBUG
	#define EU_BUILD_DEBUG
#endif

#define EU_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define EU_UNUSED(x) (void)x

// Crashes the application
#define EU_CRASH                                                                                                       \
	EU_SUPPRESS_WARNING_PUSH                                                                                           \
	EU_MSVC_SUPPRESS_WARNING(6011)                                                                                     \
	do {                                                                                                               \
		int* _ptr = nullptr;                                                                                           \
		*_ptr = 0;                                                                                                     \
	} while (false) EU_SUPPRESS_WARNING_POP

// Suppress warnings generated by the standard template library
#define EU_SUPPRESS_WARNINGS_STD_BEGIN                                                                                 \
	EU_SUPPRESS_WARNING_PUSH                                                                                           \
	EU_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5262)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5219)

#define EU_SUPPRESS_WARNINGS_STD_END EU_SUPPRESS_WARNING_POP

// Disable common warnings triggered by core when compiling with -Wall
#define EU_SUPPRESS_WARNINGS                                                                                           \
	EU_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                                        \
	EU_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                                               \
	EU_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                                         \
	EU_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                                     \
	EU_CLANG_SUPPRESS_WARNING("-Wold-style-cast")                                                                      \
	EU_CLANG_SUPPRESS_WARNING("-Wgnu-anonymous-struct")                                                                \
	EU_CLANG_SUPPRESS_WARNING("-Wnested-anon-types")                                                                   \
	EU_CLANG_SUPPRESS_WARNING("-Wglobal-constructors")                                                                 \
	EU_CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")                                                               \
	EU_CLANG_SUPPRESS_WARNING("-Wnonportable-system-include-path")                                                     \
	EU_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                                            \
	EU_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                                    \
	EU_CLANG_SUPPRESS_WARNING("-Wformat-nonliteral")                                                                   \
	EU_CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")                                                              \
	EU_CLANG_SUPPRESS_WARNING("-Wcast-align")                                                                          \
	EU_CLANG_SUPPRESS_WARNING("-Winvalid-offsetof")                                                                    \
	EU_CLANG_SUPPRESS_WARNING("-Wgnu-zero-variadic-macro-arguments")                                                   \
	EU_CLANG_SUPPRESS_WARNING("-Wdocumentation-unknown-command")                                                       \
	EU_CLANG_SUPPRESS_WARNING("-Wctad-maybe-unsupported")                                                              \
	EU_CLANG_SUPPRESS_WARNING("-Wdeprecated-copy")                                                                     \
	EU_CLANG_SUPPRESS_WARNING("-Wimplicit-int-float-conversion")                                                       \
                                                                                                                       \
	EU_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4100)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4626)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5027)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4324)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4625)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5026)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4623)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4201)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4371)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5045)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4583)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4582)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5219)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4826)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	EU_MSVC_SUPPRESS_WARNING(4127)

// Begin the eu namespace
#define EU_NAMESPACE_BEGIN                                                                                             \
	EU_SUPPRESS_WARNING_PUSH                                                                                           \
	EU_SUPPRESS_WARNINGS                                                                                               \
	namespace eu {

// End the eu namespace
#define EU_NAMESPACE_END                                                                                               \
	}                                                                                                                  \
	EU_SUPPRESS_WARNING_POP

// Begin the eu::core namespace
#define EU_CORE_NAMESPACE_BEGIN                                                                                        \
	EU_NAMESPACE_BEGIN                                                                                                 \
	namespace core {

// End the eu::core namespace
#define EU_CORE_NAMESPACE_END                                                                                          \
	}                                                                                                                  \
	EU_NAMESPACE_END

#define EU_HIDDEN_NAMESPACE_BEGIN namespace hidden {

#define EU_HIDDEN_NAMESPACE_END }

// Declare primitive types under core namespace
EU_CORE_NAMESPACE_BEGIN

using u8 = unsigned char;
constexpr u8 U8_MIN = 0;
constexpr u8 U8_MAX = 0xff;

using u16 = unsigned short;
constexpr u16 U16_MIN = 0;
constexpr u16 U16_MAX = 0xffff;

using u32 = unsigned int;
constexpr u32 U32_MIN = 0;
constexpr u32 U32_MAX = 0xffffffff;

using u64 = unsigned long long;
constexpr u64 U64_MIN = 0;
constexpr u64 U64_MAX = 0xffffffffffffffff;

using usize = u64;
constexpr usize USIZE_MIN = U64_MIN;
constexpr usize USIZE_MAX = U64_MAX;

static_assert(sizeof(u8) == 1, "u8 should only be 1 byte");
static_assert(sizeof(u16) == 2, "u16 should only be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 should only be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 should only be 8 bytes");
static_assert(sizeof(usize) == 8, "usize should only be 8 bytes");

using i8 = signed char;
constexpr i8 I8_MIN = (-127 - 1);
constexpr i8 I8_MAX = 127;

using i16 = short;
constexpr i16 I16_MIN = (-32767 - 1);
constexpr i16 I16_MAX = 32767;

using i32 = int;
constexpr i32 I32_MIN = (-2147483647 - 1);
constexpr i32 I32_MAX = 2147483647;

using i64 = long long;
constexpr i64 I64_MIN = (-9223372036854775807 - 1);
constexpr i64 I64_MAX = 9223372036854775807;

using isize = i64;
constexpr isize ISIZE_MIN = I64_MIN;
constexpr isize ISIZE_MAX = I64_MAX;

static_assert(sizeof(i8) == 1, "i8 should only be 1 byte");
static_assert(sizeof(i16) == 2, "i16 should only be 2 bytes");
static_assert(sizeof(i32) == 4, "i32 should only be 4 bytes");
static_assert(sizeof(i64) == 8, "i64 should only be 8 bytes");
static_assert(sizeof(isize) == 8, "isize should only be 8 bytes");

using f32 = float;
constexpr f32 F32_MIN = 1.17549435e-38F;
constexpr f32 F32_MAX = 3.40282347e+38F;

using f64 = double;

static_assert(sizeof(f32) == 4, "f32 should only be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 should only be 8 bytes");

using NullPtr = decltype(nullptr);

using wchar = wchar_t;

EU_CORE_NAMESPACE_END

// Export primitive types to eu namespace
EU_NAMESPACE_BEGIN

using core::f32;
using core::f64;
using core::i16;
using core::i32;
using core::i64;
using core::i8;
using core::isize;
using core::NullPtr;
using core::u16;
using core::u32;
using core::u64;
using core::u8;
using core::usize;
using core::wchar;

EU_NAMESPACE_END

// Turn on asserts if compiled in debug
#if defined(_DEBUG)
	#define EU_ENABLE_ASSERTS
#endif

#ifdef EU_ENABLE_ASSERTS

EU_CORE_NAMESPACE_BEGIN
bool _assert_failed(bool must_crash, const char* expression, const char* message, const char* file, u32 line);

struct AssertLastParam {};
EU_ALWAYS_INLINE bool _assert_failed_helper(bool must_crash, const char* expression, const char* file, u32 line,
											AssertLastParam) {
	return _assert_failed(must_crash, expression, nullptr, file, line);
}
EU_ALWAYS_INLINE bool _assert_failed_helper(bool must_crash, const char* expression, const char* file, u32 line,
											const char* message, AssertLastParam) {
	return _assert_failed(must_crash, expression, message, file, line);
}
EU_CORE_NAMESPACE_END

	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define EU_ASSERT(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression) && eu::core::_assert_failed_helper(true, #expression, __FILE__, eu::u32(__LINE__),       \
																 ##__VA_ARGS__, eu::core::AssertLastParam())) {        \
				EU_CRASH;                                                                                              \
			}                                                                                                          \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define EU_ENSURE(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression) && eu::core::_assert_failed_helper(false, #expression, __FILE__, eu::u32(__LINE__),      \
																 ##__VA_ARGS__, eu::core::AssertLastParam()))          \
				EU_BREAKPOINT;                                                                                         \
		} while (false)

#else
	#define EU_ASSERT(...) ((void)0)
	#define EU_ENSURE(...) ((void)0)
#endif // EU_ENABLE_ASSERTS

#include <utility>

EU_NAMESPACE_BEGIN
using std::forward;
using std::move;
EU_NAMESPACE_END
