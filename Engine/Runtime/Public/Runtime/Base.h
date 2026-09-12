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
void log_info(
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
void log_warn(
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
void log_error(
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
void log_assertion_failed(
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
constexpr void log_info(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

template <typename... Args>
constexpr void log_warn(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

template <typename... Args>
constexpr void log_error(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}


template <typename... Args>
void log_assertion_failed(
  std::source_location loc,
  std::format_string<Args...> fmt,
  Args&&... args
) noexcept {}

#endif

#if CONFIG_DEBUG

#define LOG_INFO(...) \
    ::log_info(std::source_location::current(), __VA_ARGS__)

#define LOG_WARN(...) \
    ::log_warn(std::source_location::current(), __VA_ARGS__)

#define LOG_ERR(...) \
    ::log_error(std::source_location::current(), __VA_ARGS__)

#define ASSERT(condition)                              \
  do {                                                 \
    if (!(condition)) {                                \
        ::log_assertion_failed(                        \
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
