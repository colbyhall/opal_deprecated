// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define SF_PLATFORM_WINDOWS_UWP
	#endif
	#define SF_PLATFORM_WINDOWS
#else
	#error Unsupported platform
#endif

// Determine compiler
#if defined(__clang__)
	#define SF_COMPILER_CLANG
#elif defined(_MSC_VER)
	#define SF_COMPILER_MSVC
#else
	#error Unsupported compiler
#endif

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define SF_CPU_X86
	#if defined(__x86_64__) || defined(_M_X64)
		#define SF_CPU_ADDRESS_BITS 64
	#else
		#define SF_CPU_ADDRESS_BITS 32
	#endif
#else
	#error Unsupported CPU architecture
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#ifdef SF_COMPILER_CLANG
	#define SF_PRAGMA(x)				 _Pragma(#x)
	#define SF_SUPPRESS_WARNING_PUSH	 SF_PRAGMA(clang diagnostic push)
	#define SF_SUPPRESS_WARNING_POP		 SF_PRAGMA(clang diagnostic pop)
	#define SF_CLANG_SUPPRESS_WARNING(w) SF_PRAGMA(clang diagnostic ignored w)
#else
	#define SF_CLANG_SUPPRESS_WARNING(w)
#endif

#ifdef SF_COMPILER_MSVC
	#define SF_PRAGMA(x)				__pragma(x)
	#define SF_SUPPRESS_WARNING_PUSH	SF_PRAGMA(warning(push))
	#define SF_SUPPRESS_WARNING_POP		SF_PRAGMA(warning(pop))
	#define SF_MSVC_SUPPRESS_WARNING(w) SF_PRAGMA(warning(disable : w))
#else
	#define SF_MSVC_SUPPRESS_WARNING(w)
#endif

// Define inline macro
#if defined(SF_COMPILER_CLANG)
	#define SF_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#elif defined(SF_COMPILER_MSVC)
	#define SF_ALWAYS_INLINE __forceinline
#else
	#error Undefined inline
#endif

#define SF_NO_DISCARD [[nodiscard]]

// Cache line size (used for aligning to cache line)
#ifndef SF_CACHE_LINE_SIZE
	#define SF_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if defined(SF_COMPILER_CLANG) || defined(COMPILER_GCC)
	#define SF_FUNCTION_NAME __PRETTY_FUNCTION__
#elif defined(SF_COMPILER_MSVC)
	#define SF_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined
#endif

// OS-specific includes
#if defined(SF_PLATFORM_WINDOWS)
	#define SF_BREAKPOINT __debugbreak()
#else
	#error Unknown platform
#endif

#ifdef _DEBUG
	#define SF_BUILD_DEBUG 1
#else
	#define SF_BUILD_DEBUG 1
#endif

#define SF_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define SF_UNUSED(x) (void)x

// Crashes the application
#define SF_CRASH                                                                                                       \
	SF_SUPPRESS_WARNING_PUSH                                                                                           \
	SF_MSVC_SUPPRESS_WARNING(6011)                                                                                     \
	do {                                                                                                               \
		int* _ptr = nullptr;                                                                                           \
		*_ptr = 0;                                                                                                     \
	} while (false) SF_SUPPRESS_WARNING_POP

// Suppress warnings generated by the standard template library
#define SF_SUPPRESS_WARNINGS_STD_BEGIN                                                                                 \
	SF_SUPPRESS_WARNING_PUSH                                                                                           \
	SF_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5262)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5219)

#define SF_SUPPRESS_WARNINGS_STD_END SF_SUPPRESS_WARNING_POP

// Disable common warnings triggered by core when compiling with -Wall
#define SF_SUPPRESS_WARNINGS                                                                                           \
	SF_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                                        \
	SF_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                                               \
	SF_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                                         \
	SF_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                                     \
	SF_CLANG_SUPPRESS_WARNING("-Wold-style-cast")                                                                      \
	SF_CLANG_SUPPRESS_WARNING("-Wgnu-anonymous-struct")                                                                \
	SF_CLANG_SUPPRESS_WARNING("-Wnested-anon-types")                                                                   \
	SF_CLANG_SUPPRESS_WARNING("-Wglobal-constructors")                                                                 \
	SF_CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")                                                               \
	SF_CLANG_SUPPRESS_WARNING("-Wnonportable-system-include-path")                                                     \
	SF_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                                            \
	SF_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                                    \
	SF_CLANG_SUPPRESS_WARNING("-Wformat-nonliteral")                                                                   \
	SF_CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")                                                              \
	SF_CLANG_SUPPRESS_WARNING("-Wcast-align")                                                                          \
	SF_CLANG_SUPPRESS_WARNING("-Winvalid-offsetof")                                                                    \
	SF_CLANG_SUPPRESS_WARNING("-Wgnu-zero-variadic-macro-arguments")                                                   \
	SF_CLANG_SUPPRESS_WARNING("-Wdocumentation-unknown-command")                                                       \
	SF_CLANG_SUPPRESS_WARNING("-Wctad-maybe-unsupported")                                                              \
	SF_CLANG_SUPPRESS_WARNING("-Wdeprecated-copy")                                                                     \
	SF_CLANG_SUPPRESS_WARNING("-Wimplicit-int-float-conversion")                                                       \
                                                                                                                       \
	SF_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4100)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4626)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5027)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4324)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4625)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5026)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4623)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4201)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4371)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5045)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4583)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4582)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5219)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4826)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	SF_MSVC_SUPPRESS_WARNING(4127)

// Begin the sf namespace
#define SF_NAMESPACE_BEGIN                                                                                             \
	SF_SUPPRESS_WARNING_PUSH                                                                                           \
	SF_SUPPRESS_WARNINGS                                                                                               \
	namespace sf {

// End the sf namespace
#define SF_NAMESPACE_END                                                                                               \
	}                                                                                                                  \
	SF_SUPPRESS_WARNING_POP

// Begin the sf::core namespace
#define SF_CORE_NAMESPACE_BEGIN                                                                                        \
	SF_NAMESPACE_BEGIN                                                                                                 \
	namespace core {

// End the sf::core namespace
#define SF_CORE_NAMESPACE_END                                                                                          \
	}                                                                                                                  \
	SF_NAMESPACE_END

#define SF_HIDDEN_NAMESPACE_BEGIN namespace hidden {

#define SF_HIDDEN_NAMESPACE_END }

// Declare primitive types under core namespace
SF_CORE_NAMESPACE_BEGIN

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

constexpr usize KB = 1024;
constexpr usize MB = 1024 * KB;
constexpr usize GB = 1024 * MB;

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

SF_CORE_NAMESPACE_END

// Export primitive types to sf namespace
SF_NAMESPACE_BEGIN

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

using core::GB;
using core::KB;
using core::MB;

SF_NAMESPACE_END

// Turn on asserts if compiled in debug
#if defined(_DEBUG)
	#define SF_ENABLE_ASSERTS
#endif

#define SF_ENUM_CLASS_BITFIELD(Enum)                                                                                   \
	inline Enum& operator|=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((__underlying_type(Enum)&)A |= (__underlying_type(Enum))B);                                     \
	}                                                                                                                  \
	inline Enum& operator&=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((__underlying_type(Enum)&)A &= (__underlying_type(Enum))B);                                     \
	}                                                                                                                  \
	inline Enum& operator^=(Enum& A, Enum B) {                                                                         \
		return (Enum&)((__underlying_type(Enum)&)A ^= (__underlying_type(Enum))B);                                     \
	}                                                                                                                  \
	inline Enum operator|(Enum A, Enum B) { return (Enum)((__underlying_type(Enum))A | (__underlying_type(Enum))B); }  \
	inline Enum operator&(Enum A, Enum B) { return (Enum)((__underlying_type(Enum))A & (__underlying_type(Enum))B); }  \
	inline Enum operator^(Enum A, Enum B) { return (Enum)((__underlying_type(Enum))A ^ (__underlying_type(Enum))B); }  \
	inline Enum operator~(Enum A) { return (Enum) ~(__underlying_type(Enum))A; }

#ifdef SF_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define SF_ASSERT(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression)) {                                                                                       \
				SF_CRASH;                                                                                              \
			}                                                                                                          \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define SF_ENSURE(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression) && sf::core::_assert_failed_helper(                                                      \
									 false,                                                                            \
									 #expression,                                                                      \
									 __FILE__,                                                                         \
									 sf::u32(__LINE__),                                                                \
									 ##__VA_ARGS__,                                                                    \
									 sf::core::AssertLastParam()                                                       \
								 ))                                                                                    \
				SF_BREAKPOINT;                                                                                         \
		} while (false)

#else
	#define SF_ASSERT(...) ((void)0)
	#define SF_ENSURE(...) ((void)0)
#endif // SF_ENABLE_ASSERTS

#define SF_PANIC(msg)		 SF_ASSERT(false, msg)
#define SF_UNIMPLEMENTED	 SF_PANIC("Unimplemented!")
#define SF_INVALID_CODE_PATH SF_PANIC("Invalid code path!")

#include <utility>

SF_NAMESPACE_BEGIN
using std::forward;
using std::move;
SF_NAMESPACE_END
