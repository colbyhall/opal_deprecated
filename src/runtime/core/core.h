// Copyright Colby Hall. All Rights Reserved.

#pragma once

// Determine platform
#if defined(_WIN32) || defined(_WIN64)
	#include <winapifamily.h>
	#if WINAPI_FAMILY == WINAPI_FAMILY_APP
		#define OP_PLATFORM_WINDOWS_UWP
	#endif
	#define OP_PLATFORM_WINDOWS 1
#else
	#error Unsupported platform
#endif
#ifndef OP_PLATFORM_WINDOWS
	#define OP_PLATFORM_WINDOWS 0
#endif

// Determine compiler
#if defined(__clang__)
	#define OP_COMPILER_CLANG 1
#elif defined(_MSC_VER)
	#define OP_COMPILER_MSVC 1
#else
	#error Unsupported compiler
#endif
#ifndef OP_COMPILER_CLANG
	#define OP_COMPILER_CLANG 0
#endif
#ifndef OP_COMPILER_MSVC
	#define OP_COMPILER_MSVC 0
#endif
#ifndef OP_COMPILER_GCC
	#define OP_COMPILER_GCC 0
#endif

// Detect CPU architecture
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__) || defined(_M_IX86)
	// X86 CPU architecture
	#define OP_CPU_X86 1
	#if defined(__x86_64__) || defined(_M_X64)
		#define OP_CPU_ADDRESS_BITS 64
	#else
		#define OP_CPU_ADDRESS_BITS 32
	#endif
#else
	#error Unsupported CPU architecture
#endif
#ifndef OP_GPU_X86
	#define OP_GPU_X86 0
#endif

// Pragmas to store / restore the warning state and to disable individual
// warnings
#if OP_COMPILER_CLANG
	#define OP_PRAGMA(x)				 _Pragma(#x)
	#define OP_SUPPRESS_WARNING_PUSH	 OP_PRAGMA(clang diagnostic push)
	#define OP_SUPPRESS_WARNING_POP		 OP_PRAGMA(clang diagnostic pop)
	#define OP_CLANG_SUPPRESS_WARNING(w) OP_PRAGMA(clang diagnostic ignored w)
#else
	#define OP_CLANG_SUPPRESS_WARNING(w)
#endif

#if OP_COMPILER_MSVC
	#define OP_PRAGMA(x)				__pragma(x)
	#define OP_SUPPRESS_WARNING_PUSH	OP_PRAGMA(warning(push))
	#define OP_SUPPRESS_WARNING_POP		OP_PRAGMA(warning(pop))
	#define OP_MSVC_SUPPRESS_WARNING(w) OP_PRAGMA(warning(disable : w))
#else
	#define OP_MSVC_SUPPRESS_WARNING(w)
#endif

// Define inline macro
#if OP_COMPILER_CLANG
	#define OP_ALWAYS_INLINE __inline__ __attribute__((always_inline))
#elif OP_COMPILER_MSVC
	#define OP_ALWAYS_INLINE __forceinline
#else
	#error Undefined inline
#endif

#define OP_NO_DISCARD [[nodiscard]]

// Cache line size (used for aligning to cache line)
#ifndef OP_CACHE_LINE_SIZE
	#define OP_CACHE_LINE_SIZE 64
#endif

// Define macro to get current function name
#if OP_COMPILER_CLANG || OP_COMPILER_GCC
	#define OP_FUNCTION_NAME __PRETTY_FUNCTION__
#elif OP_COMPILER_MSVC
	#define OP_FUNCTION_NAME __FUNCTION__
#else
	#error Undefined
#endif

// OS-specific includes
#if OP_PLATFORM_WINDOWS
	#define OP_BREAKPOINT __debugbreak()
#else
	#error Unknown platform
#endif

#ifdef _DEBUG
	#define OP_BUILD_DEBUG 1
#else
	#define OP_BUILD_DEBUG 0
#endif

#define OP_THREAD_LOCAL __declspec(thread)

// Macro to indicate that a parameter / variable is unused
#define OP_UNUSED(x) (void)x

// Crashes the application
#define OP_CRASH                                                                                                       \
	OP_SUPPRESS_WARNING_PUSH                                                                                           \
	OP_MSVC_SUPPRESS_WARNING(6011)                                                                                     \
	do {                                                                                                               \
		int* _ptr = nullptr;                                                                                           \
		*_ptr = 0;                                                                                                     \
	} while (false) OP_SUPPRESS_WARNING_POP

// Suppress warnings generated by the standard template library
#define OP_SUPPRESS_WARNINGS_STD_BEGIN                                                                                 \
	OP_SUPPRESS_WARNING_PUSH                                                                                           \
	OP_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5262)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5219)

#define OP_SUPPRESS_WARNINGS_STD_END OP_SUPPRESS_WARNING_POP

// Disable common warnings triggered by core when compiling with -Wall
#define OP_SUPPRESS_WARNINGS                                                                                           \
	OP_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                                        \
	OP_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                                               \
	OP_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                                         \
	OP_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                                     \
	OP_CLANG_SUPPRESS_WARNING("-Wold-style-cast")                                                                      \
	OP_CLANG_SUPPRESS_WARNING("-Wgnu-anonymous-struct")                                                                \
	OP_CLANG_SUPPRESS_WARNING("-Wnested-anon-types")                                                                   \
	OP_CLANG_SUPPRESS_WARNING("-Wglobal-constructors")                                                                 \
	OP_CLANG_SUPPRESS_WARNING("-Wexit-time-destructors")                                                               \
	OP_CLANG_SUPPRESS_WARNING("-Wnonportable-system-include-path")                                                     \
	OP_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                                            \
	OP_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                                    \
	OP_CLANG_SUPPRESS_WARNING("-Wformat-nonliteral")                                                                   \
	OP_CLANG_SUPPRESS_WARNING("-Wcovered-switch-default")                                                              \
	OP_CLANG_SUPPRESS_WARNING("-Wcast-align")                                                                          \
	OP_CLANG_SUPPRESS_WARNING("-Winvalid-offsetof")                                                                    \
	OP_CLANG_SUPPRESS_WARNING("-Wgnu-zero-variadic-macro-arguments")                                                   \
	OP_CLANG_SUPPRESS_WARNING("-Wdocumentation-unknown-command")                                                       \
	OP_CLANG_SUPPRESS_WARNING("-Wctad-maybe-unsupported")                                                              \
	OP_CLANG_SUPPRESS_WARNING("-Wdeprecated-copy")                                                                     \
	OP_CLANG_SUPPRESS_WARNING("-Wimplicit-int-float-conversion")                                                       \
                                                                                                                       \
	OP_MSVC_SUPPRESS_WARNING(4619)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4514)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4710)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4711)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4820)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4100)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4626)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5027)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4365)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4324)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4625)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5026)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4623)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4201)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4371)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5045)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4583)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4582)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5219)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4826)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(5264)                                                                                     \
	OP_MSVC_SUPPRESS_WARNING(4127)

// Begin the op namespace
#define OP_NAMESPACE_BEGIN                                                                                             \
	OP_SUPPRESS_WARNING_PUSH                                                                                           \
	OP_SUPPRESS_WARNINGS                                                                                               \
	namespace op {

// End the op namespace
#define OP_NAMESPACE_END                                                                                               \
	}                                                                                                                  \
	OP_SUPPRESS_WARNING_POP

// Begin the op::core namespace
#define OP_CORE_NAMESPACE_BEGIN                                                                                        \
	OP_NAMESPACE_BEGIN                                                                                                 \
	namespace core {

// End the op::core namespace
#define OP_CORE_NAMESPACE_END                                                                                          \
	}                                                                                                                  \
	OP_NAMESPACE_END

#define OP_HIDDEN_NAMESPACE_BEGIN namespace hidden {

#define OP_HIDDEN_NAMESPACE_END }

// Declare primitive types under core namespace
OP_CORE_NAMESPACE_BEGIN

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

OP_CORE_NAMESPACE_END

// Export primitive types to op namespace
OP_NAMESPACE_BEGIN

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

OP_NAMESPACE_END

// Turn on asserts if compiled in debug
#if defined(_DEBUG)
	#define OP_ENABLE_ASSERTS
#endif

#define OP_ENUM_CLASS_BITFIELD(Enum)                                                                                   \
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

#ifdef OP_ENABLE_ASSERTS
	// Crashes application if expression evaluates to false. Usage:
	// ASSERT(condition) or ASSERT(condition, message)
	#define OP_ASSERT(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression)) {                                                                                       \
				OP_CRASH;                                                                                              \
			}                                                                                                          \
		} while (false)

	// Halts application if expression evaluates to false but can resume. Usage:
	// ENSURE(condition) or ENSURE(condition, message)
	#define OP_ENSURE(expression, ...)                                                                                 \
		do {                                                                                                           \
			if (!(expression) && op::core::_assert_failed_helper(                                                      \
									 false,                                                                            \
									 #expression,                                                                      \
									 __FILE__,                                                                         \
									 op::u32(__LINE__),                                                                \
									 ##__VA_ARGS__,                                                                    \
									 op::core::AssertLastParam()                                                       \
								 ))                                                                                    \
				OP_BREAKPOINT;                                                                                         \
		} while (false)

#else
	#define OP_ASSERT(...) ((void)0)
	#define OP_ENSURE(...) ((void)0)
#endif // OP_ENABLE_ASSERTS

#define OP_PANIC(msg)		 OP_ASSERT(false, msg)
#define OP_UNIMPLEMENTED	 OP_PANIC("Unimplemented!")
#define OP_INVALID_CODE_PATH OP_PANIC("Invalid code path!")

#include <utility>

OP_NAMESPACE_BEGIN
using std::forward;
using std::move;
OP_NAMESPACE_END
