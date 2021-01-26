#pragma once

// DLL export

#ifndef PR_PLATFORM_WINDOWS
    #error Parrot only support Windows right now
#endif

// Assertion

#ifdef PR_DEBUG
    #define PR_ENABLE_ASSERTS
#endif

#ifdef PR_ENABLE_ASSERTS
    #define PR_ASSERT(x, ...)     { if(!(x)) { PR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define PR_INT_ASSERT(x, ...) { if(!(x)) { PR_INT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define PR_ASSERT(x, ...)     
    #define PR_INT_ASSERT(x, ...) 
#endif

// Bit Operation

#define BIT(x) (1 << x)

// Event Function Binding

#define PR_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)