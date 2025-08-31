#pragma once

#include <memory>
#include <functional>

#ifdef EG_PLATFORM_WINDOWS
#if EG_DYNAMIC_LINK
#ifdef EG_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif
#else
#define ENGINE_API
#endif
#else
#error This engine currently supports only Windows!
#endif

// Debugging helpers
#ifdef EG_DEBUG
#define EG_ENABLE_CHECKS
#endif

#ifdef EG_ENABLE_CHECKS
#define EG_CHECK(expr, msg) { if(!(expr)) { EG_ERROR("Check failed: {0}", msg); __debugbreak(); } }
#define EG_CORE_CHECK(expr, msg) { if(!(expr)) { EG_CORE_ERROR("Check failed: {0}", msg); __debugbreak(); } }
#else
#define EG_CHECK(expr, msg)
#define EG_CORE_CHECK(expr, msg)
#endif

namespace Engine {

    // Type-safe bitmask helper instead of BIT(x)
    constexpr unsigned int BitMask(unsigned int bit) { return 1u << bit; }

    // Event binding helper (cleaner than macro)
    template<typename T, typename Fn>
    auto BindEvent(T* instance, Fn&& fn)
    {
        return [instance, fn = std::forward<Fn>(fn)](auto&&... args) -> decltype(auto)
            {
                return (instance->*fn)(std::forward<decltype(args)>(args)...);
            };
    }

    // Memory helpers
    template<typename T>
    using Unique = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Unique<T> MakeUnique(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Shared = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Shared<T> MakeShared(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}
