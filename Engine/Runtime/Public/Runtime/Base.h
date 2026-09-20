#pragma once

// Context Cracking

#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__) || defined(__linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MACOS 1
#else
#error "This compiler/OS combo is not supported."
#endif

// DLL Related stuff.

#if defined(OS_WINDOWS)
#if defined(RUNTIME_BUILD)
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API __attribute__((visibility("default")))
#endif

// Debug Break.
#ifdef CONFIG_DEBUG
#if defined(OS_WINDOWS)
#define DEBUG_BREAK() __debugbreak()
#elif defined(OS_LINUX) || defined(OS_MACOS)
#include <signal.h>
#define DEBUG_BREAK() raise(SIGTRAP)
#else
#error "This OS doesn't support debugbreak yet!"
#endif
#else
#define DEBUG_BREAK()
#endif

// Logging.

#if CONFIG_DEBUG

template <typename... Args>
void LogInfo(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) {
  std::println(
    "[INFO] {}",
    std::format(fmt, std::forward<Args>(args)...)
  );
}

template <typename... Args>
void LogWarning(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) {
  std::println(
    "[WARN] {}\n{}: {}",
    std::format(fmt, std::forward<Args>(args)...),
    loc.file_name(),
    loc.line()
  );
}

template <typename... Args>
void LogError(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) {
  std::println(
    stderr,
    "[ERROR] {}\n{}: {}",
    std::format(fmt, std::forward<Args>(args)...),
    loc.file_name(),
    loc.line()
  );
}

template <typename... Args>
void LogAssertionFailed(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) {
  std::println(
    stderr,
    "[ASSERTION FAILED] {}\n{}: {}",
    std::format(fmt, std::forward<Args>(args)...),
    loc.file_name(),
    loc.line()
  );
}

#else

template <typename... Args>
constexpr void LogInfo(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

template <typename... Args>
constexpr void LogWarning(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

template <typename... Args>
constexpr void LogError(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}


template <typename... Args>
void LogAssertionFailed(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

#endif

#if CONFIG_DEBUG

#define LOG_INFO(...) \
    ::LogInfo(std::source_location::current(), __VA_ARGS__)

#define LOG_WARN(...) \
    ::LogWarning(std::source_location::current(), __VA_ARGS__)

#define LOG_ERR(...) \
    ::LogError(std::source_location::current(), __VA_ARGS__)

#define ASSERT(condition)                              \
  do {                                                 \
    if (!(condition)) {                                \
        ::LogAssertionFailed(                          \
            std::source_location::current(),           \
            #condition);                               \
        DEBUG_BREAK();                                 \
    }                                                  \
  } while (false)

#else

#define LOG_INFO(...) ((void)0)
#define LOG_WARN(...) ((void)0)
#define LOG_ERR(...)  ((void)0)
#define LOG_ASSERT(condition, ...) ((void)0)

#endif

// Utils.

// Loop helpers macros.
#define EACH_INDEX(it, count) (s32 it = 0; it < (count); it += 1)

// Unit macros.
#define KB(n) (((u64)(n)) << 10)
#define MB(n) (((u64)(n)) << 20)
#define GB(n) (((u64)(n)) << 30)
#define TB(n) (((u64)(n)) << 40)
#define THOUSAND(n) ((n)*1000)
#define MILLION(n) ((n)*1000000)
#define BILLION(n) ((n)*1000000000)

// ALIGN_OF, MIN, MAX, CLAMP macros.
#define ALIGN_OF(T) __alignof(T) // @Pending(Platform): This will work just in clang and msvc.
#define MIN(A,B) (((A)<(B))?(A):(B))
#define MAX(A,B) (((A)>(B))?(A):(B))
#define CLAMP(A,X,B) (((X)<(A))?(A):((X)>(B))?(B):(X))

// Misc macros.
#define UNUSED(v) ((void)v)
#define DEFER_BLOCK(begin, end) for(s32 _i_ = ((begin), 0); !_i_; _i_ += 1, (end))
#define ALIGN_POW2(x,b) (((x) + (b) - 1)&(~((b) - 1))) // @Note: b has to be pow2. 
