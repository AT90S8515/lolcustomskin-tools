#ifndef LCS_ERROR_HPP
#define LCS_ERROR_HPP
#include <string>
#include <utility>
#include <sstream>

#define lcs_paste_impl(x, y) x ## y
#define lcs_paste(x, y) lcs_paste_impl(x, y)
#define lcs_assert_msg(msg, ...) do {                     \
        if(!(__VA_ARGS__)) {                              \
            ::LCS::throw_error(msg);                      \
        }                                                 \
    } while(false)
#define lcs_assert(...) do {                              \
        if(!(__VA_ARGS__)) {                              \
            ::LCS::throw_error(#__VA_ARGS__);             \
        }                                                 \
    } while(false)
#define lcs_rethrow(...)                                  \
    [&, func = __func__]() -> decltype(auto) {            \
        try {                                             \
            return __VA_ARGS__;                           \
        } catch (std::exception const &) {                \
            ::LCS::throw_error(func, ": " #__VA_ARGS__);  \
        }                                                 \
    }()
#define lcs_trace(...) ::LCS::ErrorTrace lcs_paste(trace_,__LINE__) {   \
        [&] () { ::LCS::push_error_msg(__VA_ARGS__); }                  \
    }

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define lcs_trace_func() ::LCS::ErrorTrace lcs_paste(trace_,__LINE__) {   \
    [func = __PRETTY_FUNCTION__, line = __LINE__] () {                    \
        ::LCS::push_error_msg(func, ':', line); }                         \
}

namespace LCS {
    [[noreturn]] extern void throw_error(char const* msg);

    extern std::stringstream& error_stack() noexcept;

    extern std::string error_stack_trace(std::string message) noexcept;

    template<typename...Args>
    inline void push_error_msg(Args&&...args) noexcept {
        auto& ss = error_stack();
        ss << '\n';
        (ss << ... << std::forward<Args>(args)) ;
    }

    template<typename Func>
    struct ErrorTrace : Func {
        inline ErrorTrace(Func&& func) noexcept : Func(std::move(func)) {}
        inline ~ErrorTrace() noexcept {
            if (std::uncaught_exceptions()) {
                Func::operator()();
            }
        }
    };
}
#endif // LCS_ERROR_HPP
