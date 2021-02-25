#pragma once

#include <memory>

// DLL export

#ifndef PR_PLATFORM_WINDOWS
    #error Parrot only support Windows right now
#endif

// Assertion

#ifdef PR_DEBUG
    #define PR_ENABLE_ASSERTS
#endif

#ifdef PR_ENABLE_ASSERTS
    #define PR_ASSERT(x, ...)      { if(!(x)) { PR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define PR_CORE_ASSERT(x, ...) { if(!(x)) { PR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define PR_ASSERT(x, ...)     
    #define PR_CORE_ASSERT(x, ...) 
#endif

// Bit Operation

#define BIT(x) (1 << x)

// Event Function Binding

// #define PR_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)
#define PR_BIND_EVENT_FUNC(func) [this](auto&&... args) -> decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

namespace parrot {
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Scope<T> createScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    constexpr Ref<T> createRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<typename T, typename ... Args>
    constexpr WeakRef<T> createWeakRef(Args&& ... args) {
        return std::weak_ptr<T>(std::forward<Args>(args)...);
    }
}